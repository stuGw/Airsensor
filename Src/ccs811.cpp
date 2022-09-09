/*
 * ccs811.cpp
 *
 *  Created on: Aug 18, 2022
 *      Author: vit
 */

#include <ccs811.h>
#include <stdlib.h>
#include <cstring>
#include "logger.h"
#include "system.h"

// Timings
#define CCS811_WAIT_AFTER_RESET_US     2000
#define CCS811_WAIT_AFTER_APPSTART_US  1000
#define CCS811_WAIT_AFTER_WAKE_US        50
#define CCS811_WAIT_AFTER_APPERASE_MS   500
#define CCS811_WAIT_AFTER_APPVERIFY_MS   70
#define CCS811_WAIT_AFTER_APPDATA_MS     50


CCS811::CCS811() {
	// TODO Auto-generated constructor stub

}

CCS811::~CCS811() {
	// TODO Auto-generated destructor stub
}

bool CCS811::checkFirmware()
{
	 wakeUp();

	 uint8_t sw_reset[]= {0x11,0xE5,0x72,0x8A};
	   uint8_t app_start[]= {};
	   uint8_t hw_id;
	   uint8_t hw_version;
	   uint8_t app_version[2];
	   uint8_t status;
	   bool ok;

	    // Invoke a SW reset (bring CCS811 in a know state)
	    ok= registerWrite(CCS811_REG_SW_RESET,sw_reset,4);
	    if( !ok ) {
	      LOG->DEBG("ccs811: reset failed");
	      goto abort_begin;
	    }
	    delay_us(CCS811_WAIT_AFTER_RESET_US);
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
LOG->DEBG("Delay reset");
	    // Check that HW_ID is 0x81
	    ok= registerRead(CCS811_REG_HW_ID,&hw_id,1);
	    if( !ok ) {
	    	LOG->DEBG("ccs811: HW_ID read failed");
	      goto abort_begin;
	    }
	    if( hw_id!=0x81 ) {
	    	LOG->DEBGH("ccs811: Wrong HW_ID: ",hw_id);

	    	LOG->DEBGH("Hw id = ",hw_id);
	     // goto abort_begin;
	    }

	    // Check that HW_VERSION is 0x1X
	    ok= registerRead(CCS811_REG_HW_VER,&hw_version,1);
	    if( !ok ) {
	    	LOG->DEBG("ccs811: HW_VERSION read failed");
	     // goto abort_begin;
	    }
	    if( (hw_version&0xF0)!=0x10 ) {
	    	LOG->DEBGH("ccs811: Wrong HW_VERSION: ",hw_version);

	      //goto abort_begin;
	    }
	    LOG->DEBGH("Hw iver = ",hw_version);

	    // Check status (after reset, CCS811 should be in boot mode with valid app)
	    ok= registerRead(CCS811_REG_STATUS,&status,1);
	    if( !ok ) {
	    	LOG->DEBG("ccs811: STATUS read (boot mode) failed");
	     // goto abort_begin;
	    }
	    if( status!=0x10 ) {
	    	LOG->DEBGH("ccs811: Not in boot mode, or no valid app: ",status);

	    //  goto abort_begin;
	    }
	    LOG->DEBGH("statup= ",status);
	    // Read the application version
	    ok= registerRead(CCS811_REG_FW_APP_VER,app_version,2);
	    if( !ok ) {
	    	LOG->DEBG("ccs811: APP_VERSION read failed");
	   //   goto abort_begin;
	    }
	//    _appversion= app_version[0]*256+app_version[1];

	    // Switch CCS811 from boot mode into app mode
	    ok= registerWrite(CCS811_REG_APP_START,app_start,0);
	    if( !ok ) {
	    	LOG->DEBG("ccs811: Goto app mode failed");
	     // goto abort_begin;
	    }


	    LOG->DEBG("Delay app");
	    LOG->DEBG("Delay app");
	    LOG->DEBG("Delay app");
	    LOG->DEBG("Delay app");
	    LOG->DEBG("Delay app");
	    LOG->DEBG("Delay app");
	    LOG->DEBG("Delay app");
	    LOG->DEBG("Delay app");
	    LOG->DEBG("Delay app");
	    LOG->DEBG("Delay app");
	    LOG->DEBG("Delay app");
	    	    LOG->DEBG("Delay app");
	    	    LOG->DEBG("Delay app");
	    	    LOG->DEBG("Delay app");
	    	    LOG->DEBG("Delay app");
	    	    LOG->DEBG("Delay app");
	    	    LOG->DEBG("Delay app");
	    	    LOG->DEBG("Delay app");
	    	    LOG->DEBG("Delay app");
	    	    LOG->DEBG("Delay app");
	    	    LOG->DEBG("Delay app");
	    	    	    LOG->DEBG("Delay app");
	    	    	    LOG->DEBG("Delay app");
	    	    	    LOG->DEBG("Delay app");
	    	    	    LOG->DEBG("Delay app");
	    	    	    LOG->DEBG("Delay app");
	    	    	    LOG->DEBG("Delay app");
	    	    	    LOG->DEBG("Delay app");
	    	    	    LOG->DEBG("Delay app");
	    	    	    LOG->DEBG("Delay app");
	    	    	    LOG->DEBG("Delay app");
	    	    	    	    LOG->DEBG("Delay app");
	    	    	    	    LOG->DEBG("Delay app");
	    	    	    	    LOG->DEBG("Delay app");
	    	    	    	    LOG->DEBG("Delay app");
	    	    	    	    LOG->DEBG("Delay app");
	    	    	    	    LOG->DEBG("Delay app");
	    	    	    	    LOG->DEBG("Delay app");
	    	    	    	    LOG->DEBG("Delay app");
	    	    	    	    LOG->DEBG("Delay app");
	    delay_us(CCS811_WAIT_AFTER_APPSTART_US);

	    // Check if the switch was successful
	    ok= registerRead(CCS811_REG_STATUS,&status,1);
	    if( !ok ) {
	    	LOG->DEBG("ccs811: STATUS read (app mode) failed");
	  //    goto abort_begin;
	    }
	    if( status!=0x90 ) {
	    	LOG->DEBG("ccs811: Not in app mode, or no valid app: ");

	   //   goto abort_begin;
	    }

	  // CCS811 back to sleep
//	  wakeDown();
	  // Return success
	  return true;

	abort_begin:
	  // CCS811 back to sleep
	  wakeDown();
}

