/*
 * adc.h
 *
 *  Created on: May 13, 2021
 *      Author: Raimonn
 */

#ifndef INC_COMPONENTS_ADC_H_
#define INC_COMPONENTS_ADC_H_

#include "stm32f4xx_hal.h"
#include "components/masb_comm_s.h"

#define Rtia	10000

//Prototypes of functions
struct Data_S ADC_measure(uint32_t count, double samplingPeriod);

#endif /* INC_COMPONENTS_ADC_H_ */
