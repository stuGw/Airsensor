/*
 * sgp30.cpp
 *
 *  Created on: Aug 31, 2022
 *      Author: vit
 */

#include <sgp30.h>

SGP30::SGP30() {
	// TODO Auto-generated constructor stub

}

SGP30::~SGP30() {
	// TODO Auto-generated destructor stub
}


#define SGP30_PRODUCT_TYPE 0
static const uint8_t SGP30_I2C_ADDRESS = 0x58<<1;

/* command and constants for reading the serial ID */
#define SGP30_CMD_GET_SERIAL_ID 0x3682
#define SGP30_CMD_GET_SERIAL_ID_DURATION_US 500
#define SGP30_CMD_GET_SERIAL_ID_WORDS 3

/* command and constants for reading the featureset version */
#define SGP30_CMD_GET_FEATURESET 0x202f
#define SGP30_CMD_GET_FEATURESET_DURATION_US 10000
#define SGP30_CMD_GET_FEATURESET_WORDS 1

/* command and constants for on-chip self-test */
#define SGP30_CMD_MEASURE_TEST 0x2032
#define SGP30_CMD_MEASURE_TEST_DURATION_US 220000
#define SGP30_CMD_MEASURE_TEST_WORDS 1
#define SGP30_CMD_MEASURE_TEST_OK 0xd400

/* command and constants for IAQ init */
#define SGP30_CMD_IAQ_INIT 0x2003
#define SGP30_CMD_IAQ_INIT_DURATION_US 10000

/* command and constants for IAQ measure */
#define SGP30_CMD_IAQ_MEASURE 0x2008
#define SGP30_CMD_IAQ_MEASURE_DURATION_US 12000
#define SGP30_CMD_IAQ_MEASURE_WORDS 2

/* command and constants for getting IAQ baseline */
#define SGP30_CMD_GET_IAQ_BASELINE 0x2015
#define SGP30_CMD_GET_IAQ_BASELINE_DURATION_US 10000
#define SGP30_CMD_GET_IAQ_BASELINE_WORDS 2

/* command and constants for setting IAQ baseline */
#define SGP30_CMD_SET_IAQ_BASELINE 0x201e
#define SGP30_CMD_SET_IAQ_BASELINE_DURATION_US 10000

/* command and constants for raw measure */
#define SGP30_CMD_RAW_MEASURE 0x2050
#define SGP30_CMD_RAW_MEASURE_DURATION_US 25000
#define SGP30_CMD_RAW_MEASURE_WORDS 2

/* command and constants for setting absolute humidity */
#define SGP30_CMD_SET_ABSOLUTE_HUMIDITY 0x2061
#define SGP30_CMD_SET_ABSOLUTE_HUMIDITY_DURATION_US 10000

/* command and constants for getting TVOC inceptive baseline */
#define SGP30_CMD_GET_TVOC_INCEPTIVE_BASELINE 0x20b3
#define SGP30_CMD_GET_TVOC_INCEPTIVE_BASELINE_DURATION_US 10000
#define SGP30_CMD_GET_TVOC_INCEPTIVE_BASELINE_WORDS 1

/* command and constants for setting TVOC baseline */
#define SGP30_CMD_SET_TVOC_BASELINE 0x2077
#define SGP30_CMD_SET_TVOC_BASELINE_DURATION_US 10000

/**
 * sgp30_check_featureset() - Check if the connected sensor has a certain FS
 *
 * @needed_fs: The featureset that is required
 *
 * Return: STATUS_OK if the sensor has the required FS,
 *         SGP30_ERR_INVALID_PRODUCT_TYPE if the sensor is not an SGP30,
 *         SGP30_ERR_UNSUPPORTED_FEATURE_SET if the sensor does not
 *                                           have the required FS,
 *         an error code otherwise
 */


int16_t SGP30::measureTest(uint16_t* test_result) {
   /* uint8_t measure_test_word_buf[SGP30_CMD_MEASURE_TEST_WORDS];
    int16_t ret;

    *test_result = 0;

    ret = readI2C(0,SGP30_I2C_ADDRESS, SGP30_CMD_MEASURE_TEST, measure_test_word_buf,SENSIRION_NUM_WORDS(measure_test_word_buf));
    if (ret != STATUS_OK)
        return ret;

    *test_result = *measure_test_word_buf;
    if (*test_result == SGP30_CMD_MEASURE_TEST_OK)
        return STATUS_OK;

    return STATUS_FAIL;*/
}

