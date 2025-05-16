#pragma once

#include "hardware.h"
#include "ibutton.hpp"

namespace hw {

class button : public ibutton {
  public:
    button(uint32_t port, uint16_t pin, ibutton::active_level level)
        : ibutton{level}, port_{port}, pin_{pin} {}
    button(const button&) = delete;
    button& operator=(button&&) = delete;
    button(button&&) = delete;
    button& operator=(const button&) = delete;
    ~button() = default;

  private:
    const uint32_t port_;
    const uint16_t pin_;

    uint8_t pin_read() const override { return gpio_pin_read(port_, pin_); }
};

}    // namespace hw