// Flashes the firmware of the CCS811 with size bytes from image - image _must_ be in PROGMEM.
bool CCS811::flash( int size) {
  uint8_t sw_reset[]=   {0x11,0xE5,0x72,0x8A};
  uint8_t app_erase[]=  {0xE7,0xA7,0xE6,0x09};
  uint8_t app_verify[]= {};
  uint8_t status;

#ifdef CCS811_FLASH
  const uint8_t* image = image_data;


  volatile int grt = 0;
  int count;
  bool ok;
  wakeUp();

    // Try to ping CCS811 (can we reach CCS811 via I2C?)
    LOG->DEBG("ccs811: ping ");
   // ok= registerWrite(0,0,0);
   // if( !ok ) {
   // 	LOG->DEBG("FAILED");
   //   goto abort_begin;
    //}
    //LOG->DEBG("ok");

    // Invoke a SW reset (bring CCS811 in a know state)
    		LOG->DEBG("ccs811: reset ");
    ok= registerWrite(CCS811_REG_SW_RESET,sw_reset,4);
    if( !ok ) {
    	LOG->DEBG("FAILED");
      goto abort_begin;
    }
    delay_us(CCS811_WAIT_AFTER_RESET_US);
    LOG->DEBG("ok");

    // Check status (after reset, CCS811 should be in boot mode with or without valid app)
    LOG->DEBG("ccs811: status (reset1) ");
    ok= registerRead(CCS811_REG_STATUS,&status,1);
    if( !ok ) {
    	LOG->DEBG("FAILED");
      goto abort_begin;
    }

    if( status!=0x00 && status!=0x10 ) {
    	LOG->DEBG("ERROR - ignoring"); // Seems to happens when there is no valid app
    } else {
    	LOG->DEBG("ok");
    }
    LOG->DEBGH("Status = ",status);
    // Invoke app erase
    LOG->DEBG("ccs811: app-erase ");
    ok= registerWrite(CCS811_REG_APP_ERASE,app_erase,4);
    if( !ok ) {
    	LOG->DEBG("FAILED");
      goto abort_begin;
    }
    delay_ms(CCS811_WAIT_AFTER_APPERASE_MS);


    while(grt<200){
    	LOG->DEBG("EA.");
    	grt++;

    }
    LOG->DEBG("ok");

    // Check status (CCS811 should be in boot mode without valid app, with erase completed)
    LOG->DEBG("ccs811: status (app-erase) ");
    ok= registerRead(CCS811_REG_STATUS,&status,1);
    if( !ok ) {
    	LOG->DEBG("FAILED");
      goto abort_begin;
    }
LOG->DEBGH("Status = ",status);
    if( status!=0x40 ) {
    	LOG->DEBG("ERROR");
      goto abort_begin;
    }
LOG->DEBG("ok");

    // Write all blocks
    count= 0;
    while( size>0 ) {
        if( count%64==0 ) { LOG->DEBG("ccs811: writing ",size);  }
        int len= size<8 ? size : 8;
        // Copy PROGMEM to RAM
        uint8_t ram[8];
      //  memcpy_P(ram, image, len);
       // memcpy(ram,image,len);
        ram[0] = *image;
        ram[1] = *(image+1);
        ram[2] = *(image+2);
        ram[3] = *(image+3);
        ram[4] = *(image+4);
        ram[5] = *(image+5);
        ram[6] = *(image+6);
        ram[7] = *(image+7);
        // Send 8 bytes from RAM to CCS811
        ok= registerWrite(CCS811_REG_APP_DATA,ram,len);
        if( !ok ) {
        	LOG->DEBG("ccs811: app data failed");
          goto abort_begin;
        }
        LOG->DEBG(".");
        LOG->DEBG(".");
        LOG->DEBG(".");
        LOG->DEBGH(".",*(image));
        delay_ms(CCS811_WAIT_AFTER_APPDATA_MS);
        image+= len;
        size-= len;
        count++;
        if( count%64==0 ) { LOG->DEBG(" ",size); }
    }
    if( count%64!=0 ) { LOG->DEBG(" ", size); }

    LOG->DEBG(".");
           LOG->DEBG(".");
           LOG->DEBG(".");
           LOG->DEBGH(".",*(image));
           LOG->DEBG(".");
                  LOG->DEBG(".");
                  LOG->DEBG(".");
                  LOG->DEBGH(".",*(image));
                  LOG->DEBG(".");
                         LOG->DEBG(".");
                         LOG->DEBG(".");
                         LOG->DEBGH(".",*(image));
                         LOG->DEBG(".");
                                LOG->DEBG(".");
                                LOG->DEBG(".");
                                LOG->DEBGH(".",*(image));
                                LOG->DEBG(".");
                                       LOG->DEBG(".");
                                       LOG->DEBG(".");
                                       LOG->DEBGH(".",*(image));
                                       LOG->DEBG(".");
                                              LOG->DEBG(".");
                                              LOG->DEBG(".");
                                              LOG->DEBGH(".",*(image));

    // Invoke app verify
    LOG->DEBG("ccs811: app-verify ");
    ok= registerWrite(CCS811_REG_APP_VERIFY,app_verify,0);
    if( !ok ) {
    	LOG->DEBG("FAILED");
      goto abort_begin;
    }
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");

    delay_ms(CCS811_WAIT_AFTER_APPVERIFY_MS);
    LOG->DEBG("ok");

    // Check status (CCS811 should be in boot mode with valid app, and erased and verified)
    LOG->DEBG("ccs811: status (app-verify) ");
    ok= registerRead(CCS811_REG_STATUS,&status,1);
    if( !ok ) {
    	LOG->DEBG("FAILED");
      goto abort_begin;
    }

    if( status!=0x30 ) {
    	LOG->DEBGH("ERROR",status);
      goto abort_begin;
    }
    LOG->DEBGH("ok",status);

    // Invoke a second SW reset (clear flashing flags)
    LOG->DEBG("ccs811: reset2 ");
    ok= registerWrite(CCS811_REG_SW_RESET,sw_reset,4);
    if( !ok ) {
    	LOG->DEBG("FAILED");
      goto abort_begin;
    }
    delay_ms(CCS811_WAIT_AFTER_RESET_US);

    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBG(".");
    LOG->DEBGH("ok",status);

    // Check status (after reset, CCS811 should be in boot mode with valid app)
    LOG->DEBG("ccs811: status (reset2) ");
    ok= registerRead(CCS811_REG_STATUS,&status,1);
    if( !ok ) {
    	LOG->DEBG("FAILED");
      goto abort_begin;
    }

    if( status!=0x10 ) {
    	LOG->DEBGH("ERROR",status);
      goto abort_begin;
    }
    LOG->DEBGH("ok",status);

  // CCS811 back to sleep
  wakeDown();


  LOG->DEBG(".");
  LOG->DEBG(".");
  LOG->DEBG(".");
  LOG->DEBG(".");
  LOG->DEBG(".");

  LOG->DEBG(".");
  LOG->DEBG(".");
  LOG->DEBG(".");
  LOG->DEBG(".");
  LOG->DEBG(".");

  LOG->DEBG(".");
  LOG->DEBG(".");
  LOG->DEBG(".");
  LOG->DEBG(".");
  LOG->DEBG("!");
  wakeUp();
  // Return success


//init(0, CCS811_I2C_ADDRESS_1);

 // isAvailable();
  return true;

abort_begin:
  // CCS811 back to sleep
  wakeDown();
  // Return failure
#else
  return false;
#endif

}



