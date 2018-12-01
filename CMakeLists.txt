cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)

set(AVR_MCU attiny85)
project(tiny13clock)

include_directories(src)

add_avr_executable(tiny13clock
    src/I2c.cpp
    src/I2c.h
		src/BMP280.cpp
		src/BMP280.h
		src/DS1307.cpp
		src/DS1307.h
    src/main.cpp
    src/pin_config.h
    Makefile)
