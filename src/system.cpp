#include "system.h"
#include "FreeRTOS.h"
#include "hardware.h"

#include <cstdio>

system_time_ms sys_time_ms() {
    return static_cast<system_time_ms>(HAL_GetTick());
}