void CCS811::init(uint8_t busl, uint8_t addrl)
{




    // init sensor data structure
    bus  = busl;
    addr = addrl;
    mode = ccs811_mode_idle;
    error_code = CCS811_OK;

    // check whether sensor is available including the check of the hardware
    // id and the error state
    if (!isAvailable())
    {

    }

    const static uint8_t sw_reset[4] = { 0x11, 0xe5, 0x72, 0x8a };

    // doing a software reset first
    if (!registerWrite(CCS811_REG_SW_RESET, (uint8_t*)sw_reset, 4))
    {

    }

    uint8_t status;


    // get the status to check whether sensor is in bootloader mode
    if (!registerRead(CCS811_REG_STATUS, &status, 1))
    {

    }

    // if sensor is in bootloader mode (FW_MODE == 0), it has to switch
    // to the application mode first
    if (!(status & CCS811_STATUS_FW_MODE))
    {
        // check whether valid application firmware is loaded
        if (!(status & CCS811_STATUS_APP_VALID))
        {

        }

        // swtich to application mode
        if (!registerWrite(CCS811_REG_APP_START, 0, 0))
        {

        }



        // get the status to check whether sensor switched to application mode
        if (!registerRead(CCS811_REG_STATUS, &status, 1) ||
            !(status & CCS811_STATUS_FW_MODE))
        {

        }
    }

    // try to set default measurement mode to *ccs811_mode_1s*
    if (!setMode(ccs811_mode_1s))
    {

    }

}
bool CCS811::getStatus(uint8_t* stat)
{
	if(!registerRead(CCS811_REG_STATUS, stat, 1))
	{
		return false;
	}

	return true;
}

