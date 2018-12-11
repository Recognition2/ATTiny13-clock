#ifndef LEDCLOCK_I2C_H
#define LEDCLOCK_I2C_H

#include "pin_config.h"
#include <avr/io.h>

class I2c {
public:
    explicit I2c(uint8_t address);
    ~I2c() = default;

    enum Op {
        Write = 0x0,
        Read = 0x1,
    };
    // Highest-level functions
    uint8_t readReg(uint8_t reg);
    bool writeReg(uint8_t reg, uint8_t data);

    bool writeAddress(Op o) { return writeByte(address | o); };
    // Raw byte transfer
    bool writeByte(uint8_t);
    uint8_t readByte(bool ack = false);

    // Start/stop transaction
    void startCondition();
    void stopCondition();

private:
    const bool useClockStretching = false;
    const uint8_t address;

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
