// This example prints the uncalibrated internal temperature to the UART
#include "STM32G030.h"
#include <stdint.h>
#include <stdlib.h>
#include "system.h"
#include "serial.h"
#include "logger.h"
#include "rtctime.h"
#include "ccs811.h"
#include "appbin_2_0_1.h"
#include "bme680.h"
#include "bme680_defs.h"
#include "sht40.h"
#include "sgp30.h"
#include <string>
#include "spi.h"
#include "adc.h"
#include "voltageconverter.h"

#include "oled1306.h"

OLED1306Display display;

CCS811 sensor811;
SHT40* sensor;
SGP30 sgp30Sensor;
struct bme680_dev gsens;

Serial serial;
RTCTime time;//using to configure and read realtime
uint8_t flagSecund{ 0 };
AnalogConverter adc;




void RTCIrq_Handler()
{
	if((RTC->MISR & 0x00000001) == 0x00000001){ RTC->SCR |= 0x00000001; /*flagMinute = 1;*/}
	if((RTC->MISR & 0x00000002) == 0x00000002){ RTC->SCR |= 0x00000002; flagSecund = 1; }
	//led->on();
}

uint32_t I2CSRValue(){ return I2C2->ISR; }

#define I2C_NO_ERR 0
#define I2C_NO_DEVICE -1
#define I2C_TIMEOUT -2
#define I2C_BUSY -3
#define I2C_ARLO -4
#define I2C_BUSERR -5

//#define I2C
int8_t errI2C2()
{
	if(I2C2->ISR & 0x00000020){ I2C2->ICR |= 0x00000020; }//STOPF
	if(I2C2->ISR &0x00000010){ I2C2->ICR |= 0x00000010; return I2C_NO_DEVICE; }//NACK
	if(I2C2->ISR & 0x00008000){ I2C2->CR1&= ~0x00000001; return I2C_BUSY; }
	if(I2C2->ISR & 0x00000200){ I2C2->ICR |= 0x00000200; return I2C_ARLO; }
	if(I2C2->ISR & 0x00000100){I2C2->ICR |= 0x00000100;return I2C_BUSERR; }
	return I2C_TIMEOUT;
}


int8_t writeI2C2(uint8_t addrMode, uint16_t deviceAddress, uint16_t memAddress, uint8_t count, uint8_t* data, bool twoByteAddress)
{
	uint32_t tmp = count + 1;//+ address byte

	if(twoByteAddress) tmp = count + 2;

	I2C2->CR2 |= ((addrMode&0x1)<<11);///ADD10 set address mode 0 - 7bit, 1 - 10bit

	I2C2->CR2 &= ~0x00000500; //RD/WRN = 0 - write
	I2C2->CR2 &= ~0x00ff0000; //clear num bytes
	I2C2->CR2 |= tmp<<16;

	I2C2->CR2 |= 0x02000000;//AUTOEND = 1; STOP after transfer
	I2C2->CR2 &= ~0xff;
	I2C2->CR2 |= deviceAddress; // set slave address

	I2C2->CR2 |= 0x00002000; //start

	volatile uint32_t waitTimer = 0;
	int countToSend = 0;
	//write address byte
	if(twoByteAddress)
	{
		while((I2C2->ISR & 0x00000002) == 0){ waitTimer++; if(waitTimer>10000) return errI2C2(); }//wait until txis setted to 1
		I2C2->TXDR = (memAddress>>8)&0xff;//data[countToSend];//High byte of address

		waitTimer = 0;
		while((I2C2->ISR & 0x00000002) == 0){ waitTimer++; if(waitTimer>10000) return errI2C2(); }//wait until txis setted to 1
		I2C2->TXDR = memAddress&0xff;//data[countToSend];
	}
	else
	{
		while((I2C2->ISR & 0x00000002) == 0){ waitTimer++; if(waitTimer>10000) return errI2C2(); }//wait until txis setted to 1
		I2C2->TXDR = memAddress&0xff;//data[countToSend];
	}
	//transmit while num bytes != 0;
	while(countToSend<count)
	{
		waitTimer = 0;
		while((I2C2->ISR & 0x00000002) == 0){ waitTimer++; if(waitTimer>10000) return errI2C2(); }; //wait until txis setted to 1
		I2C2->TXDR = data[countToSend];
		countToSend++;
	}
	return I2C_NO_ERR;
}




