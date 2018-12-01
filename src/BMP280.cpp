#include "BMP280.h"

BMP280::BMP280(): i2c(this->address) {
}

/**
 * The ID can be read out as register 0xD0
 *
 * @return ID of the device, should be 0x5B
 */
uint8_t BMP280::getID() {
    i2c.startCondition();
    i2c.writeAddress(i2c.Write);
    i2c.writeByte(0xD0); // ID register

    i2c.startCondition();
    i2c.writeAddress(i2c.Read);
    uint8_t id = i2c.readByte(false);

    i2c.stopCondition();
    return id;
}

void BMP280::reset() {
    i2c.startCondition();
    i2c.writeAddress(i2c.Write);
    i2c.writeByte(0xE0); // Reset register
    i2c.writeByte(0xB6); // Reset value

    i2c.stopCondition();
}

BMP280::Status BMP280::getStatus() {
    i2c.startCondition();
    i2c.writeAddress(i2c.Write);
    i2c.writeByte(0xF3); // Status reg

    i2c.startCondition();
    i2c.writeAddress(i2c.Read);
    uint8_t status = i2c.readByte(false); // get status
    i2c.stopCondition();

    if (status & 0x1) {
        return Status::UpdatingImageRegisters;
    } else if (status & 0xF) {
        return Status::Measuring;
    }
    return Status::Idle;
}

//bool BMP280::setConfig(uint8_t standbyTime, uint8_t IIRFilterConstant, bool enable3WireSPI) {
//
//}

//uint32_t BMP280::getPressure() {
//
//}