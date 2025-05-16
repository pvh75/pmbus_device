#pragma once

#include "hardware.h"
#include "iled.hpp"

namespace hw {

class led : public iled {
  public:
    led(uint32_t port, uint16_t pin, iled::active_level level)
        : iled{level}, port_{port}, pin_{pin} {}

  private:
    const uint32_t port_;
    const uint16_t pin_;

    void pin_set() const override { gpio_pin_set(port_, pin_); }
    void pin_reset() const override { gpio_pin_clear(port_, pin_); }
    void pin_toggle() const override { gpio_pin_toggle(port_, pin_); }
};

}    // namespace hw
