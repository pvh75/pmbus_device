#!/bin/bash

./build.sh release

openocd -f interface/stlink.cfg -f target/stm32h7x.cfg \
 -c 'program ./build/Release/pmbus_device_h7.elf verify reset exit'