//read count bytes from device(deviceAddress) position addrFom
int8_t readI2C2(uint8_t addrMode, uint16_t deviceAddress, uint16_t addrFrom, uint8_t count, uint8_t* data, bool twoByteAddress)
{
	//write address of memory to be readed
	uint32_t tmp = 1;

	if(twoByteAddress) tmp = 2;

	//i2cStatusFlag = 1;
	I2C2->CR2 |= ((addrMode&0x1)<<11);///ADD10 set address mode 0 - 7bit, 1 - 10bit

	I2C2->CR2 &= ~0x00000500; //RD/WRN = 0 - write
	I2C2->CR2 &= ~0x00ff0000;//clear num bytes
	I2C2->CR2 |= tmp<<16;//set actual num bytes
	I2C2->CR2 &= ~0x02000000;//AUTOEND = 0; no STOP after transfer
	I2C2->CR2 &= ~0x000000ff;
	I2C2->CR2 |= deviceAddress&0xff; // set slave address

	I2C2->CR2 |= 0x00002000; //start

	int countToRecieve = 0;
	volatile uint32_t waitTimer = 0;

	if(twoByteAddress)
	{
		while((I2C2->ISR & 0x00000002) == 0){ waitTimer++; if(waitTimer>10000)return errI2C2(); }; //wait until txis setted to 1
		I2C2->TXDR = (addrFrom>>8)&0xff;//High byte of address
		waitTimer = 0;
		while((I2C2->ISR & 0x00000002) == 0){ waitTimer++; if(waitTimer>10000)return errI2C2(); }; //wait until txis setted to 1

		I2C2->TXDR = (addrFrom)&0xff;//Low Byte of address
	}
	else
	{
		waitTimer = 0;
		while((I2C2->ISR & 0x00000002) == 0){ waitTimer++; if(waitTimer>10000)return errI2C2(); }; //wait until txis setted to 1
		I2C2->TXDR = addrFrom & 0xff;//address
	}
	waitTimer = 0;

	while((I2C2->ISR & 0x00000040) == 0){ waitTimer++; if(waitTimer>10000)return errI2C2(); }; //wait until tc setted to 1

	tmp = count;
	//read data, set devaddress and read mode
	I2C2->CR2 |= 0x00000400; //RD/WRN = 1 - read
	I2C2->CR2 &= ~0x00ff0000;//clear num bytes
	I2C2->CR2 |= tmp<<16;//set actual num bytes
	I2C2->CR2 &= ~0x01000000;
	I2C2->CR2 |= deviceAddress&0xff; // set slave address

	I2C2->CR2 |= 0x02000000;//AUTOEND = 1; STOP after transfer

	I2C2->CR2 |= 0x00002000; //start

	//i2cStatusFlag = 0;
	while(countToRecieve<count)
	{
		waitTimer = 0;
		while((I2C2->ISR & 0x00000004) == 0){ waitTimer++; if(waitTimer>10000)return errI2C2(); };
		data[countToRecieve] = I2C2->RXDR;
		countToRecieve++;
	}

	//I2C2->CR2 |= 0x00004000;//STOP after transfer complete
	//waitTimer = 0;
	while((I2C2->ISR & 0x00000020) == 0x00000000){ waitTimer++; if(waitTimer>10000)return errI2C2(); };
	I2C2->ICR |= 0x00000020;//clear stop flag

	return I2C_NO_ERR;
}
void offI2C22()
{
	I2C2->CR1 &= ~0x00000001;
	while((I2C2->CR1 & 0x00000001)!=0);
}



int8_t i2cScan(uint8_t &address, uint8_t addrFrom)
{
	int i = addrFrom;
	int8_t err { 0 };
	while(i<128)
	{
		I2C2->CR2 &= ~0x00000500; //RD/WRN = 0 - write
		I2C2->CR2 &= ~0x00ff0000; //clear num bytes
		I2C2->CR2 |= 1<<16;
		I2C2->CR2 |= 0x02000000;//AUTOEND = 1; STOP after transfer
		I2C2->CR2 &= ~0x000003ff;
		I2C2->CR2 |= (uint8_t)((i<<1)&0xff); // set slave address
		I2C2->CR2 |= 0x00002000; //start
		//I2C2->CR2 |= 0x00004000;
		volatile uint32_t waitTimer = 0;
		err = 0;
		while((I2C2->ISR & 0x00000002) == 0){ waitTimer++; if(waitTimer>10000){ err = errI2C2(); break; }}//wait until txis setted to 1
		if(waitTimer>10000)
		{
			//LOG->DEBGH("Timeout! addr = ",i);
			//LOG->DEBGH("ISR = ",I2C2->ISR);
			//LOG->DEBG("Error = ", err);
		}
		else
		{
			address = i;
			if(I2C2->ISR & 0x00000020){ I2C2->ICR |= 0x00000020; }//STOPF
			offI2C22();
			I2C2->CR1|=0x00000001;
			return 0;
		}
		i++;
	}

	return err;
}

