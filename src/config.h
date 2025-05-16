#pragma once

#include "button.hpp"
#include "led.hpp"

extern "C" {
#include "FreeRTOSConfig.h"
}

const auto button_port{reinterpret_cast<uint32_t> BUTTON_GPIO_Port};
const auto button_pin{BUTTON_Pin};
const auto button_active_level{hw::button::active_level::high};

const auto red_led_port{reinterpret_cast<uint32_t> RED_LED_GPIO_Port};
const auto red_led_pin{RED_LED_Pin};
const auto red_led_active_level{hw::led::active_level::high};

const auto green_led_port{reinterpret_cast<uint32_t> GREEN_LED_GPIO_Port};
const auto green_led_pin{GREEN_LED_Pin};
const auto green_led_active_level{hw::led::active_level::high};

const auto yellow_led_port{reinterpret_cast<uint32_t> YELLOW_LED_GPIO_Port};
const auto yellow_led_pin{YELLOW_LED_Pin};
const auto yellow_led_active_level{hw::led::active_level::high};

const auto pmbus_logger_task_name{"pmbus_logger"};
const auto pmbus_logger_task_stack_size{configMINIMAL_STACK_SIZE * 2};
const auto pmbus_logger_task_priority{configMAX_PRIORITIES - 1};
const auto pmbus_logger_queue_size{64};