bool CCS811::setMode (ccs811_mode_t model)
{
    ccs811_meas_mode_reg_t reg;



    error_code = CCS811_OK;

    // read measurement mode register value
    if (!registerRead(CCS811_REG_MEAS_MODE, (uint8_t*)&reg, 1))
        return false;

    reg.drive_mode = model;

    // write back measurement mode register
    if (!registerWrite(CCS811_REG_MEAS_MODE, (uint8_t*)&reg, 1))
    {

        return false;
    }

    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");
    LOG->DEBG("Wait write mode");

    // check whether setting measurement mode were succesfull
    if (!registerRead(CCS811_REG_MEAS_MODE, (uint8_t*)&reg, 1) ||
        reg.drive_mode != model)
    {
        return checkErrorStatus();
    }

    mode = model;

    return true;
}



bool CCS811::getResults(
                         uint16_t* iaq_tvoc,
                         uint16_t* iaq_eco2,
                         uint8_t*  raw_i,
                         uint16_t* raw_v)
{


    error_code = CCS811_OK;

    if (mode == ccs811_mode_idle)
    {
    	LOG->DEBG("Mode = IDLE!");
        return false;
    }

    if (mode == ccs811_mode_250ms && (iaq_tvoc || iaq_eco2))
    {

        error_code = CCS811_DRV_NO_IAQ_DATA;
        LOG->DEBG("Mode = 250ms!");
        return false;
    }

    uint8_t data[8];

    // read IAQ sensor values and RAW sensor data including status and error id
    if (!registerRead(CCS811_REG_ALG_RESULT_DATA, data, 8))
    {
    	LOG->DEBG("RDFailed!");
        error_code |= CCS811_DRV_RD_DATA_FAILED;
        return false;
    }

    // check for errors
  /*  if (data[CCS811_ALG_DATA_STATUS] & CCS811_STATUS_ERROR)
    {
    	LOG->DEBG("Errors occured!");
        return checkErrorStatus();
    }

    // check whether new data are ready, if not, latest values are read from sensor
    // and error_code is set
    if (!(data[CCS811_ALG_DATA_STATUS] & CCS811_STATUS_DATA_RDY))
    {
        error_code = CCS811_DRV_NO_NEW_DATA;
    }
*/
    // if *iaq* is not NULL return IAQ sensor values
    if (iaq_tvoc) *iaq_tvoc = data[CCS811_ALG_DATA_TVOC_HB] << 8 | data[CCS811_ALG_DATA_TVOC_LB];
    if (iaq_eco2) *iaq_eco2 = data[CCS811_ALG_DATA_ECO2_HB] << 8 | data[CCS811_ALG_DATA_ECO2_LB];

    // if *raw* is not NULL return RAW sensor data
    if (raw_i) *raw_i = data[CCS811_ALG_DATA_RAW_HB] >> 2;
    if (raw_v) *raw_v = (data[CCS811_ALG_DATA_RAW_HB] & 0x03) << 8 | data[CCS811_ALG_DATA_RAW_LB];

    return true;
}

