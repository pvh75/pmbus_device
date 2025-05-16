#pragma once

#include <cstdint>

class ilogger {
  public:
    enum class level : uint8_t { emergency, error, warning, info, debug };

    virtual bool log(level log_level, const char* src, const char* format, ...) = 0;

    virtual ~ilogger() = default;
};
