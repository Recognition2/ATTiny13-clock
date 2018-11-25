/*
 * This file is part of the avr-gcc-examples project.
 *
 * Copyright (C) 2008 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include "pin_config.h"

#include <avr/io.h>
#include <avr/wdt.h>

#include <util/delay.h>
#include "I2c.h"

void systemReset();


int main() {
    auto i2c = I2c(0x40);
    uint8_t reg = i2c.readReg(0);
    i2c.writeReg(1, reg);
    const int delay = 100;
    volatile bool is_running = true;

    DDRB |= 0b111111;    /* Set PORTB bit 4 to output. */
    PORTB = 0;
	while (is_running) {
	    PORTB |= 1<<BLINK_PIN;
        _delay_us(delay);

        PORTB &= ~(1<<BLINK_PIN);
        _delay_us(100*delay);

    }

	systemReset();
	return 0;
}

void systemReset() {
    wdt_disable();
    wdt_enable(WDTO_15MS);
    while(1);
}
