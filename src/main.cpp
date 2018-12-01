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
#include <stdarg.h>
#include "I2c.h"
#include "DS1307.h"

void systemReset();
void writeCharliePlex4(int ledNum, bool value);


inline void pinHigh(int pin) {
    DDRB &= ~(1 << pin); // DDR is 0, PORT unknown -> tristate -> input or pul-up
    PORTB |= (1 << pin);
}

inline void pinLow(int pin) {
    PORTB &= ~(1<<pin); // write low
    DDRB |= (1<<pin); // configure as output
}

inline void pinHighZ(int pin) {
    DDRB &= ~(1<<pin); // DDR is 0, PORT unknown -> tristate -> input or pul-up
    PORTB &= ~(1<<pin);
}

void charliePlexCurrentTime(DS1307& rtc) {
    uint8_t hours = rtc.getHours();
    uint8_t minutes = rtc.getMinutes();
    uint8_t seconds = rtc.getSeconds();
    for (int i = 0; i < 5; i++) {
        writeCharliePlex4(i, hours & (1<<5));
        hours <<= 1;
    }
    for (int i = 0; i < 6; i++) {
        writeCharliePlex4(i+5, minutes & (1<<6));
        minutes <<= 1;
    }
    writeCharliePlex4(11,seconds&1);
}

int main() {
    DS1307 rtc;

    bool is_running = true;
    while (is_running) {
        charliePlexCurrentTime(rtc);
        _delay_ms(100);
    }
}

/**
 *
 * @param ledNum
 * @param value
 */
void writeCharliePlex4(int ledNum, bool value) {
    if (!value) {
        pinHighZ(0);
        pinHighZ(1);
        pinHighZ(2);
        pinHighZ(3);
    }
    switch (ledNum) {
        case 0:
            pinHighZ(2);
            pinHighZ(3);
            pinLow(1);
            pinHigh(0);
            break;

        case 1:
            pinHighZ(2);
            pinHighZ(3);
            pinLow(0);
            pinHigh(1);
            break;

        case 2:
            pinHighZ(0);
            pinHighZ(3);
            pinLow(2);
            pinHigh(1);
            break;

        case 3:
            pinHighZ(0);
            pinHighZ(3);
            pinLow(1);
            pinHigh(2);
            break;

        case 4:
            pinHighZ(0);
            pinHighZ(1);
            pinLow(3);
            pinHigh(2);
            break;

        case 5:
            pinHighZ(0);
            pinHighZ(1);
            pinLow(2);
            pinHigh(3);
            break;

        case 6:
            pinHighZ(1);
            pinHighZ(3);
            pinLow(2);
            pinHigh(0);
            break;

        case 7:
            pinHighZ(1);
            pinHighZ(3);
            pinLow(0);
            pinHigh(2);
            break;

        case 8:
            pinHighZ(0);
            pinHighZ(2);
            pinLow(3);
            pinHigh(1);
            break;

        case 9:
            pinHighZ(0);
            pinHighZ(2);
            pinLow(1);
            pinHigh(3);
            break;

        case 10:
            pinHighZ(1);
            pinHighZ(2);
            pinLow(3);
            pinHigh(0);
            break;

        case 11:
            pinHighZ(1);
            pinHighZ(2);
            pinLow(0);
            pinHigh(3);
    }
    _delay_us(100);
}

int blink_main() {
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
