//
// Created by gregory on 19/11/18.
//

#include "I2c.h"
#include <util/delay.h>

#define I2C_READ                    0b1
#define I2C_WRITE                   0b0

#define I2C_CLOCK_CYCLE_DELAY       NOP4

// DDRx: Data Direction Register
// PORTx: your port
// PINx: the value on the pin. Writing a 1 does a toggle

// DDxn written 1, Pxn is configured as output pin.
// If it is written logic zero, Pxn is configured as input pin
// If PORTxn is written logic one when the pin is configured as
// an input pin, the pull-up resistor is activated;

// --> Zero:   DDR = 1, PORT = 0
// --> One:    DDR = 1, PORT = 1
// --> Input:  DDR = 0, PORT = 0 - Tri-state, high-impedance
// --> High-Z: DDR = 0, PORT = 1 // Pull-Up


I2c::I2c(uint8_t address) {
    this->address = address << 1;
}

// Public functions
/**
 * While not part of the I2C spec, devices often work with registers that data
 * can be read from or written to.
 * In this case, the spec changes from <Slaveaddr Data> to <Slaveaddr reg data>
 *
 * @param reg
 * @return
 */
uint8_t I2c::readReg(uint8_t reg) {
    startCondition();
    writeByte(address | I2C_WRITE);
    writeByte(reg); // Which reg to read

    startCondition();
    writeByte(address | I2C_READ);

    uint8_t res = readByte(false); // The actual byte
    stopCondition(); // End transmission
    return res;
}

/**
 * While not part of the I2C spec, devices often work with registers that data
 * can be read from or written to.
 * In this case, the spec changes from <Slaveaddr Data> to <Slaveaddr reg data>
 * @param reg to write to
 * @param data to write
 * @return
 */
bool I2c::writeReg(uint8_t reg, uint8_t data) {
    startCondition();
    bool ack = writeByte(address | I2C_WRITE);
    ack &= writeByte(reg);
    ack &= writeByte(data);
    stopCondition();
    return ack;
}

///////////
////// Private helpers
////////////

/**
 * writeByte clocks all bits of a given byte to the device
 * MSB first
 * Returns whether or not the byte was ack'd
 * @warning You need to take care of the startCondition beforehand and stopCondition after
 *
 * @param byte to write to the I2C device
 * @return
 */
bool I2c::writeByte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        (byte & 0x80) ? dataHigh() : dataLow();
        clockHigh();
        clockLow();
        byte <<= 1;
    }

    // Check for ack
    clockHigh();
    bool res = getAck();
    clockLow();

    return res;
}

/**
 * Clocks a byte in from the device.
 * If ignoreAck is true, leave the line high
 * @warning You need to take care of the startCondition beforehand and stopCondition after
 * @param ack
 * @return
 */
uint8_t I2c::readByte(bool ack) {
    dataInput();
    uint8_t res = 0;
    for (int i = 0; i < 8; i++) {
        res <<= readBit();
        clockHigh();
        clockLow();
    }
    // Send ACK
    if (ack) {
        dataLow();
    }
    clockHigh();
    clockLow();

    dataHigh();
    return res;
}

/**
 * I2C Start condition, data line goes low when clock is high *
 */
void I2c::startCondition() {
    dataHigh();
    clockHigh(true);
    NOP;
    dataLow();
    clockLow();
}

/**
 * I2C Stop condition, clock goes high when data is low
 */
void I2c::stopCondition() {
    clockLow();
    dataLow();
    NOP;
    clockHigh(true);
    dataHigh();
}

/**
 * Actively pull line down
 * Cannot be done at once (requires change in both PORT and DDR register)
 * So go through high-impedance (input) mode.
 */
void I2c::clockLow() {
    PORTB &= ~(1<< I2C_CLK_PIN); // PORT = 0 (ddr=0) -> High-Impedance
    DDRB |= (1 << I2C_CLK_PIN); // DDR = 1 (port=0) -> Zero

    I2C_CLOCK_CYCLE_DELAY;
}

// NOT active high, but pull-up high
/**
 * Let line float high with pull-up resistor
 * Cannot be done at once
 * Go through Input mode
 *
 * Clock stretching is the fenomenon where the slave can actively hold the clock line down
 * if they are not ready to continue communication yet.
 * If this happens, the master should wait until they release the clock line to continue
 *
 * @param ignoreClockStretching
 */
void I2c::clockHigh(bool ignoreClockStretching) {
    DDRB &= ~(1 << I2C_CLK_PIN); // DDR = 0 (port=0) -> Input / High-Impedance
    PORTB |= (1 << I2C_CLK_PIN); // PORT = 1 (ddr=0) -> Pull-up

    if (useClockStretching && !ignoreClockStretching) {
        // Allow the slave to stretch this clock flank by holding the clock low.
        while (!(PORTB & (1 << I2C_CLK_PIN)));
    }

    I2C_CLOCK_CYCLE_DELAY;
}

// Active-low
void I2c::dataLow() {
    PORTB &= ~(1 << I2C_DATA_PIN); // Either pull-up or high-impedance.
    DDRB |= (1 << I2C_DATA_PIN); // Zero
    // When
}

// NOT active high, but pull-up
void I2c::dataHigh() {
    DDRB &= ~(1 << I2C_CLK_PIN); // DDR = 0 (port=0) -> Input / High-Impedance
    PORTB |= (1 << I2C_CLK_PIN); // PORT = 1 (ddr=0) -> Pull-up
}

// High-impedance mode
void I2c::dataInput() {
    DDRB &= ~(1 << I2C_DATA_PIN); // DDR is 0, PORT unknown -> tristate -> input or pul-up
    PORTB &= ~(1 << I2C_DATA_PIN); // PORTB = 0 -> input
}

bool I2c::readBit() {
    return (PORTB & I2C_DATA_PIN) != 0;
}
