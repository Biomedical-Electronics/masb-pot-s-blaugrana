/**
  ******************************************************************************
  * @file		chronoamperometry.c
  * @brief		Gestión de la cronoamperometría.
  * @author		Albert Álvarez Carulla
  * @copyright	Copyright 2020 Albert Álvarez Carulla. All rights reserved.
  ******************************************************************************
  */

// Anadimos los archivos header necesarios para el programa
#include "components/chronoamperometry.h" // donde tenemos definida la crono
#include "components/masb_comm_s.h" // en este header se detallan los paquetes de comandos que inician y realizan las mediciones
#include "components/dac.h"
#include "components/adc.h"

// per configurar el voltatge de la cela

#include "components/mcp4725_driver.h"
#include "components/i2c_lib.h"

extern uint8_t count;
extern char state;
extern MCP4725_Handle_T hdac;

// caConfiguration=MASB_COMM_S_getCaConfiguration(void)

void ChronoAmperometry(struct CA_Configuration_S caConfiguration){

	double eDC= caConfiguration.eDC;

	// eDC es el voltatge constant de la cela electroquimica,
	// el fixem mitjançant la funcio seguent

	MCP4725_SetOutputVoltage(hdac, 1.65-eDC/2);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1); // (reset) Tanquem rele

	uint32_t samplingPeriod=caConfiguration.samplingPeriodMs; // temps entre mostra i mostra (ms)
	uint32_t mTime=caConfiguration.measurementTime; // durada de la crono (s)

	// (durada total de la crono) / (temps entre mostres) = quantes mostres hi ha.
	// COMPTE: Cal passar el Sampling period a segons

	uint8_t measures = round(mTime/(samplingPeriod/1000)); // nombre de mesures

	// CONFIGURAR EL TIMER

	ClockSettings(samplingPeriod);

	state = "CA";  // s'esta fent la cronoamperometria
	count = 1;  // s'ha fet la primera mesura

	while (count <= measures){  // mentre no arribem al nombre total de mesures
		if (count == measures){  // quan arribem a la mesura final
			state = "IDLE";  // no fa ni crono ni cv
		}

		state= "CA";
	}

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0); // OBRIM EL RELÉ
}