int16_t SGP30::measureIAQ() {
    return writeI2C(0,SGP30_I2C_ADDRESS, SGP30_CMD_IAQ_MEASURE,0,0,true);
}

int16_t SGP30::readIAQ(uint16_t* tvoc_ppb, uint16_t* co2_eq_ppm) {
    int16_t ret;
    uint16_t words[SGP30_CMD_IAQ_MEASURE_WORDS];

    ret = readI2CS(0,SGP30_I2C_ADDRESS, 4, (uint8_t*)words);

    *tvoc_ppb = words[1];
    *co2_eq_ppm = words[0];

    return ret;
}

int16_t  SGP30::measureIAQBlockingRead(uint16_t* tvoc_ppb,
                                        uint16_t* co2_eq_ppm) {
    int16_t ret;

    ret = measureIAQ();
    if (ret != STATUS_OK)
        return ret;

   // sensirion_sleep_usec(SGP30_CMD_IAQ_MEASURE_DURATION_US);

    return readIAQ(tvoc_ppb, co2_eq_ppm);
}

int16_t SGP30::measureTVOC() {
    return measureIAQ();
}

int16_t SGP30::readTVOC(uint16_t* tvoc_ppb) {
    uint16_t co2_eq_ppm;
    return readIAQ(tvoc_ppb, &co2_eq_ppm);
}

int16_t SGP30::measureTVOCBlockingRead(uint16_t* tvoc_ppb) {
    uint16_t co2_eq_ppm;
    return measureIAQBlockingRead(tvoc_ppb, &co2_eq_ppm);
}

int16_t SGP30::measureCO2Eq() {
    return measureIAQ();
}

int16_t SGP30::readCO2Eq(uint16_t* co2_eq_ppm) {
    uint16_t tvoc_ppb;
    return readIAQ(&tvoc_ppb, co2_eq_ppm);
}

int16_t SGP30::measureCOEqBlockingRead(uint16_t* co2_eq_ppm) {
    uint16_t tvoc_ppb;
    return measureIAQBlockingRead(&tvoc_ppb, co2_eq_ppm);
}

int16_t SGP30::measureRawBlockingRead(uint16_t* ethanol_raw_signal,
                                        uint16_t* h2_raw_signal) {
    int16_t ret;

    ret = measureRaw();
    if (ret != STATUS_OK)
        return ret;

  //  sensirion_sleep_usec(SGP30_CMD_RAW_MEASURE_DURATION_US);

    return readRaw(ethanol_raw_signal, h2_raw_signal);
}

int16_t SGP30::measureRaw() {
    return writeI2C(0,SGP30_I2C_ADDRESS, SGP30_CMD_RAW_MEASURE,0,0,true);
}

int16_t SGP30::readRaw(uint16_t* ethanol_raw_signal, uint16_t* h2_raw_signal) {
    int16_t ret;
    uint16_t words[SGP30_CMD_RAW_MEASURE_WORDS];

    ret = readI2CS(0,SGP30_I2C_ADDRESS, 4, (uint8_t*)words);

    *ethanol_raw_signal = words[1];
    *h2_raw_signal = words[0];

    return ret;
}

int16_t SGP30::getIAQBaseline(uint32_t* baseline) {
    int16_t ret;
    uint16_t words[SGP30_CMD_GET_IAQ_BASELINE_WORDS];

    ret =
        writeI2C(0,SGP30_I2C_ADDRESS, SGP30_CMD_GET_IAQ_BASELINE,0,0,true);

    if (ret != STATUS_OK)
        return ret;

   //DELAY
   // sensirion_sleep_usec(SGP30_CMD_GET_IAQ_BASELINE_DURATION_US);

    ret = readI2CS(0,SGP30_I2C_ADDRESS, 4, (uint8_t*)words);

    if (ret != STATUS_OK)
        return ret;

    *baseline = ((uint32_t)words[1] << 16) | ((uint32_t)words[0]);

    if (*baseline)
        return STATUS_OK;
    return STATUS_FAIL;
}

