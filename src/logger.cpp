#include "logger.h"
#include "istream_out.h"
#include "system.h"
#include "text_colors.h"

#include <array>
#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace {

using log_time_str = std::array<char, 32>;

log_time_str log_time() {
    log_time_str time_str{};
    system_time_ms time_ms = sys_time_ms();

    std::snprintf(time_str.data(),
                  time_str.size(),
                  "%012.3f",
                  ((time_ms / 1000) + (time_ms % 1000) / 1000.0));

    return time_str;
}

}    // namespace

logger::~logger() {}

bool logger::log(level log_level, const char* src, const char* format, ...) {
    rtos::LockGuard lg{mutex_};

    if (static_cast<uint8_t>(log_level) > static_cast<uint8_t>(level_)) {
        return false;
    }

    static std::array<uint8_t, 512> logger_string;
    static std::array<uint8_t, 256> format_string;

    va_list arg;
    va_start(arg, format);
    vsnprintf(reinterpret_cast<char*>(format_string.data()),
              format_string.size(),
              format,
              arg);
    va_end(arg);

    auto time_str = log_time();

    std::snprintf(
            reinterpret_cast<char*>(logger_string.data()),
            logger_string.max_size(),
            "%s <%s%s%s>\t[%s] %s\n",
            time_str.data(),
            level_color_string(log_level),
            level_string(log_level),
            color_enable_ == color_enable::yes ? text_color_default : text_color_empty,
            src,
            format_string.data());

    out_.write(logger_string.data(),
               static_cast<uint16_t>(
                       std::strlen(reinterpret_cast<const char*>(logger_string.data()))));

    return true;
}

auto logger::level_string(level log_level) const -> const char* {
    static const char* level_strings[] = {
            "EMERGENCY", "ERROR", "WARNING", "INFO", "DEBUG"};

    auto level = static_cast<uint8_t>(log_level);

    if (level > sizeof(level_strings) / sizeof(level_strings[0])) {
        return "UNDEFINED";
    }

    return level_strings[level];
}

auto logger::level_color_string(level log_level) const -> const char* {
    if (color_enable_ == color_enable::no) {
        return text_color_empty;
    }

    switch (log_level) {
        case level::emergency:
            return text_color_red;
        case level::error:
            return text_color_red;
        case level::warning:
            return text_color_orange;
        case level::info:
            return text_color_green;
        case level::debug:
            return text_color_blue;
        default:
            return text_color_red;
    }
}
