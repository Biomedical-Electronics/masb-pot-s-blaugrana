/*
 * adc.c
 *
 *  Created on: May 13, 2021
 *      Author: Raimonn
 */

#include "components/adc.h"

extern ADC_HandleTypeDef hadc1;
uint32_t Vadc=0;
extern TIM_HandleTypeDef htim3;
uint32_t prescaler=8399; //reduim freq del rellotge a 10KHz

struct Data_S ADC_measure(uint32_t count, uint32_t samplingPeriod){ //
	HAL_ADC_Start(&hadc1);

	HAL_ADC_PollForConversion(&hadc1, 100);
	Vadc=HAL_ADC_GetValue(&hadc1); //conversion tenint en compte (voltatge referencia/4096) ja que opera a 12 bits

	double Vcell= ((1.65-Vadc)*2); //formula per Vcell
	double Icell= ((Vadc-1.65)*2)/Rtia; //formula per Icell

	struct Data_S dataPoint; //guardem les mesures per cada count
	dataPoint.point=count;
	dataPoint.timeMs= count*samplingPeriod;
	dataPoint.voltage=Vcell;
	dataPoint.current=Icell;

	return dataPoint;

}

void ClockSettings (uint32_t samplingPeriod){
	uint16_t ticks= (84e6/prescaler)*(samplingPeriod/1000); //samplingPeriod esta en ms
	//84e6 es la frecuencia del internal clock del tim3

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = prescaler;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period=ticks;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	HAL_TIM_Base_Start_IT(&htim3); //inicialitzem timer

}