int8_t i2cFindDevices()
{
	int i = 1;
	uint8_t addr { 0 };
	uint8_t count { 0 };
	while(i<128)
	{
		if(i2cScan(addr,i) == I2C_NO_ERR)
		{
			LOG->DEBGH("Found device, ", addr);
			count++;
			i = addr + 1;
		}
		else
		{
			LOG->DEBG("No I2C devices");
			return count;
		}
	}
	return count;
}

//read count bytes from device(deviceAddress) position addrFom
int8_t readI2C2Simple(uint8_t addrMode, uint16_t deviceAddress, uint8_t count, uint8_t* data)
{
	//write address of memory to be readed
	uint16_t tmp = count;

	//read data, set devaddress and read mode
	I2C2->CR2 |= 0x00000400; //RD/WRN = 1 - read
	I2C2->CR2 &= ~0x00ff0000;//clear num bytes
	I2C2->CR2 |= tmp<<16;//set actual num bytes
	I2C2->CR2 &= ~0x000003ff;
	I2C2->CR2 |= deviceAddress&0xff; // set slave address

	I2C2->CR2 |= 0x02000000;//AUTOEND = 1; STOP after transfer

	I2C2->CR2 |= 0x00002000; //star

	int countToRecieve = 0;
	volatile uint32_t waitTimer = 0;

	//i2cStatusFlag = 0;
	while(countToRecieve<count)
	{
		//waitTimer = 0;
		while((I2C2->ISR & 0x00000004) == 0){ waitTimer++; if(waitTimer>10000) return -1; };
		data[countToRecieve] = I2C2->RXDR;
		countToRecieve++;
	}

	//if(I2C2->ISR & 0x00000040){I2C2->CR2 |= 0x00004000;  I2C2->ICR |= 0x00000020;return -1;}
	//waitTimer = 0;
	//while((I2C2->ISR & 0x00000040) == 0) { /*waitTimer++;* if(waitTimer>10000) return -1; */}; //wait until tc setted to 1
	//	}
	//}
	return 0;
}

