#pragma once

#include <cstdint>

class ibutton {
  public:
    enum class active_level : uint8_t { low, high };

    ibutton(active_level level) : active_level_{level} {}
    virtual ~ibutton() = default;

    bool is_pressed() const { return pin_read() == static_cast<uint8_t>(active_level_); }

  private:
    const active_level active_level_;

    virtual uint8_t pin_read() const = 0;
};
