/*
 * adc.c
 *
 *  Created on: May 13, 2021
 *      Author: Raimonn
 */

#include "components/adc.h"
#include "components/formulas.h"

uint32_t samplingPeriod;
extern ADC_HandleTypeDef hadc1; //ADC
uint32_t Vadc=0;
extern TIM_HandleTypeDef htim3; //timer 
uint32_t prescaler=8399; //reduim freq del rellotge a 10KHz, no ho utilitzem al final

struct Data_S ADC_measure(uint32_t count, uint32_t samplingPeriod){
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100); //wait
	uint32_t VadcValue = HAL_ADC_GetValue(&hadc1); //ADC value

	double Vcell= calculateVrefVoltage(VadcValue); //conversion tenint en compte (voltatge referencia/4096) ja que opera a 12 bits

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100); //wait
	VadcValue = HAL_ADC_GetValue(&hadc1); //ADC value

	double Icell = calculateIcellCurrent(VadcValue); //formula per Icell

	struct Data_S dataPoint; //guardem les mesures per cada count i temps
	dataPoint.point=count;
	dataPoint.timeMs= count*samplingPeriod;
	dataPoint.voltage=Vcell;
	dataPoint.current=Icell;

	return dataPoint;

}

void ClockSettings (uint32_t samplingPeriod){
	uint16_t ticks= samplingPeriod*10; //samplingPeriod esta en ms
	//84e6 es la frecuencia del internal clock del tim3
/*
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = prescaler;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period=ticks;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;*/

	__HAL_TIM_SET_AUTORELOAD(&htim3, ticks);
	__HAL_TIM_SET_COUNTER(&htim3, 0);



	HAL_TIM_Base_Start_IT(&htim3); //inicialitzem timer

}








