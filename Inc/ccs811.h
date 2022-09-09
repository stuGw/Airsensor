/*
 * ccs811.h
 *
 *  Created on: Aug 18, 2022
 *      Author: vit
 */

#ifndef CCS811_H_
#define CCS811_H_
#include <stdint.h>

#ifdef CCS811_FLASH
#include "appbin_2_0_1.h"
#endif

// CCS811 I2C addresses
#define CCS811_I2C_ADDRESS_1      0x5A      // default
#define CCS811_I2C_ADDRESS_2      0x5B

// CCS811 clock streching counter
#define CCS811_I2C_CLOCK_STRETCH  200

// Definition of error codes
#define CCS811_OK                 0
#define CCS811_NOK                -1

#define CCS811_INT_ERROR_MASK     0x000f
#define CCS811_DRV_ERROR_MASK     0xfff0

// Error codes for the I2C interface ORed with CCS811 driver error codes
#define CCS811_I2C_READ_FAILED    1
#define CCS811_I2C_WRITE_FAILED   2
#define CCS811_I2C_BUSY           3

// CCS811 driver error codes ORed with error codes for I2C the interface
#define CCS811_DRV_BOOT_MODE      (1  << 8) // firmware is in boot mode
#define CCS811_DRV_NO_APP         (2  << 8) // no application firmware loaded
#define CCS811_DRV_NO_NEW_DATA    (3  << 8) // no new data samples are ready
#define CCS811_DRV_NO_IAQ_DATA    (4  << 8) // no new data samples are ready
#define CCS811_DRV_HW_ID          (5  << 8) // wrong hardware ID
#define CCS811_DRV_INV_SENS       (6  << 8) // invalid sensor ID
#define CCS811_DRV_WR_REG_INV     (7  << 8) // invalid register addr on write
#define CCS811_DRV_RD_REG_INV     (8  << 8) // invalid register addr on read
#define CCS811_DRV_MM_INV         (9  << 8) // invalid measurement mode
#define CCS811_DRV_MAX_RESIST     (10 << 8) // max sensor resistance reached
#define CCS811_DRV_HEAT_FAULT     (11 << 8) // heater current not in range
#define CCS811_DRV_HEAT_SUPPLY    (12 << 8) // heater voltage not correct
#define CCS811_DRV_WRONG_MODE     (13 << 8) // wrong measurement mode
#define CCS811_DRV_RD_STAT_FAILED (14 << 8) // read status register failed
#define CCS811_DRV_RD_DATA_FAILED (15 << 8) // read sensor data failed
#define CCS811_DRV_APP_START_FAIL (16 << 8) // sensor app start failure
#define CCS811_DRV_WRONG_PARAMS   (17 << 8) // wrong parameters used

// ranges
#define CCS_ECO2_RANGE_MIN 400
#define CCS_ECO2_RANGE_MAX 8192
#define CCS_TVOC_RANGE_MIN 0
#define CCS_TVOC_RANGE_MAX 1187

#define CCS811_ALG_DATA_ECO2_HB   0
#define CCS811_ALG_DATA_ECO2_LB   1
#define CCS811_ALG_DATA_TVOC_HB   2
#define CCS811_ALG_DATA_TVOC_LB   3
#define CCS811_ALG_DATA_STATUS    4
#define CCS811_ALG_DATA_ERROR_ID  5
#define CCS811_ALG_DATA_RAW_HB    6
#define CCS811_ALG_DATA_RAW_LB    7

/* CCS811 register addresses */
#define CCS811_REG_STATUS          0x00
#define CCS811_REG_MEAS_MODE       0x01
#define CCS811_REG_ALG_RESULT_DATA 0x02
#define CCS811_REG_RAW_DATA        0x03
#define CCS811_REG_ENV_DATA        0x05
#define CCS811_REG_NTC             0x06
#define CCS811_REG_THRESHOLDS      0x10
#define CCS811_REG_BASELINE        0x11

#define CCS811_REG_HW_ID           0x20
#define CCS811_REG_HW_VER          0x21
#define CCS811_REG_FW_BOOT_VER     0x23
#define CCS811_REG_FW_APP_VER      0x24

#define CCS811_REG_ERROR_ID        0xe0

#define CCS811_REG_APP_ERASE       0xf1
#define CCS811_REG_APP_DATA        0xf2
#define CCS811_REG_APP_VERIFY      0xf3
#define CCS811_REG_APP_START       0xf4
#define CCS811_REG_SW_RESET        0xff

