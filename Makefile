##
## This file is part of the avr-gcc-examples project.
##
## Copyright (C) 2008 Uwe Hermann <uwe@hermann-uwe.de>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
##

MCU = attiny85
PROGRAM=$(MCU)-clock
CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
CFLAGS += -flto -Wall -Os -mmcu=$(MCU)
CXXFLAGS += -flto -Wall -Os -mmcu=$(MCU)
LDFLAGS += -flto
SRC     = $(shell find . -name '*.c' -o -name '*.cpp' ! -path './cmake-build-debug/*' ! -path './cmake-build-default/*')
OBJ     = $(patsubst %.cpp,%.o,$(SRC))

# Be silent per default, but 'make V=1' will show all compiler calls.
Q := @
all: $(PROGRAM).hex

$(PROGRAM).elf: $(OBJ)
	@printf "  OBJ: $(OBJ)\n"
	@printf "  LD      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@ 

$(PROGRAM).hex: $(PROGRAM).elf
	@printf "  OBJCOPY $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(OBJCOPY) -O ihex $< $@
	@printf "$(avr-size $(PROGRAM.hex))\n"	


%.o: %.cpp
	@printf "  CXX      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(CXX) $(CXXFLAGS) -o $@ -c $<

%.o: %.c
	@printf "  CC      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(CC) $(CFLAGS) -o $@ -c $<

flash: $(PROGRAM).hex
	@printf "  FLASH   $(PROGRAM).hex\n"
	$(Q)avrdude -c usbtiny -p t13 -U flash:w:$(PROGRAM).hex

clean:
	@printf "  CLEAN   $(subst $(shell pwd)/,,$(OBJS))\n"
	$(Q)rm -f $(OBJS)
	@printf "  CLEAN   $(PROGRAM).elf\n"
	$(Q)rm -f *.elf
	@printf "  CLEAN   $(PROGRAM).hex\n"
	$(Q)rm -f *.hex
	$(Q) rm -f *.o
