#ifndef TINY13CLOCK_BMP280_H
#define TINY13CLOCK_BMP280_H


#include "I2c.h"

///////////
/**
 *
 * WARNING: DEVICE WORKS ONLY AT 3.3V
 * DONT BRICK IT
 *
 * Settings:
 * This impl only works with I2C, though the device also supports SPI (both 4 or 3-wire)
 * This means CSB must be held high continuously
 * The address is yours to choose, depending on SDO
 */
///////////

static const bool BMP280_SDO_PIN_HIGH = false;
 
class BMP280 {
public:
    enum class Status {
        Idle,
        Measuring,
        UpdatingImageRegisters,
    };
    BMP280();
    ~BMP280() = default;

    uint8_t getID();
    void reset();
    Status getStatus();
    bool setDataAqcuisition(uint8_t temperatureSampling, uint8_t pressureSampling, uint8_t powerMode);
    bool setConfig(uint8_t standbyTime, uint8_t IIRFilterConstant, bool enable3WireSPI);

    uint32_t getPressure();
    uint32_t getTemperature();

private:
    uint8_t const address = BMP280_SDO_PIN_HIGH ? 0x77 : 0x76;
    I2c i2c;
};


#endif //TINY13CLOCK_BMP280_H
