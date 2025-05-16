#pragma once

#include "uart_logger.h"

using current_logger = uart3_logger;

#define LOG(level, source, ...) \
    (current_logger::instance().log((level), (source), __VA_ARGS__))

#define LOG_EMERGENCY(...) \
    (current_logger::instance().log(logger::level::emergency, __func__, __VA_ARGS__))

#define LOG_ERROR(...) \
    (current_logger::instance().log(logger::level::error, __func__, __VA_ARGS__))

#define LOG_WARNING(...) \
    (current_logger::instance().log(logger::level::warning, __func__, __VA_ARGS__))

#define LOG_INFO(...) \
    (current_logger::instance().log(logger::level::info, __func__, __VA_ARGS__))

#define LOG_DEBUG(...) \
    (current_logger::instance().log(logger::level::debug, __func__, __VA_ARGS__))
