cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)
set(AVR_UPLOADTOOL micronucleus )
set(AVR_PROGRAMMER micronucleus)
set(AVR_UPLOADTOOL_PORT usb)


set(AVR_MCU attiny85)
# set fuses
set(MCU_SPEED "1200000UL")

include(/home/gregory/git/cmake-avr/generic-gcc-avr.cmake)
project(ledclock)

set(SOURCE_FILES
		main.cpp
		I2c.cpp
		)

#CC = avr-gcc
#OBJCOPY = avr-objcopy
#CFLAGS += -Wall -g -Os -mmcu=$(MCU)
#LDFLAGS +=
#OBJS = $(PROGRAM).o

add_avr_executable(ledclock
		${SOURCE_FILES}
		)
