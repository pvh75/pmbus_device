#pragma once

#include "logger.h"
#include "singleton.h"
#include "uart_stream_out.h"

class uart3_logger : public static_singleton<uart3_logger>, public logger {
  public:
    friend class static_singleton<uart3_logger>;

  private:
    uart3_logger()
        : logger{logger::level::debug, color_enable::yes, uart3_stream_out::instance()} {}
};
