//
// Created by gregory on 01/12/18.
//

#ifndef TINY13CLOCK_DS1307_H
#define TINY13CLOCK_DS1307_H

#include "I2c.h"

class DS1307 {
public:
    explicit DS1307();
    ~DS1307() = default;
    uint8_t getSeconds();
    uint8_t getMinutes();
    uint8_t getHours();

private:
    I2c i2c;
    uint8_t bcdToDecimal(uint8_t);

    static const uint8_t secondsRegister = 0x00;
    static const uint8_t minutesRegister = 0x01;
    static const uint8_t hoursRegister = 0x02;
    static const uint8_t address = 0b1101000; // Datasheet
};


#endif //TINY13CLOCK_DS1307_H
