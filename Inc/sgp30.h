/*
 * sgp30.h
 *
 *  Created on: Aug 31, 2022
 *      Author: vit
 */

#ifndef SGP30_H_
#define SGP30_H_

#include <stdint.h>


class SGP30 {
	int8_t (*writeI2C)(uint8_t addrMode, uint16_t deviceAddress, uint16_t memAddress, uint8_t count, uint8_t* data, bool twoByteAddress);
	int8_t (*readI2C)(uint8_t addrMode, uint16_t deviceAddress, uint16_t memAddress, uint8_t count, uint8_t* data, bool twoByteAddress);
	//read count bytes from device(deviceAddress) position addrFom
	int8_t (*readI2CS)(uint8_t addrMode, uint16_t deviceAddress, uint8_t count, uint8_t* data);
public:


#define NO_ERROR 0
/* deprecated defines, use NO_ERROR or custom error codes instead */
static constexpr int8_t  STATUS_OK { 0 };
static constexpr int8_t   STATUS_FAIL { -1 };



//#define CRC8_POLYNOMIAL 0x31
//#define CRC8_INIT 0xFF
//#define CRC8_LEN 1

//#define SENSIRION_COMMAND_SIZE 2
//#define SENSIRION_WORD_SIZE 2
//#define SENSIRION_NUM_WORDS(x) (sizeof(x) / SENSIRION_WORD_SIZE)
//#define SENSIRION_MAX_BUFFER_WORDS 32


	SGP30();
	virtual ~SGP30();

	void setInterface(int8_t (*i2cfr)(uint8_t addrMode, uint16_t deviceAddress, uint16_t memAddress, uint8_t count, uint8_t* data, bool twoByteAddress),
				int8_t (*i2cfw)(uint8_t addrMode, uint16_t deviceAddress, uint16_t memAddress, uint8_t count, uint8_t* data, bool twoByteAddress),
			    int8_t (*i2cfrs)(uint8_t addrMode, uint16_t deviceAddress,uint8_t count, uint8_t* data))
		{
			writeI2C = i2cfw;
			readI2C = i2cfr;
			readI2CS = i2cfrs;
		}

	int16_t measureTest(uint16_t* test_result);

	int16_t measureIAQ();

	int16_t readIAQ(uint16_t* tvoc_ppb, uint16_t* co2_eq_ppm);

	int16_t  measureIAQBlockingRead(uint16_t* tvoc_ppb,uint16_t* co2_eq_ppm);

	int16_t measureTVOC();

	int16_t readTVOC(uint16_t* tvoc_ppb);
	int16_t measureTVOCBlockingRead(uint16_t* tvoc_ppb);
	int16_t measureCO2Eq();
	int16_t readCO2Eq(uint16_t* co2_eq_ppm);
	int16_t measureCOEqBlockingRead(uint16_t* co2_eq_ppm);
	int16_t measureRawBlockingRead(uint16_t* ethanol_raw_signal,uint16_t* h2_raw_signal);
	int16_t measureRaw();

	int16_t readRaw(uint16_t* ethanol_raw_signal, uint16_t* h2_raw_signal);
	int16_t getIAQBaseline(uint32_t* baseline);
	int16_t setIAQBaseline(uint32_t baseline);
	int16_t getTVOCInceptiveBaseline(uint16_t* tvoc_inceptive_baseline);
	int16_t setTVOCBaseline(uint16_t tvoc_baseline);
	int16_t setAbsoluteHumidity(uint32_t absolute_humidity);
	int16_t getFeatureSetVersion(uint16_t* feature_set_version,uint8_t* product_type);
	int16_t getID(uint64_t* serial_id);
	int16_t IAQInit();
	int16_t checkFeatureSet(uint16_t needed_fs);
	int16_t probe();

};

#endif /* SGP30_H_ */