int16_t SGP30::setIAQBaseline(uint32_t baseline) {
    int16_t ret;
    uint16_t words[2] = {(uint16_t)((baseline & 0xffff0000) >> 16),
                         (uint16_t)(baseline & 0x0000ffff)};

    if (!baseline)
        return STATUS_FAIL;

    ret = writeI2C(0,SGP30_I2C_ADDRESS, SGP30_CMD_SET_IAQ_BASELINE, 4, (uint8_t*)words, true);

    //sensirion_sleep_usec(SGP30_CMD_SET_IAQ_BASELINE_DURATION_US);

    return ret;
}
int16_t SGP30::checkFeatureSet(uint16_t needed_fs) {
    int16_t ret;
    uint16_t fs_version;
    uint8_t product_type;

    ret = getFeatureSetVersion(&fs_version, &product_type);
    if (ret != STATUS_OK)
        return ret;

    if (product_type != SGP30_PRODUCT_TYPE)
        return -1;// SGP30_ERR_INVALID_PRODUCT_TYPE;

    if (fs_version < needed_fs)
        return  -2;//SGP30_ERR_UNSUPPORTED_FEATURE_SET;

    return STATUS_OK;
}

int16_t SGP30::getTVOCInceptiveBaseline(uint16_t* tvoc_inceptive_baseline) {
    int16_t ret;

    ret = checkFeatureSet(0x21);

    if (ret != STATUS_OK)
        return ret;

    ret = writeI2C(0,SGP30_I2C_ADDRESS,SGP30_CMD_GET_TVOC_INCEPTIVE_BASELINE,0,0,true);

    if (ret != STATUS_OK)
        return ret;
//DELAY
   // sensirion_sleep_usec(SGP30_CMD_GET_TVOC_INCEPTIVE_BASELINE_DURATION_US);

    return readI2CS(0,SGP30_I2C_ADDRESS, 2,(uint8_t*)tvoc_inceptive_baseline);
}

int16_t SGP30::setTVOCBaseline(uint16_t tvoc_baseline) {
    int16_t ret;

    ret = checkFeatureSet(0x21);

    if (ret != STATUS_OK)
        return ret;

    if (!tvoc_baseline)
        return STATUS_FAIL;

    ret = writeI2C(0,SGP30_I2C_ADDRESS, SGP30_CMD_SET_TVOC_BASELINE, 2, (uint8_t*)&tvoc_baseline,true);
//DELAY
    //sensirion_sleep_usec(SGP30_CMD_SET_TVOC_BASELINE_DURATION_US);

    return ret;
}

int16_t SGP30::setAbsoluteHumidity(uint32_t absolute_humidity) {
    int16_t ret;
    uint16_t ah_scaled;

    if (absolute_humidity > 256000)
        return STATUS_FAIL;

    /* ah_scaled = (absolute_humidity / 1000) * 256 */
    ah_scaled = (uint16_t)((absolute_humidity * 16777) >> 16);

    ret = writeI2C(0,SGP30_I2C_ADDRESS, SGP30_CMD_SET_ABSOLUTE_HUMIDITY, 2, (uint8_t*)&ah_scaled,true);

    //sensirion_sleep_usec(SGP30_CMD_SET_ABSOLUTE_HUMIDITY_DURATION_US);

    return ret;
}


int16_t SGP30::getFeatureSetVersion(uint16_t* feature_set_version,
                                      uint8_t* product_type) {
    int16_t ret;
    uint16_t words[SGP30_CMD_GET_FEATURESET_WORDS];

    ret = readI2C(0,SGP30_I2C_ADDRESS,SGP30_CMD_GET_FEATURESET, 2, (uint8_t*)words,true);

    if (ret != STATUS_OK)
        return ret;

    *feature_set_version = words[0] & 0x00FF;
    *product_type = (uint8_t)((words[0] & 0xF000) >> 12);

    return STATUS_OK;
}

int16_t SGP30::getID(uint64_t* serial_id) {
    int16_t ret;
    uint16_t words[SGP30_CMD_GET_SERIAL_ID_WORDS];

    ret = readI2C(0,SGP30_I2C_ADDRESS,SGP30_CMD_GET_SERIAL_ID, 6, (uint8_t*)words,true);

    if (ret != STATUS_OK)
        return ret;

    *serial_id = (((uint64_t)words[0]) << 32) | (((uint64_t)words[1]) << 16) |
                 (((uint64_t)words[2]) << 0);

    return STATUS_OK;
}

int16_t SGP30::IAQInit() {
    int16_t ret =
        writeI2C(0,SGP30_I2C_ADDRESS, SGP30_CMD_IAQ_INIT,0,0,true);
  //  sensirion_sleep_usec(SGP30_CMD_IAQ_INIT_DURATION_US);
    return ret;
}

int16_t SGP30::probe() {

    return IAQInit();
}
