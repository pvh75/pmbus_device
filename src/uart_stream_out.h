#pragma once

#include "istream_out.h"
#include "singleton.h"
#include "hardware.h"

class uart_stream_out : public istream_out {
  public:
    explicit uart_stream_out(uart_handle_type uart) : uart_{uart} {}
    virtual ~uart_stream_out() = 0;

    bool write(uint8_t* data, uint16_t length) override;

  private:
    const uart_handle_type uart_;
};

class uart3_stream_out : public static_singleton<uart3_stream_out>,
                         public uart_stream_out {
  public:
    friend class static_singleton<uart3_stream_out>;
    ~uart3_stream_out() = default;

  private:
    uart3_stream_out() : uart_stream_out{huart3} {};
};
