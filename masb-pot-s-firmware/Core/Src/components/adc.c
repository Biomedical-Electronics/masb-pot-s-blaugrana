/*
 * adc.c
 *
 *  Created on: May 13, 2021
 *      Author: Raimonn
 */

#include "components/adc.h"

ADC_HandleTypeDef hadc1;
uint32_t Vadc=0;

struct Data_S ADC_measure(uint32_t count, double samplingPeriod){ //
	HAL_ADC_Start(&hadc1);

	HAL_ADC_PollForConversion(&hadc1, 100);
	Vadc=HAL_ADC_GetValue(&hadc1); //conversion tenint en compte (voltatge referencia/4096) ja que opera a 12 bits

	double Vcell= ((1.65-Vadc)*2);
	double Icell= ((Vadc-1.65)*2)/Rtia;

	struct Data_S dataPoint;
	dataPoint.point=count;
	dataPoint.timeMs= count*samplingPeriod;
	dataPoint.voltage=Vcell;
	dataPoint.current=Icell;

	return dataPoint;

}






