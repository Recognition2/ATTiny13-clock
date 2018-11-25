//
// Created by gregory on 19/11/18.
//

#include "I2c.h"
#include <util/delay.h>

#if defined(__arm__)
# define NOP __asm__ __volatile__ ("nop\n");
#else
#  define NOP __asm__ __volatile__ ("cp r0,r0\n");
#endif

#define NOP2 NOP NOP
#define NOP4 NOP2 NOP2

#define I2C_READ true
#define I2C_WRITE false

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
uint8_t I2c::readReg(uint8_t reg) {
    startCondition();
    writeByte(address | I2C_WRITE);
    writeByte(reg); // Which reg to read

    startCondition();
    writeByte(address | I2C_READ);

    uint8_t res = readByte(true); // The actual byte
    stopCondition(); // End transmission
    return res;
}

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

bool I2c::writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
        (b & 0x80) ? dataHigh() : dataLow();
        clockHigh();
        clockLow();
        b <<= 1;
    }

    // Check for ack
    clockHigh();
    bool res = getAck();
    clockLow();

    return res;
}

uint8_t I2c::readByte(bool ignoreAck) {
    dataInput();
    uint8_t res = 0;
    for (int i = 0; i < 8; i++) {
        res <<= readBit();
        clockHigh();
        clockLow();
    }
    // Send ACK
    ignoreAck ? dataHigh() : dataLow();
    clockHigh();
    clockLow();
    return res;
}

/* I2C Start condition, data line goes low when clock is high */
void I2c::startCondition() {
    dataHigh();
    clockHigh(true);
    dataLow();
    clockLow();
}

/* I2C Stop condition, clock goes high when data is low */
void I2c::stopCondition() {
    clockLow();
    dataLow();
    clockHigh();
    dataHigh();
}

void I2c::clockLow() {
    PORTB &= ~(1<< I2C_CLK_PIN); // PORT = 0 (ddr=0) -> High-Impedance
    DDRB |= (1 << I2C_CLK_PIN); // DDR = 1 (port=0) -> Zero

    NOP4;
}

// NOT active high, but pull-up high
void I2c::clockHigh(bool ignoreClockStretching) {
    DDRB &= ~(1 << I2C_CLK_PIN); // DDR = 0 (port=0) -> Input / High-Impedance
    PORTB |= (1 << I2C_CLK_PIN); // PORT = 1 (ddr=0) -> Pull-up

    if (useClockStretching && !ignoreClockStretching) {
        // Allow the slave to stretch this clock flank by holding the clock low.
        while (!(PORTB & (1 << I2C_CLK_PIN)));
    }

    NOP4;
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

// Active low
void I2c::dataInput() {
    DDRB &= ~(1 << I2C_DATA_PIN); // DDR is 0, PORT unknown -> tristate -> input or pul-up
    PORTB |= (1 << I2C_DATA_PIN);
}

bool I2c::readBit() {
    return (PORTB & I2C_DATA_PIN) != 0;
}
