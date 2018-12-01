//
// Created by gregory on 01/12/18.
//

#include "DS1307.h"

DS1307::DS1307(): i2c(address) {
}

uint8_t DS1307::getSeconds() {
    const uint8_t bcdSeconds = i2c.readReg(secondsRegister);
    return bcdToDecimal(bcdSeconds);
}

uint8_t DS1307::getMinutes() {
    const uint8_t bcdminutes = i2c.readReg(minutesRegister);
    return bcdToDecimal(bcdminutes);
}

uint8_t DS1307::getHours() {
    const uint8_t bcdhours = i2c.readReg(hoursRegister);

    const bool isIn24HrMode = (bcdhours & 0x40) == 0;
    if (isIn24HrMode) {
        return bcdToDecimal(bcdhours & 0x3F); // Ignore highest two bits,
    } else {
        const uint8_t convertedHours = bcdToDecimal(bcdhours & 0x1F);
        return convertedHours + (uint8_t) ((bcdhours & (1<<5)) ? 12 : 0);
    }
}


uint8_t DS1307::bcdToDecimal(uint8_t bcd) {
    uint8_t lowerDigit= bcd & 0xF0;
    uint8_t upperDigit = bcd & 0x0F;
    return (uint8_t) 10*upperDigit + lowerDigit;
}