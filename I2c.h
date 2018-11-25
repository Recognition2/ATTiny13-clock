//
// Created by gregory on 19/11/18.
//

#ifndef LEDCLOCK_I2C_H
#define LEDCLOCK_I2C_H

#include "pin_config.h"
#include <avr/io.h>


class I2c {
public:
    I2c(uint8_t address);
    ~I2c() = default;

    uint8_t readReg(uint8_t reg);
    bool writeReg(uint8_t reg, uint8_t data);

private:
    const bool useClockStretching = false;
    uint8_t address;

    // Raw bytes
    bool writeByte(uint8_t);
    uint8_t readByte(bool ignoreAck = false);

    // Start cond & addr & read/write bit

    void startCondition();
    void stopCondition();

    // Most basic operations
    void clockHigh(bool ignoreClockStretching = false);
    void clockLow();

    void dataHigh();
    void dataLow();
    void dataInput();

    bool readBit();
    bool getAck() { return !readBit(); }
};


#endif //LEDCLOCK_I2C_H
