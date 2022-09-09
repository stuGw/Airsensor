/*
 * sht40.cpp
 *
 *  Created on: Aug 17, 2022
 *      Author: vit
 */

#include "sht40.h"
#include "system.h"
#include "serial.h"
SHT40::SHT40(uint8_t (*readFunctionPtr)(uint8_t, uint8_t*, uint16_t),uint8_t (*writeFunctionPtr)(uint8_t, uint8_t*, uint16_t)) {
	// TODO Auto-generated constructor stub
	sensirion_i2c_read = readFunctionPtr;
	sensirion_i2c_write = writeFunctionPtr;
}

SHT40::~SHT40() {
	// TODO Auto-generated destructor stub
}

int16_t SHT40::startMeasure(void) {
    return sensirion_i2c_write(SHT4X_ADDRESS, &sht4x_cmd_measure, 1);
}



int16_t SHT40::startMeasureNRead(int32_t* temperature, int32_t* humidity) {
    int16_t ret;

    ret = startMeasure();
    if (ret)
        return ret;
    delay_us(sht4x_cmd_measure_delay_us);
    return read(temperature, humidity);
}


int16_t SHT40::read(int32_t* temperature, int32_t* humidity) {
    uint8_t words[6];
    int16_t ret = sensirion_i2c_read(SHT4X_ADDRESS, words,
                                           6);
    /**
     * formulas for conversion of the sensor signals, optimized for fixed point
     * algebra:
     * Temperature = 175 * S_T / 65535 - 45
     * Relative Humidity = 125 * (S_RH / 65535) - 6
     */
    int32_t tticks = words[0]*256 + words[1];
    int32_t rhticks = words[3]*256 + words[4];
    *temperature =  ((21875 * (int32_t)tticks) >> 13) - 45000;//-45.0 + 175.0* (float)tticks/65535;//((21875 * (int32_t)words[0]) >> 13) - 45000;
    *humidity = ((15625 * (int32_t)rhticks) >> 13) - 6000;

    return ret;
}

int16_t SHT40::ping(uint32_t &serial) {

    return readSerialNumber(&serial);
}

void SHT40::enableLowPowerMode(uint8_t enable_low_power_mode) {
    if (enable_low_power_mode) {
        sht4x_cmd_measure = SHT4X_CMD_MEASURE_LPM;
        sht4x_cmd_measure_delay_us = SHT4X_MEASUREMENT_DURATION_LPM_USEC;
    } else {
        sht4x_cmd_measure = SHT4X_CMD_MEASURE_HPM;
        sht4x_cmd_measure_delay_us = SHT4X_MEASUREMENT_DURATION_USEC;
    }
}

int16_t SHT40::readSerialNumber(uint32_t* serial) {
    uint8_t cmd = SHT4X_CMD_READ_SERIAL;
    int16_t ret;
    uint8_t serial_words[6];

    ret = sensirion_i2c_write(SHT4X_ADDRESS, &cmd, 1);
    if (ret)
        return ret;

    delay_us(SHT4X_CMD_DURATION_USEC);
    ret = sensirion_i2c_read(SHT4X_ADDRESS, (uint8_t*)serial_words,
                                   6);
    *serial = ((uint32_t)serial_words[0] << 24) | ((uint32_t)serial_words[1]<<16) | ((uint32_t)serial_words[3]) << 8 | ((uint32_t)serial_words[4]);

    return ret;
}