int8_t writeI2CBME(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{

	return writeI2C2(0,dev_id<<1,reg_addr,len,data,false);
	//return 0;
}

int8_t readI2CBME(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
	return readI2C2(0, dev_id<<1, reg_addr, len, data, false);
	//return 0;
}

uint8_t writeI2CSHT(uint8_t dev_id, uint8_t *data, uint16_t len)
{

	return writeI2C2(0,dev_id<<1,data[0],len-1,data+1,false);
	//return 0;
}

uint8_t readI2CSHT(uint8_t dev_id, uint8_t *data, uint16_t len)
{
	return readI2C2Simple(0, dev_id<<1,len, data);
	//return 0;
}

void delaymsBME(long unsigned int delay)
{
	delay_ms(delay);
}


int   humidity_score, gas_score;
float gas_reference = 2500;
float hum_reference = 40;
int   getgasreference_count = 0;
float   gas_lower_limit = 10000;  // Bad air quality limit
float   gas_upper_limit = 300000; // Good air quality limit

int getGasScore() {
  //Calculate gas contribution to IAQ index
  gas_score = (0.75 / (gas_upper_limit - gas_lower_limit) * gas_reference - (gas_lower_limit * (0.75 / (gas_upper_limit - gas_lower_limit)))) * 100.00;
  if (gas_score > 75) gas_score = 75; // Sometimes gas readings can go outside of expected scale maximum
  if (gas_score <  0) gas_score = 0;  // Sometimes gas readings can go outside of expected scale minimum
  return gas_score;
}

void initOledPins()
{
	//PA6, PA8,
GPIOA->MODER |= 0x00011000;
GPIOA->MODER &= ~0x00022000;

}


void initializeSensors()
{
	sgp30Sensor.setInterface(readI2C2, writeI2C2, readI2C2Simple);

		sgp30Sensor.probe();
		uint16_t feature_set_version;
		uint8_t product_type;

		uint16_t i = 0;
		int16_t err;
		uint16_t tvoc_ppb, co2_eq_ppm;
		uint32_t iaq_baseline;
		uint16_t ethanol_raw_signal, h2_raw_signal;

		err = sgp30Sensor.getFeatureSetVersion(&feature_set_version, &product_type);
		if (err == SGP30::STATUS_OK) {
			LOG->DEBG("Feature set version: ", feature_set_version);
			LOG->DEBG("Product type: ", product_type);
		} else {
			LOG->DEBG("sgp30_get_feature_set_version failed!");
		}
		uint64_t serial_id;
		err = sgp30Sensor.getID(&serial_id);
		if (err == SGP30::STATUS_OK) {
			LOG->DEBG("SerialID: ", serial_id);
		} else {
			LOG->DEBG("sgp30_get_serial_id failed!\n");
		}

		/* Read gas raw signals */
		err = sgp30Sensor.readRaw(&ethanol_raw_signal, &h2_raw_signal);
		if (err == SGP30::STATUS_OK) {
			/* Print ethanol raw signal and h2 raw signal */
			LOG->DEBG("Ethanol raw signal: ", ethanol_raw_signal);
			LOG->DEBG("H2 raw signal: ", h2_raw_signal);
		} else {
			LOG->DEBG("error reading raw signals");
		}

		err = sgp30Sensor.IAQInit();
		if (err == SGP30::STATUS_OK) {
			LOG->DEBG("sgp30_iaq_init done\n");
		} else {
			LOG->DEBG("sgp30_iaq_init failed!\n");
		}






		//sht40
		sensor = new SHT40(readI2CSHT, writeI2CSHT);

		uint32_t sn { 0 };
		if(sensor->ping(sn) == SHT40::STATUS_OK) serial.DEBG("Sensor probe ok!",sn);
		sensor->startMeasure();

		sensor811.setWakePin(&(GPIOA->ODR), 0x00000400);
		sensor811.setInterface(readI2CBME, writeI2CBME);
		sensor811.setAddress(CCS811_I2C_ADDRESS_1);
		sensor811.checkFirmware();
		sensor811.setMode(ccs811_mode_1s);



		gsens.dev_id = 0x77;//BME680_I2C_ADDR_PRIMARY;
		gsens.intf = BME680_I2C_INTF;

		gsens.read = readI2CBME;
		gsens.write = writeI2CBME;
		gsens.delay_ms = delaymsBME;
		gsens.amb_temp = 25;

		LOG->DEBG("Waiting bme ready...");
		LOG->DEBG("Waiting bme ready...");
		LOG->DEBG("Waiting bme ready...");
		LOG->DEBG("Waiting bme ready...");
		LOG->DEBG("Waiting bme ready...");
		LOG->DEBG("Waiting bme ready...");
		LOG->DEBG("Waiting bme ready...");
		LOG->DEBG("Waiting bme ready...");
		LOG->DEBG("Waiting bme ready...");
		LOG->DEBG("Waiting bme ready...");
		LOG->DEBG("Waiting bme ready...");
		LOG->DEBG("Waiting bme ready...");
		LOG->DEBG("Waiting bme ready...");
		LOG->DEBG("Waiting bme ready...");
		int8_t result = BME680_OK;
		result = bme680_init(&gsens);
		LOG->DEBG("Sensor init = ",result);
		uint8_t settings;



		gsens.tph_sett.os_hum = BME680_OS_2X;
		gsens.tph_sett.os_pres = BME680_OS_4X;
		gsens.tph_sett.os_temp = BME680_OS_8X;
		gsens.tph_sett.filter = BME680_FILTER_SIZE_3;

		gsens.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
		gsens.gas_sett.heatr_temp = 320;
		gsens.gas_sett.heatr_dur = 150;

		gsens.power_mode = BME680_FORCED_MODE;

		settings = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;

		result = bme680_set_sensor_settings(settings, &gsens);
		LOG->DEBG("set settings = ",result);
		result = bme680_set_sensor_mode(&gsens);
		LOG->DEBG("set mode = ",result);

		uint16_t meas_period;
		bme680_get_profile_dur(&meas_period, &gsens);

		struct bme680_field_data data;
}

int main()
{
	VoltageConverter vMeter { 1000000, 2700000, 4095, 3.3 };
	serial.begin();
	serial.DEBG("Starting hardware initialization...");
	serial.DEBG("RTC initialization...");

	RTCTime::TimeNDate tmdt;
	tmdt.hour = 0;
	tmdt.min = 0;
	tmdt.sec = 0;
	time.RTCInit();
	time.setTime(tmdt);

	serial.DEBG("Logger initialization...");
	LOG->setSerial(&serial);
	LOG->setTimeSource(&time);

	LOG->DEBG("Logger is on!");

	//LOG->DEBG("Total i2c devices - ", i2cFindDevices());





	initOledPins();
	initSPI();


	sgp30Sensor.setInterface(readI2C2, writeI2C2, readI2C2Simple);
	sgp30Sensor.probe();
			uint16_t feature_set_version;
			uint8_t product_type;

			uint16_t i = 0;
			int16_t err;
			uint16_t tvoc_ppb, co2_eq_ppm;
			uint32_t iaq_baseline;
			uint16_t ethanol_raw_signal, h2_raw_signal;

			err = sgp30Sensor.getFeatureSetVersion(&feature_set_version, &product_type);
			if (err == SGP30::STATUS_OK) {
				LOG->DEBG("Feature set version: ", feature_set_version);
				LOG->DEBG("Product type: ", product_type);
			} else {
				LOG->DEBG("sgp30_get_feature_set_version failed!");
			}
			uint64_t serial_id;
			err = sgp30Sensor.getID(&serial_id);
			if (err == SGP30::STATUS_OK) {
				LOG->DEBG("SerialID: ", serial_id);
			} else {
				LOG->DEBG("sgp30_get_serial_id failed!\n");
			}

			/* Read gas raw signals */
			err = sgp30Sensor.readRaw(&ethanol_raw_signal, &h2_raw_signal);
			if (err == SGP30::STATUS_OK) {
				/* Print ethanol raw signal and h2 raw signal */
				LOG->DEBG("Ethanol raw signal: ", ethanol_raw_signal);
				LOG->DEBG("H2 raw signal: ", h2_raw_signal);
			} else {
				LOG->DEBG("error reading raw signals");
			}

			err = sgp30Sensor.IAQInit();
			if (err == SGP30::STATUS_OK) {
				LOG->DEBG("sgp30_iaq_init done\n");
			} else {
				LOG->DEBG("sgp30_iaq_init failed!\n");
			}






			//sht40
			sensor = new SHT40(readI2CSHT, writeI2CSHT);

			uint32_t sn { 0 };
			if(sensor->ping(sn) == SHT40::STATUS_OK) serial.DEBG("Sensor probe ok!",sn);
			sensor->startMeasure();

			sensor811.setWakePin(&(GPIOA->ODR), 0x00000400);
			sensor811.setInterface(readI2CBME, writeI2CBME);
			sensor811.setAddress(CCS811_I2C_ADDRESS_1);
			sensor811.checkFirmware();
			sensor811.setMode(ccs811_mode_1s);



			gsens.dev_id = 0x77;//BME680_I2C_ADDR_PRIMARY;
			gsens.intf = BME680_I2C_INTF;

			gsens.read = readI2CBME;
			gsens.write = writeI2CBME;
			gsens.delay_ms = delaymsBME;
			gsens.amb_temp = 25;

			LOG->DEBG("Waiting bme ready...");
			LOG->DEBG("Waiting bme ready...");
			LOG->DEBG("Waiting bme ready...");
			LOG->DEBG("Waiting bme ready...");
			LOG->DEBG("Waiting bme ready...");
			LOG->DEBG("Waiting bme ready...");
			LOG->DEBG("Waiting bme ready...");
			LOG->DEBG("Waiting bme ready...");
			LOG->DEBG("Waiting bme ready...");
			LOG->DEBG("Waiting bme ready...");
			LOG->DEBG("Waiting bme ready...");
			LOG->DEBG("Waiting bme ready...");
			LOG->DEBG("Waiting bme ready...");
			LOG->DEBG("Waiting bme ready...");
			int8_t result = BME680_OK;
			result = bme680_init(&gsens);
			LOG->DEBG("Sensor init = ",result);
			uint8_t settings;



			gsens.tph_sett.os_hum = BME680_OS_2X;
			gsens.tph_sett.os_pres = BME680_OS_4X;
			gsens.tph_sett.os_temp = BME680_OS_8X;
			gsens.tph_sett.filter = BME680_FILTER_SIZE_3;

			gsens.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
			gsens.gas_sett.heatr_temp = 320;
			gsens.gas_sett.heatr_dur = 150;

			gsens.power_mode = BME680_FORCED_MODE;

			settings = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;

			result = bme680_set_sensor_settings(settings, &gsens);
			LOG->DEBG("set settings = ",result);
			result = bme680_set_sensor_mode(&gsens);
			LOG->DEBG("set mode = ",result);

			uint16_t meas_period;
			bme680_get_profile_dur(&meas_period, &gsens);

			struct bme680_field_data data;



	display.setInterface(&(GPIOA->ODR), 0x00000100, 0x00000040, transferSPI8);
	display.reset();
	display.init(128, 64);

	adc.init();
	//pb0
	adc.initChannels(AnalogConverter::ADC_8 | AnalogConverter::ADC_9 | AnalogConverter::ADC_7);


	/*std::string humidityString = new std::string("");
	humidityString = "Влажнoсть-";
	humidityString += std::to_string(47);
	humidityString += "%";

	std::string temperatureString = "";
	temperatureString = "t1-";
	temperatureString += std::to_string(25);
	temperatureString += "C, ";
	temperatureString += "t2-";
	temperatureString += std::to_string(27);
	temperatureString += "C, ";


	std::string pressureString = "";
	pressureString = "Давление-";
	pressureString += std::to_string(768);
	pressureString += "mm";

	std::string TVOCString = "";
	TVOCString = "TVOC ";
	TVOCString += std::to_string(2222);
	TVOCString += " l ";
	TVOCString += std::to_string(3333);

	std::string CO2String = "";
	CO2String = "CO2 ";
	CO2String += std::to_string(4444);
	CO2String += " l ";
	CO2String += std::to_string(5555);

	std::string resistanceString = "";
	resistanceString = "Сопр.-";
	resistanceString += std::to_string(123000);
	resistanceString += "Ohm";


	display.drawString(temperatureString,0,0);
	display.drawString(humidityString,0,8);
	display.drawString(pressureString,0,16);
	display.drawString(TVOCString,0,24);
	display.drawString(CO2String,0,32);
	display.drawString(resistanceString,0,40);
	display.refresh();
	*/
	//display.fill(0x0f);
	while(1)
	{


		float vbat { 0 };
		int vbatD { 0 };
		float voltageBatterymV { 0.0 };
		float vbatM { 0 };
		int32_t tSHT40, hSHT40;

		if(flagSecund)
		{
			//sht40
			flagSecund = 0;

			vbat = vMeter.getVoltage(adc.getDataFiltered(AnalogConverter::ADC_8));
			voltageBatterymV = vbat*1000;
			vbatD = vbat;
			vbatM = (vbat - vbatD)*10;
			//GET SHT40 DATA
			bool shtErr = sensor->read(&tSHT40, &hSHT40);
			if(shtErr == 0) shtErr = sensor->startMeasure();
			if(shtErr!=0){ LOG->DEBG("SHT40 Reading Error!");}



			uint16_t tvoc;
			uint16_t eco2;
			uint8_t  raw_i;
			uint16_t raw_v;
			uint8_t status { 0 };
			bool ccs811Err { 0 };
			sensor811.setEnviromentalData(tSHT40, hSHT40);
			//// get the results and do something with them
			if (sensor811.getResults(&tvoc, &eco2, &raw_i, &raw_v)){}
			else
			{
				LOG->DEBG("Error get results from CCS811!!!");
				ccs811Err = true;
			}

			if(sensor811.getStatus(&status)){
				//LOG->DEBGH("status = ",status);
			}
			else
				LOG->DEBG("Error get status from CCS811!");

			//BME680
			bool bme680Err = bme680_get_sensor_data(&data, &gsens);
			if(bme680Err){ LOG->DEBG("Error reading BME680!"); }
			else
			{
				if(data.status & BME680_GASM_VALID_MSK){}


				if (gsens.power_mode == BME680_FORCED_MODE)
				{
					result = bme680_set_sensor_mode(&gsens);
				}
			}

			//SGP30


			sgp30Sensor.setAbsoluteHumidity(hSHT40);

			bool sgp30Err = sgp30Sensor.readIAQ(&tvoc_ppb, &co2_eq_ppm);
			if (err == SGP30::STATUS_OK)
			{
				sgp30Err = sgp30Sensor.measureIAQ();
				if(sgp30Err == SGP30::STATUS_OK)
				{

				}
				else
					LOG->DEBG("Start measure SGP30 errr!");

			} else
			{
				LOG->DEBG("error reading SGP30 IAQ values\n");
			}


			//
			gas_reference = (float)data.gas_resistance;
			int value = getGasScore();
			int score = (100-value)*5;



			std::string *weatherString = new std::string(" ");
			std::string *temperatureString = new std::string(" ");
			if(!bme680Err && !shtErr)
			{
				*weatherString = std::to_string(((int)(tSHT40/1000) + (int)(data.temperature/100))/2);

				*weatherString+="C";
				*weatherString+=(char)4;
				*weatherString+=" ";
				*weatherString+=(char)3;
				*weatherString+=std::to_string((int)((data.humidity/1000) + (int)(hSHT40/1000))/2);
				*weatherString += "% ";
				*weatherString+=(char)2;
				*weatherString += std::to_string((int)((float)data.pressure/133.3));

			}
			else
			{
				if(shtErr)
				{
					*weatherString+= std::to_string((int)(data.temperature/100));
					*weatherString+="C";
					*weatherString+=std::to_string((int)(data.humidity/1000));
					*weatherString += "%  ";
					*weatherString += std::to_string((int)((float)data.pressure/133.3));
				}
				else
				{
					if(!bme680Err)
					{
						*weatherString+= std::to_string(((int)(tSHT40/1000) + (int)(data.temperature/100))/2);
						*weatherString+="C";
						*weatherString+="я"+2;
						*weatherString+=std::to_string((int)((data.humidity/1000) + (int)(hSHT40/1000))/2);
						*weatherString += "%  ";
						*weatherString += std::to_string((int)((float)data.pressure/133.3));
					}
					else
					{

						*weatherString += "Err!";
						*weatherString+="C  ";
						*weatherString+="Err!";
						*weatherString += "%  ";

					}
				}
			}

			*weatherString += "ммрс";



			std::string* TVOCString = new std::string("000000");
			//*TVOCString = "V-(";
			*TVOCString = TVOCString->replace(TVOCString->length()-std::to_string(tvoc).length(), std::to_string(tvoc).length(), std::to_string(tvoc));
			//*TVOCString += ' ';
			//*TVOCString += (char)1;
			//*TVOCString += std::to_string(tvoc_ppb);
			//*TVOCString += ")";

			std::string* CO2String = new std::string("000000");
			//CO2String->
			//*CO2String = "C-(";
			*CO2String = CO2String->replace(CO2String->length()-std::to_string(eco2).length(), std::to_string(eco2).length(), std::to_string(eco2));
			//*CO2String += ' ';//(char)2;
			//*CO2String += (char)2;
			//*CO2String += (char)3;
			//*CO2String += (char)4;
			//*CO2String += ") (";
			//*CO2String += std::to_string(co2_eq_ppm);
			//*CO2String += ")";

			std::string* bmeString = new std::string(" ");
			*bmeString = "BME -";
			*bmeString += std::to_string(500-((data.gas_resistance-50000)/400));
			*bmeString += " IAQ";

			std::string* resistanceString = new std::string("000000");
			//*resistanceString = "Сопр.-";
			if(data.gas_resistance>999999)data.gas_resistance = 999999;
			*resistanceString = resistanceString->replace(resistanceString->length() - std::to_string(data.gas_resistance).length(), std::to_string(data.gas_resistance).length(),  std::to_string(data.gas_resistance));
			//*resistanceString += " Ohm";


			std::string* voltageString = new std::string(" ");
			*voltageString = "Напр. АКБ-";
			*voltageString = std::to_string((int)vbatD);
			*voltageString += ',';//std::to_string((int)vbatD);
			*voltageString += std::to_string((int)vbatM);
			*voltageString += "В";//std::to_string((int)vbatD);
				char sensorType { 'O' };
				std::string* sensorTypeString = new std::string("ТД-");
				*sensorTypeString += sensorType;

				display.drawString(*weatherString,0,0);

				//display.drawChar8x16FromGraphicArray(1, 64, 32);
				//display.drawChar8x16FromGraphicArray(2, 72, 32);
				//display.drawCharFromGraphicArray('2'-28, 80, 32);
				display.drawHorizontalLine(0,127,9);
				display.drawVerticalLine(10,63,90);





				display.drawBattery(voltageBatterymV, 96, 24, false);
				///int8_t chargeGraphValueX = GRAPHIC_BATTERY_RANGE-(LION_MAX_VOLTAGE - voltageBatterymV)/GRAPHIC_BATTERY_COEF;//()
				//if(chargeGraphValueX>22)chargeGraphValueX = 0;
				//if(chargeGraphValueX<0)chargeGraphValueX = GRAPHIC_BATTERY_RANGE;
				//display.drawHorizontalLine(96, chargeGraphValueX, 25);
				//display.drawHorizontalLine(96,chargeGraphValueX,26);
				//display.drawHorizontalLine(96,chargeGraphValueX,27);
				//display.drawHorizontalLine(96,chargeGraphValueX,28);
				//display.drawHorizontalLine(96,chargeGraphValueX,29);
				//display.drawHorizontalLine(96,chargeGraphValueX,30);
			//	display.drawHorizontalLine(96,chargeGraphValueX,31);




		/*
		{
					display.drawHorizontalLine(x,22,y);
									display.drawHorizontalLine(x,22,y+8);
									display.drawVerticalLine(y,y+8,x);
									display.drawVerticalLine(y+1,33,x-1);
									display.drawVerticalLine(y+1,33,x-2);
									display.drawVerticalLine(y+2,34,x+22);

									xe = 22-(4200 - voltage)
//1200 =

									if(voltage>3000)
									{

									}
									if(voltage>)
		}*/


			//	display.drawVerticalLine(24,31,120);
			/*	display.drawChar8x16FromGraphicArray(16,  0, 16);
				display.drawChar8x16FromGraphicArray(17,  10, 16);
				display.drawChar8x16FromGraphicArray(18,  20, 16);
				display.drawChar8x16FromGraphicArray(19,  30, 16);

				display.drawChar8x16FromGraphicArray(20,  0, 32);
				display.drawChar8x16FromGraphicArray(21,  10, 32);
				display.drawChar8x16FromGraphicArray(22,  20, 32);
				display.drawChar8x16FromGraphicArray(23,  30, 32);

				display.drawChar8x16FromGraphicArray(24,  0, 48);
								display.drawChar8x16FromGraphicArray(25,  10, 48);
								display.drawChar8x16FromGraphicArray(22,  20, 48);
								display.drawChar8x16FromGraphicArray(23,  30, 48);*/

				display.drawStringUtf8x16("      ", 32, 16);
				display.drawStringUtf8x16(resistanceString->c_str(), 32, 16);
				display.drawStringUtf8x16("      ", 32, 32);
				display.drawStringUtf8x16(TVOCString->c_str(), 32, 32);
				display.drawStringUtf8x16("      ", 32, 48);
				display.drawStringUtf8x16(CO2String->c_str(), 32, 48);
				display.drawStringUtf("СОПР", 0, 24);
				display.drawStringUtf("ОРГС", 0, 40);
				display.drawStringUtf("УГЛГ", 0, 56);

				display.drawStringUtf(sensorTypeString->c_str(),94,56);
				display.drawStringUtf(voltageString->c_str(),94,16);


								//display.drawCharFromGraphicArray(0x9e, 0, 0
				//display.drawCharFromGraphicArray(0x9e, 0, 0);
				//display.drawCharFromGraphicArray(' ', 8, 0);
				//display.drawCharFromGraphicArray(0x9e, 16, 0);
				//display.drawCharFromGraphicArray(0x9f, 24, 0);
				//display.drawCharFromGraphicArray(' ', 32, 0);
				//display.drawCharFromGraphicArray(0xa0, 40, 0);
				//display.drawCharFromGraphicArray(' ', 48, 0);
				//display.drawCharFromGraphicArray(0xa1, 56, 0);
				//diaplay.drawString()
				//display.drawString(*humidityString,0,8);
				//display.drawString(*pressureString,0,16);
			//	display.drawString(*TVOCString,0,24);
			//	display.drawString(*CO2String,0,32);
			//	display.drawString(*bmeString,0,40);
			//	display.drawString(*resistanceString,0,48);
			//	display.drawString(*voltageString,0,8);
				display.refresh();

			//	delete humidityString;
				delete temperatureString;
			//	delete pressureString;
				delete TVOCString;
				delete CO2String;
				delete resistanceString;
				delete weatherString;
				delete voltageString;
				delete bmeString;
				delete sensorTypeString;

		}



	}
}

