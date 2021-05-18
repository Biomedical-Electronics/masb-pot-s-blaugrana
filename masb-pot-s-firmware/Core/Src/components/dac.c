/*
 * dac.c
 *
 *  Created on: 15 de maig 2021
 *      Author: julia
 */

#include <components/dac.h>

MCP4725_Handle_T hdac = NULL;



void setup_DAC(MCP4725_Handle_T *newhdac) {
	hdac = newhdac;
	hdac = MCP4725_Init();
	MCP4725_ConfigSlaveAddress(hdac, 0x66); // DIRECCION DEL ESCLAVO
	MCP4725_ConfigVoltageReference(hdac, 4.0f); // TENSION DE REFERENCIA
	MCP4725_ConfigWriteFunction(hdac, I2C_Write); // FUNCION DE ESCRITURA (libreria I2C_lib)
}

void sendVoltage(MCP4725_Handle_T *newhdac, float voltage) {
	hdac = newhdac;
	MCP4725_SetOutputVoltage(hdac, voltage); // NUEVA TENSION
}








