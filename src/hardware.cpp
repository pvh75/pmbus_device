#include "hardware.h"

bool uart_transmit(uart_handle_type huart, uint8_t* pdata, uint16_t data_len) {
    return HAL_UART_Transmit(&huart, pdata, data_len, 100) == HAL_OK;
}

void gpio_pin_set(uint32_t port, uint16_t pin) {
    HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef*>(port), pin, GPIO_PIN_SET);
}

void gpio_pin_clear(uint32_t port, uint16_t pin) {
    HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef*>(port), pin, GPIO_PIN_RESET);
}

void gpio_pin_toggle(uint32_t port, uint16_t pin) {
    HAL_GPIO_TogglePin(reinterpret_cast<GPIO_TypeDef*>(port), pin);
}

uint8_t gpio_pin_read(uint32_t port, uint16_t pin) {
    return HAL_GPIO_ReadPin(reinterpret_cast<GPIO_TypeDef*>(port), pin);
}
