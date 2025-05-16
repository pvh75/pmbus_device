#pragma once

#include "ilogger.h"

#include <mutex.hpp>

class istream_out;

namespace rtos = cpp_freertos;

class logger : public ilogger {
  public:
    enum class color_enable : bool { no, yes };

    logger(level log_level, color_enable color, istream_out& out)
        : level_{log_level}, color_enable_{color}, out_{out} {}

    bool log(level log_level, const char* src, const char* format, ...) override;

    auto level_string(level log_level) const -> const char*;
    auto level_color_string(level log_level) const -> const char*;

    virtual ~logger() = 0;

  private:
    level level_;
    color_enable color_enable_;

    istream_out& out_;
    rtos::MutexStandard mutex_{};
};
