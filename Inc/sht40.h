/*
 * sht40.h
 *
 *  Created on: Aug 17, 2022
 *      Author: vit
 */

#ifndef SHT40_H_
#define SHT40_H_
#include <stdint.h>



class SHT40 {

	uint8_t (*sensirion_i2c_read)(uint8_t dev_id, uint8_t *data, uint16_t len);
	uint8_t (*sensirion_i2c_write)(uint8_t dev_id, uint8_t *data, uint16_t len);


/* all measurement commands return T (CRC) RH (CRC) */
static constexpr uint8_t SHT4X_CMD_MEASURE_HPM { 0xFD };
static constexpr uint8_t SHT4X_CMD_MEASURE_LPM { 0xE0 };
static constexpr uint8_t SHT4X_CMD_READ_SERIAL { 0x89 };
static constexpr uint16_t SHT4X_CMD_DURATION_USEC { 1000 };

static constexpr uint8_t SHT4X_ADDRESS { 0x44 };

 uint8_t sht4x_cmd_measure { 0xfd };
 uint16_t sht4x_cmd_measure_delay_us { SHT4X_MEASUREMENT_DURATION_USEC };

public:
	SHT40(uint8_t (*readFunctionPtr)(uint8_t, uint8_t*, uint16_t),
			uint8_t (*writeFunctionPtr)(uint8_t, uint8_t*, uint16_t));
	virtual ~SHT40();

	static constexpr int16_t STATUS_OK { 0 };
	static constexpr int16_t STATUS_ERR_BAD_DATA { -1 };
	static constexpr int16_t STATUS_CRC_FAIL { -2 };
	static constexpr int16_t STATUS_UNKNOWN_DEVICE  { -3 };
	static constexpr uint16_t SHT4X_MEASUREMENT_DURATION_USEC { 10000 };
	static constexpr uint16_t SHT4X_MEASUREMENT_DURATION_LPM_USEC { 2500 };


	int16_t ping(uint32_t &serial);
	int16_t startMeasureNRead(int32_t* temperature, int32_t* humidity);
	int16_t startMeasure(void);
	int16_t read(int32_t* temperature, int32_t* humidity);
	void enableLowPowerMode(uint8_t enable_low_power_mode);
	int16_t readSerialNumber(uint32_t* serial);

};

#endif /* SHT40_H_ */