// status register bits
#define CCS811_STATUS_ERROR        0x01  // error, details in CCS811_REG_ERROR
#define CCS811_STATUS_DATA_RDY     0x08  // new data sample in ALG_RESULT_DATA
#define CCS811_STATUS_APP_VALID    0x10  // valid application firmware loaded
#define CCS811_STATUS_FW_MODE      0x80  // firmware is in application mode

// error register bits
#define CCS811_ERR_WRITE_REG_INV   0x01  // invalid register address on write
#define CCS811_ERR_READ_REG_INV    0x02  // invalid register address on read
#define CCS811_ERR_MEASMODE_INV    0x04  // invalid requested measurement mode
#define CCS811_ERR_MAX_RESISTANCE  0x08  // maximum sensor resistance exceeded
#define CCS811_ERR_HEATER_FAULT    0x10  // heater current not in range
#define CCS811_ERR_HEATER_SUPPLY   0x20  // heater voltage not applied correctly


typedef enum {
    ccs811_mode_idle  = 0, // Idle, low current mode
    ccs811_mode_1s    = 1, // Constant Power mode, IAQ values every 1 s
    ccs811_mode_10s   = 2, // Pulse Heating mode, IAQ values every 10 s
    ccs811_mode_60s   = 3, // Low Power Pulse Heating, IAQ values every 60 s
    ccs811_mode_250ms = 4  // Constant Power mode, RAW data every 250 ms
} ccs811_mode_t;

typedef struct
{
    uint8_t reserved_1 :2;
    uint8_t int_thresh :1;  // interrupt if new ALG_RESULT_DAT crosses on of the thresholds
    uint8_t int_datardy:1;  // interrupt if new sample is ready in ALG_RESULT_DAT
    uint8_t drive_mode :3;  // mode number binary coded
} ccs811_meas_mode_reg_t;

typedef struct {

    int           error_code;  // contains the error code of last operation

    uint8_t       bus;         // I2C bus
    uint8_t       addr;        // I2C slave address

    ccs811_mode_t mode;        // operation mode

} ccs811_sensor_t;



class CCS811 {

	int8_t (*i2c_slave_read)(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len);
	int8_t (*i2c_slave_write)(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len);
	volatile uint32_t* ioPort { nullptr };
	uint32_t wakeMask { 0 };
public:
	CCS811();
	virtual ~CCS811();

	int  error_code;  // contains the error code of last operation

	uint8_t  bus;         // I2C bus
	uint8_t  addr;        // I2C slave address

	ccs811_mode_t mode;        // operation mode

	void init(uint8_t bus, uint8_t addr);

	bool setMode (ccs811_mode_t mode);

	#define CCS811_ALG_DATA_ECO2_HB   0
	#define CCS811_ALG_DATA_ECO2_LB   1
	#define CCS811_ALG_DATA_TVOC_HB   2
	#define CCS811_ALG_DATA_TVOC_LB   3
	#define CCS811_ALG_DATA_STATUS    4
	#define CCS811_ALG_DATA_ERROR_ID  5
	#define CCS811_ALG_DATA_RAW_HB    6
	#define CCS811_ALG_DATA_RAW_LB    7

	bool getResults(
	                         uint16_t* iaq_tvoc,
	                         uint16_t* iaq_eco2,
	                         uint8_t*  raw_i,
	                         uint16_t* raw_v);

	void setInterface(int8_t (*i2cfr)(uint8_t deviceAddress, uint8_t memAddress, uint8_t* data, uint16_t count),
			int8_t (*i2cfw)( uint8_t deviceAddress, uint8_t memAddress, uint8_t* data, uint16_t count))
	{
		i2c_slave_read = i2cfr;
		i2c_slave_write = i2cfw;
	}

	void setWakePin(volatile uint32_t* port, uint32_t mask)
	{
		ioPort = port;
		wakeMask = mask;
	}

	uint32_t getNTCValue(uint32_t r_ref);



	bool setEnviromentalData (
	                                    float temperature,
	                                    float humidity);

	void setAddress(uint8_t address){ addr = address;}
	bool setEcoThresholds (uint16_t low, uint16_t high, uint8_t hysteresis);
	bool enableInterrupts ( bool enabled);
	uint16_t getBaseline ();
	bool setBaseline (uint16_t baseline);
    bool enableThreshold( bool enabled);
	bool registerRead( uint8_t reg, uint8_t *data, uint32_t len);
	bool registerWrite( uint8_t reg, uint8_t *data, uint32_t len);
	bool checkErrorStatus ();
	bool isAvailable ();
	bool flash(int size);
	void wakeUp(){ *ioPort &= ~wakeMask; }
	void wakeDown(){ *ioPort |= wakeMask; }
	bool checkFirmware();
	bool getStatus(uint8_t *stat);
};

#endif /* CCS811_H_ */
