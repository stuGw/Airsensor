/*
 * voltageconverter.h
 *
 *  Created on: Jun 8, 2022
 *      Author: vit
 */

#ifndef VOLTAGECONVERTER_H_
#define VOLTAGECONVERTER_H_
#include <stdint.h>
//Calculate value from ADC with R1, R2 divider

class VoltageConverter {
	float R1 { 10000 };
	float R2 { 10000 };

	float RCoef { 5001 };

	uint16_t maxResValue { 4095 };
	float maxResVoltage { 3.3 };

	float voltageDiscrete { 0.0008059 };

public:
	VoltageConverter(uint32_t R1Value, uint32_t R2Value, uint16_t maxResolutionValue, float maxResolutionVoltage);
	virtual ~VoltageConverter();
	float getVoltage(uint16_t inVoltage)
	{
		return ((static_cast<float>(inVoltage))*voltageDiscrete)*RCoef;
	}
};

#endif /* VOLTAGECONVERTER_H_ */
