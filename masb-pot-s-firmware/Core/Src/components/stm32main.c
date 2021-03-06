/*
 * stm32main.c
 *
 *  Created on: May 15, 2021
 *      Author: julia
 */

#include "components/stm32main.h"
#include "components/masb_comm_s.h"



volatile struct CV_Configuration_S cvConfiguration;
struct Data_S data;
volatile struct CA_Configuration_S caConfiguration;
volatile uint8_t state;
uint8_t count = 0;

extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim3;

extern uint32_t samplingPeriod;
extern uint32_t frequency;
volatile _Bool timeElapsed = FALSE;
//extern frequency;


void setup(void) { //pasarle el puntero con la configuración UART, facilidad para cambiar de perifericos
	//MASB_COMM_S_setUart(handles->huart2);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1);
	HAL_Delay(500);

	setup_DAC();
	I2C_init(&hi2c1);

	AD5280_Handle_T hpot = NULL;

	hpot = AD5280_Init();

	// Configuramos su direccion I2C de esclavo, su resistencia total (hay
	// diferentes modelos; este tiene 50kohms) e indicamos que funcion queremos que
	// se encargue de la escritura a traves del I2C. Utilizaremos la funcion
	// I2C_Write de la libreria i2c_lib.
	AD5280_ConfigSlaveAddress(hpot, 0x2C);
	AD5280_ConfigNominalResistorValue(hpot, 50e3f);
	AD5280_ConfigWriteFunction(hpot, I2C_write);

	// Fijamos la resistencia de, por ejemplo, 12kohms.
	AD5280_SetWBResistance(hpot, 50e3f);


	MASB_COMM_S_waitForMessage();



}

void loop(void) {
	    if (MASB_COMM_S_dataReceived()) { // Si se ha recibido un mensaje...

	 		switch(MASB_COMM_S_command()) { // Miramos que comando hemos recibido

	 			case START_CV_MEAS: // Si hemos recibido START_CV_MEAS

	                 // Leemos la configuracion que se nos ha enviado en el mensaje y
	                 // la guardamos en la variable cvConfiguration
					cvConfiguration = MASB_COMM_S_getCvConfiguration();

	 				/* Mensaje a enviar desde CoolTerm para hacer comprobacion
	 				 * eBegin = 0.25 V
	 				 * eVertex1 = 0.5 V
	 				 * eVertex2 = -0.5 V
	 				 * cycles = 2
	 				 * scanRate = 0.01 V/s
	 				 * eStep = 0.005 V
	 				 *
	 				 * Mensaje previo a la codificacion (lo que teneis que poder obtener en el microcontrolador):
	 				 * 01000000000000D03F000000000000E03F000000000000E0BF027B14AE47E17A843F7B14AE47E17A743F
	 				 *
	 				 * Mensaje codificado que enviamos desde CoolTerm (incluye ya el termchar):
	 				 * 0201010101010103D03F010101010103E03F010101010114E0BF027B14AE47E17A843F7B14AE47E17A743F00
	 				 */

	 				__NOP(); // Esta instruccion no hace nada y solo sirve para poder anadir un breakpoint

	 				CyclicVoltammetry(cvConfiguration);
	 				break;
	 				// Aqui iria todo el codigo de gestion de la medicion que hareis en el proyecto
	                // si no quereis implementar el comando de stop.


	 				break;

	 			case START_CA_MEAS: // Si hemos recibido START_CA_MEAS
	 				caConfiguration = MASB_COMM_S_getCaConfiguration();

	 				// Leemos la configuracion que se nos ha enviado en el mensaje y
	                 // la guardamos en la variable caConfiguration
	 				/* Mensaje a enviar desde CoolTerm para hacer comprobacion
	 				 * eDC = 0.3 V
	 				 * samplingPeriodMs = 10 ms
	 				 * measurementTime = 120 s
	 				 *
	 				 * Mensaje previo a la codificacion (lo que teneis que poder obtener en el microcontrolador):
	 				 * 02333333333333D33F0A00000078000000
	 				 *
	 				 * Mensaje codificado que enviamos desde CoolTerm (incluye ya el termchar):
	 				 * 0B02333333333333D33F0A0101027801010100
	 				 */
	 				__NOP();

	 				ChronoAmperometry(caConfiguration);
	 				break;


				case STOP_MEAS: // Si hemos recibido STOP_MEAS

	 				/*
	 				 * Debemos de enviar esto desde CoolTerm:
	 				 * 020300
	 				 */
	 				__NOP(); // Esta instruccion no hace nada y solo sirve para poder anadir un breakpoint

	 				// Aqui iria el codigo para tener la medicion si implementais el comando stop.

	 				break;

	 			default: // En el caso que se envia un comando que no exista

	                 // Este bloque de codigo solo lo tenemos para hacer el test de que podemos enviar los
	                 // datos correctamene. EN EL PROYECTO FINAL TENEMOS QUE ELIMINAR ESTE CODIGO y pondremos
	                 // lo que se nos indique en los requerimientos del proyecto. Repito, es solo para
	                 // comprobar que podemos enviar datos del microcontrolador al PC.

	 				//data.point = 1;
	 				//data.timeMs = 100;
	 				//data.voltage = 0.23;
	 				//data.current = 12.3e-6;

	 				/*
	 				 * Debemos de enviar esto desde CoolTerm (un comando inventado):
	 				 * 010100
	 				 *
	 				 * Datos en el microcontrolador antes de la codificacion:
	 				 * 0100000064000000713D0AD7A370CD3F7050B12083CBE93E
	 				 *
	 				 * Datos codificados que debes de recibir en CoolTerm:
	 				 * 020101010264010111713D0AD7A370CD3F7050B12083CBE93E00
	 				 */

	                // Enviamos los datos
	 				//MASB_COMM_S_sendData(data);

	 				break;

	 		}

	       // Una vez procesado los comando, esperamos el siguiente
	 		MASB_COMM_S_waitForMessage();

	 	}

	 	// Aqui es donde deberia de ir el codigo de control de las mediciones si se quiere implementar
	   // el comando de STOP.

	}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	timeElapsed = TRUE;

	__NOP();

	if (state==CA){
		struct Data_S data = ADC_measure(count,samplingPeriod);
		MASB_COMM_S_sendData(data);
		count++;
	}

	if (state==CV){
		struct Data_S data = ADC_measure(count,frequency);
		MASB_COMM_S_sendData(data);
		count++;
	}
}
