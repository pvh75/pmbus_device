#pragma once

#include <cstdint>

#include "main.h"
#include "usart.h"

using uart_handle_type = UART_HandleTypeDef;

bool uart_transmit(uart_handle_type, uint8_t*, uint16_t);

void gpio_pin_set(uint32_t port, uint16_t pin);
void gpio_pin_clear(uint32_t port, uint16_t pin);
void gpio_pin_toggle(uint32_t port, uint16_t pin);
uint8_t gpio_pin_read(uint32_t port, uint16_t pin);
