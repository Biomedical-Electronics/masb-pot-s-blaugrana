/**
  ******************************************************************************
  * @file		chronoamperometry.c
  * @brief		Gestión de la cronoamperometría.
  * @author		Albert Álvarez Carulla
  * @copyright	Copyright 2020 Albert Álvarez Carulla. All rights reserved.
  ******************************************************************************
  */

#include "components/chronoamperometry.h"

// caConfiguration=MASB_COMM_S_getCaConfiguration(void)

void ChronoAmperometry(struct CA_Configuration_S caConfiguration){
	double eDC= caConfiguration.eDC;

	//alguna funcio per fixar el voltatge mitjançant el DAC

	//tanquem rele HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1(reset);

	uint32_t samplingPeriod=caConfiguration.samplingPeriodMs; //temps entre mostra i mostra
	uint32_t mTime=caConfiguration.measurementTime; //durada de la crono

	//algo per saber les mesures totals

	//while mesures
}

