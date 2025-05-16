#include "pmbus_logger.h"
#include "log.h"
#include "pmbus.h"

pmbus_logger_thread::pmbus_logger_thread()
    : Thread(pmbus_logger_task_name,
             pmbus_logger_task_stack_size,
             pmbus_logger_task_priority),
      logger_{current_logger::instance()} {}

void pmbus_logger_thread::start() {
    this->Start();
}

bool pmbus_logger_thread::log(pmbus_cmd_info* pinfo) {
    return buff_.EnqueueFromISR(pinfo, nullptr);
}

void pmbus_logger_thread::Run() {
    pmbus_cmd_info info{};

    while (true) {
        buff_.Dequeue(&info);

        logger_.log(info.status ? logger::level::debug : logger::level::error,
                    "pmbus",
                    "0x%02X %s %s",
                    info.code,
                    info.realized ? info.dir == PMBUS_READ ? "R" : "W" : "UN",
                    info.name == nullptr ? "empty" : info.name);
    }
}

pmbus_logger_thread& pmbus_logger_thread_instance(void) {
    static pmbus_logger_thread pmbus_logger{};

    return pmbus_logger;
}

extern "C" {

void pmbus_logger_init(void) {
    pmbus_logger_thread_instance().start();
}

bool pmbus_log(ppmbus_cmd_t pcmd, uint8_t dir, bool status) {
    auto realized{pcmd->prot == PMBUS_UNREALIZED ? false : true};
    auto code{pcmd->code};
    auto name{code > pmbus_cmd_names.max_size() ? "out of range" : pmbus_cmd_names[code]};

    pmbus_cmd_info info{name, code, dir, realized, status};

    return pmbus_logger_thread_instance().log(&info);
}
}
