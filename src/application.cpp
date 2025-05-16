#include "application.h"
#include "log.h"
#include "smbus.h"

namespace rtos = cpp_freertos;

extern "C" {
struct smbus* psmbus;

void pmbus_logger_init(void);
bool pmbus_log(ppmbus_cmd_t pcmd, uint8_t dir, bool status);

uint8_t smbus_slave_addr(void) {
    return (0x5B << 1);
}

bool smbus_cmd_handler(struct smbus* psmbus) {
    pmbus_cmd_t cmd = *smbus_get_curr_cmd(psmbus);
    uint8_t* pdata = smbus_get_data_buff(psmbus);
    uint8_t direction = smbus_is_read_cmd(psmbus) ? PMBUS_READ : PMBUS_WRITE;
    bool status = pmbus_cmd_handler(cmd.code, direction, pdata) == PMBUS_OK;

    pmbus_log(&cmd, direction, status);

    return status;
}

bool smbus_cmd_parser(struct smbus* psmbus) {
    uint8_t cmd_code = smbus_get_data_buff(psmbus)[0];
    pmbus_cmd_t cmd = *pmbus_get_cmd(cmd_code);
    bool status = true;

    if (cmd.prot == PMBUS_UNREALIZED) {
        cmd.code = cmd_code;
        status = false;
    }

    smbus_set_curr_cmd(psmbus, &cmd);

    return status;
}

bool smbus_init(SMBUS_HandleTypeDef* hsmbus) {
    psmbus = smbus_create(hsmbus->Instance, SMBUS_BUFF_SIZE);

    return psmbus != NULL;
}

void start_info(void) {
    LOG_INFO("PMBus stack v%s %s",
             pmbus_stack_version_str(),
             pmbus_stack_build_date_str());
    LOG_INFO("PMBus device %#02x start...", smbus_slave_addr() >> 1);
}

void run_time_stats_info(void) {
    static char stats[1024];

    vTaskGetRunTimeStats(stats);
    LOG_INFO("\r\n%s\r\n", stats);
}

void MX_FREERTOS_Init(void);

void application_init(void) {
    MX_FREERTOS_Init();
    pmbus_logger_init();
}

void application(void) {
    vTaskStartScheduler();

    while (true) {
    }
}
}
