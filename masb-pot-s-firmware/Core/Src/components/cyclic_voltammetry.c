/**
  ******************************************************************************
  * @file		cyclic_voltammetry.c
  * @brief		Gestión de la voltammetría cíclica.
  * @author		Albert Álvarez Carulla
  * @copyright	Copyright 2020 Albert Álvarez Carulla. All rights reserved.
  ******************************************************************************
  */

#include "components/cyclic_voltammetry.h"
#include "components/masb_comm_s.h" // en este header se detallan los paquetes de comandos que inician y realizan las mediciones
#include "components/dac.h"
#include "components/adc.h"

// per configurar el voltatge de la cela

#include "components/mcp4725_driver.h"
#include "components/i2c_lib.h"

extern uint8_t count;
extern char state;
extern double Vcell;
extern MCP4725_Handle_T hdac;

void CyclicVoltammetry(struct CV_Configuration_S cvConfiguration){

	double ebegin= cvConfiguration.eBegin; // Voltaje inicial que se le asigna a la celda electroquimica
	MCP4725_SetOutputVoltage(hdac, 1.65-ebegin/2);
	double vObjetivo = cvConfiguration.eVertex1; // voltaje al que debe llegar la funcion (primer vertice)

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1); //  Cerramos el rele

	uint8_t cycles = cvConfiguration.cycles; // nombre total de ciclos/medidas de la voltimetria ciclica
	double scanRate = cvConfiguration.scanRate; // variación de la tensión de la celda electroquímica en el tiempo
	double eStep = cvConfiguration.eStep; // incremento/decremento de la tensión entre dos puntos consecutivos
	double frequency = scanRate/eStep; // tiempo entre muestras

	ClockSettings(frequency);

	uint8_t count = 1; // inicializamos la voltametria.
	state = "CV"; // mientras se este ejecutando una Voltametria, el estado sera CV

	while (count <= cycles){ // mientras no hayamos llegado al ultimo ciclo de la voltametria
		state = "CV";

		// Desde ADC llamamos a la variable Vcell, la cual comparamos com Vobjetivo
		// para ver si ha llegado al vertice 1, vertice 2 o eBegin.

		if (Vcell == vObjetivo){

			// ******************* Caso 1: que nos encontremos en el vertice 1  *****************

			if (vObjetivo == cvConfiguration.eVertex1){
				vObjetivo = cvConfiguration.eVertex2; //siguiente objetivo es el vertice 2

			}

			// ***************** Caso 2: que nos encontremos en el vertice 2 *****************
			else if (vObjetivo == cvConfiguration.eVertex2){
				vObjetivo = cvConfiguration.eBegin; //objetivo es volver a eBegin

			}

			// ***************** Caso 3: ultimo ciclo *****************
			else if (count == cycles){
				state= "IDLE";
			}

			// ***************** Caso 4 no es ultim cicle ni eVertex1, eVertex2 *****************
			else {
				vObjetivo = cvConfiguration.eVertex1; //siguiente objetivo es vertice 1

			}

		}

		else { // Cuando no hemos llegado al objetivo, sumamos o restamos un incremento hasta llegar

			// ***************** Para llegar al vertice 1 *****************

			if (vObjetivo == cvConfiguration.eVertex1){
				if ((Vcell + eStep) > vObjetivo){ // sumamos eStep y nos pasamos del objetivo, asi que fijamos el objetivo a vcell
					MCP4725_SetOutputVoltage(hdac, vObjetivo);
				}
				else { //si sumamos el eStep y no nos pasamos, definimos de nuevo el voltaje de la celda.
					MCP4725_SetOutputVoltage(hdac, Vcell+eStep);
				}
			}

			// ***************** Para llegar al vertice 2 *****************

			// El vertice dos se encuentra por debajo del vertice 1, así que tendremos que restar el eStep al vcell

			if (vObjetivo == cvConfiguration.eVertex2){ //
				if ((Vcell - eStep) < vObjetivo){ // Restamos eStep y nos pasamos del objetivo (demasiado pequeño), fijamos el vcell
					MCP4725_SetOutputVoltage(hdac, vObjetivo);
				}
				else { // si restamos eStep y no nos pasamos, definimos de nuevo el voltaje de la celda.
					MCP4725_SetOutputVoltage(hdac, Vcell-eStep);
				}
			}

			// ***************** Para llegar a eBegin *****************

			// El punto inicial se encuentra entre ambos vertices, por debajo del 1 y encima del 2

			if (vObjetivo == cvConfiguration.eBegin){
				if ((Vcell + eStep) > vObjetivo){ // sumamos eStep y nos pasamos del objetivo, asi que fijamos el obetivo a vcell
					MCP4725_SetOutputVoltage(hdac, vObjetivo);
				}
				else { //si sumamos el eStep y no nos pasamos, definimos de nuevo el voltaje de la celda.
					MCP4725_SetOutputVoltage(hdac, Vcell+eStep);
				}
			}

		}

		}

	}