uint32_t CCS811::getNTCValue(uint32_t r_ref)
{


    uint8_t data[4];

    // read baseline register
    if (!registerRead(CCS811_REG_NTC, data, 4))
        return 0;

    // calculation from application note ams AN000372
    uint16_t v_ref = (uint16_t)(data[0]) << 8 | data[1];
    uint16_t v_ntc = (uint16_t)(data[2]) << 8 | data[3];

    return (v_ntc * r_ref / v_ref);
}


bool CCS811::setEnviromentalData (
                                    float temperature,
                                    float humidity)
{


    uint16_t temp = (temperature + 25) * 512;   // -25 °C maps to 0
    uint16_t hum  = humidity * 512;

    // fill environmental data
    uint8_t data[4]  = { temp >> 8, temp & 0xff,
                         hum  >> 8, hum  & 0xff  };

    // send environmental data to the sensor
    if (!registerWrite(CCS811_REG_ENV_DATA, data, 4))
    {

        return false;
    }

    return true;
}

bool CCS811::setEcoThresholds (uint16_t low, uint16_t high, uint8_t hysteresis)
{


    error_code = CCS811_OK;

    // check whether interrupt has to be disabled
    if (!low && !high && !hysteresis)
        return enableThreshold(false);

    // check parameters
    if (low < CCS_ECO2_RANGE_MIN || high > CCS_ECO2_RANGE_MAX || low > high || !hysteresis)
    {

        error_code = CCS811_DRV_WRONG_PARAMS;
        return enableThreshold(false);
    }

    // fill the threshold data
    uint8_t data[5] = { low  >> 8, low  & 0xff,
                        high >> 8, high & 0xff,
                        hysteresis };

    // write threshold data to the sensor
    if (!registerWrite(CCS811_REG_THRESHOLDS, data, 5))
    {
        return enableThreshold(false);
    }

    // finally enable the threshold interrupt mode
    return enableThreshold(true);
}


bool CCS811::enableInterrupts ( bool enabled)
{


    ccs811_meas_mode_reg_t reg;

    // read measurement mode register value
    if (!registerRead(CCS811_REG_MEAS_MODE, (uint8_t*)&reg, 1))
        return false;

    reg.int_datardy = enabled;
    reg.int_thresh  = false;      // threshold mode must not enabled

    // write back measurement mode register
    if (!registerWrite(CCS811_REG_MEAS_MODE, (uint8_t*)&reg, 1))
    {

        return false;
    }

    return true;
}

