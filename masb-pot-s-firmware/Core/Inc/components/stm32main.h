/*
 * stm32main.h
 *
 *  Created on: May 15, 2021
 *      Author: julia
 */

#ifndef INC_COMPONENTS_STM32MAIN_H_
#define INC_COMPONENTS_STM32MAIN_H_

#include "stm32f4xx_hal.h"
#include "components/masb_comm_s.h"
#include "components/chronoamperometry.h"
#include "components/cyclic_voltammetry.h"
#include "components/mcp4725_driver.h"
#include "components/adc.h"
//#include "components/dac.h"

struct Handles_S {
	UART_HandleTypeDef *huart2;
	ADC_HandleTypeDef *hadc1;
	I2C_HandleTypeDef *hi2c;
	TIM_HandleTypeDef htim3;
	// Aqui iriamos anadiendo los diferentes XXX_HandleTypeDef que necesitaramos anadir.
};

// Prototypes
void setup(struct Handles_S *handles);
void loop(void);
void interrupt(void);

#endif /* INC_COMPONENTS_STM32MAIN_H_ */
