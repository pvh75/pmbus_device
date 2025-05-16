#pragma once

#include <cstdint>

class istream_out {
  public:
    virtual ~istream_out() = default;

    virtual bool write(uint8_t* data, uint16_t length) = 0;
};
