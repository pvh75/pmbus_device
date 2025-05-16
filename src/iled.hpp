#pragma once

#include <cstdint>

class iled {
  public:
    enum class active_level : uint8_t { low, high };

    iled(active_level level) : active_level_{level} {}
    virtual ~iled() = default;

    enum class led_action : uint8_t { off, on, toggle };

    void on() const { action(led_action::on); }
    void off() const { action(led_action::off); }
    void toggle() const { action(led_action::toggle); }

  private:
    const active_level active_level_;

    virtual void pin_set() const = 0;
    virtual void pin_reset() const = 0;
    virtual void pin_toggle() const = 0;

    void action(led_action action) const {
        if (action == led_action::toggle) {
            pin_toggle();
        } else if ((static_cast<uint8_t>(action) ^ static_cast<uint8_t>(active_level_)) ==
                   0) {
            pin_set();
        } else {
            pin_reset();
        }
    }
};
