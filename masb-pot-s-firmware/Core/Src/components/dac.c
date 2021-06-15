/*
 * dac.c
 *
 *  Created on: 15 de maig 2021
 *      Author: julia
 */

#include <components/dac.h>
#include "components/formulas.h"

static MCP4725_Handle_T hdac = NULL;



void setup_DAC(void) {
	hdac = MCP4725_Init();
	MCP4725_ConfigSlaveAddress(hdac, 0x66); // DIRECCION DEL ESCLAVO
	MCP4725_ConfigVoltageReference(hdac, 4.0f); // TENSION DE REFERENCIA
	MCP4725_ConfigWriteFunction(hdac, I2C_write); // FUNCION DE ESCRITURA (libreria I2C_lib)
}

void sendVoltage(float voltage) {
	float vDac = calculateDacOutputVoltage(voltage);
	MCP4725_SetOutputVoltage(hdac, vDac); // NUEVA TENSION
}








