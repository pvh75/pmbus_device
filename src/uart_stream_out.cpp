#include "uart_stream_out.h"
#include "hardware.h"

uart_stream_out::~uart_stream_out() {}

bool uart_stream_out::write(uint8_t* data, uint16_t length) {
    return uart_transmit(uart_, data, length);
}
