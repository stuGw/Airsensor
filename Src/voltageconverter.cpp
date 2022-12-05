/*
 * voltageconverter.cpp
 *
 *  Created on: Jun 8, 2022
 *      Author: vit
 */

#include "voltageconverter.h"

VoltageConverter::VoltageConverter(uint32_t R1Value, uint32_t R2Value, uint16_t maxResolutionValue, float maxResolutionVoltage)

	{
		R1 = R1Value;
		R2 = R2Value;
		maxResValue = maxResolutionValue;
		maxResVoltage = maxResolutionVoltage;
		voltageDiscrete = maxResVoltage/static_cast<float>(maxResValue);
		RCoef = R1/R2 + 1.0;
	}

VoltageConverter::~VoltageConverter() {
	// TODO Auto-generated destructor stub
}