uint16_t CCS811::getBaseline ()
{


    uint8_t data[2];

    // read baseline register
    if (!registerWrite(CCS811_REG_BASELINE, data, 2))
        return 0;

    return (uint16_t)(data[0]) << 8 | data[1];
}


bool CCS811::setBaseline (uint16_t baseline)
{


    uint8_t data[2] = { baseline >> 8, baseline & 0xff };

    // write baseline register
    if (!registerWrite(CCS811_REG_BASELINE, data, 2))
        return false;

    return true;
}


/**
 * FUNCTIONS FOR INTERNAL USE ONLY
 */


bool CCS811::enableThreshold(bool enabled)
{


    ccs811_meas_mode_reg_t reg;

    // first, enable/disable the data ready interrupt
    if (!enableInterrupts(enabled))
        return false;

    // read measurement mode register value
    if (!registerRead(CCS811_REG_MEAS_MODE, (uint8_t*)&reg, 1))
        return false;

    // second, enable/disable the threshold interrupt mode
    reg.int_thresh = enabled;

    // write back measurement mode register
    if (!registerWrite(CCS811_REG_MEAS_MODE, (uint8_t*)&reg, 1))
    {

        return false;
    }

    return true;
}


bool CCS811::registerRead(uint8_t reg, uint8_t *data, uint32_t len)
{
    if (!data) return false;
    int result = i2c_slave_read(addr, reg, data, len);

    if (result)
    {
      // error_code |= (result == -EBUSY) ? CCS811_I2C_BUSY : CCS811_I2C_READ_FAILED;

        return false;
    }
    return true;
}


bool CCS811::registerWrite(uint8_t reg, uint8_t *data, uint32_t len)
{
    int result = i2c_slave_write(addr, reg, data, len);

    if (result)
    {
       // error_code |= (result == -EBUSY) ? CCS811_I2C_BUSY : CCS811_I2C_WRITE_FAILED;

        return false;
    }

    return true;
}


bool CCS811::checkErrorStatus ()
{


    error_code = CCS811_OK;

    uint8_t status;
    uint8_t err_reg;

    // check status register
    if (!registerRead(CCS811_REG_STATUS, &status, 1))
        return false;

    if (!status & CCS811_STATUS_ERROR)
        // everything is fine
        return true;

    // Check the error register
    if (!registerRead(CCS811_REG_ERROR_ID, &err_reg, 1))
        return false;

    if (err_reg & CCS811_ERR_WRITE_REG_INV)
    {

        error_code = CCS811_DRV_WR_REG_INV;
        return false;
    }

    if (err_reg & CCS811_ERR_READ_REG_INV)
    {

        error_code = CCS811_DRV_RD_REG_INV;
        return false;
    }

    if (err_reg & CCS811_ERR_MEASMODE_INV)
    {

        error_code = CCS811_DRV_MM_INV;
        return false;
    }

    if (err_reg & CCS811_ERR_MAX_RESISTANCE)
    {

        error_code = CCS811_DRV_MAX_RESIST;
        return false;
    }

    if (err_reg & CCS811_ERR_HEATER_FAULT)
    {

        error_code = CCS811_DRV_HEAT_FAULT;
        return false;
    }

    if (err_reg & CCS811_ERR_HEATER_SUPPLY)
    {

        error_code = CCS811_DRV_HEAT_SUPPLY;
        return false;
    }

    return true;
}


bool CCS811::isAvailable()
{


    uint8_t reg_data[5];

    // check hardware id (register 0x20) and hardware version (register 0x21)
    if (!registerRead(CCS811_REG_HW_ID, reg_data, 5))
        return false;

    if (reg_data[0] != 0x81)
    {

        error_code = CCS811_DRV_HW_ID;
        return false;
    }

    LOG->DEBGH("hardware version:",reg_data[1]);
    LOG->DEBGH("firmvware boot version:",reg_data[3]);
    LOG->DEBGH("Appl version:",reg_data[4]);
 //   debug_dev ("hardware version:      %02x", __FUNCTION__, dev, reg_data[1]);
 //   debug_dev ("firmware boot version: %02x", __FUNCTION__, dev, reg_data[3]);
  //  debug_dev ("firmware app version:  %02x", __FUNCTION__, dev, reg_data[4]);

    return checkErrorStatus();
}
