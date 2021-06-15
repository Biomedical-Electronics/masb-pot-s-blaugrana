/*
 * dac.h
 *
 *  Created on: 15 de maig 2021
 *      Author: julia
 */

#include "components/mcp4725_driver.h"
#include "components/i2c_lib.h"

#ifndef INC_COMPONENTS_DAC_H_
#define INC_COMPONENTS_DAC_H_

#define MCP4725 0X60 // address of the DAC

void setup_DAC(void);
void sendVoltage(float voltage);


#endif /* INC_COMPONENTS_DAC_H_ */
