/*=============================================================================
 * Author: Freds Amundaray Cruz <freds.amundaray@gmail.com>
 * Date: 2020/04/28
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sensorTemp.h"
#include "sapi.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "sendData.h"
#include "Record.h"

/*=====[Definition macros of private constants]==============================*/

#define AMARILLO LED1
#define ROJO LED2
#define VERDE LED3
#define SKIP_ROMCOM 0xCC
#define CONVERT_TEMP 0x44
#define READ_SCRATCH 0xBE
#define SEARCH_ROM 0xF0
#define MATCH_ROM 0x55
#define VALUE_LOW_S1 15
#define VALUE_HIGH_S1 20
#define VALUE_LOW_S2 12
#define VALUE_HIGH_S2 25
#define VALUE_LOW_S3 10
#define VALUE_HIGH_S3 28

/*=====[Definitions of extern global variables]==============================*/


static float Buffer1;
static float Buffer2;
static float Buffer3;
static uint8_t scratchSensor1[9];
static uint8_t scratchSensor2[9];
static uint8_t scratchSensor3[9];

static rtc_t rtc;
packet_t pack;
uint8_t counter, counter_record, counter_Transfer;

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/


static void skip_Rom(uint8_t n);
static bool_t Read_Scratchpad(uint8_t n);
static uint8_t crc8_scrat(uint8_t n);
static bool_t initialize_secuence(uint8_t n);
static void send_bit(uint8_t bit_data,uint8_t n);
static uint8_t read_bit(uint8_t n);
static void send_byte(uint8_t data, uint8_t n);
static uint8_t read_byte(uint8_t n);
static void Reading_Temperature(uint8_t n);
static void Control_Temperature(uint8_t n);
static void Conver_Temp(uint8_t n);



static void Conver_Temp(uint8_t n){

	delay_t delay;

	if(n == 1){
		gpioConfig( GPIO3, GPIO_INPUT );
		while(gpioRead( GPIO3 ) == 0){
			delayWrite(&delay,750);
			if(delayRead(&delay)){
			}
			//printf("espera que llegue la confirmación del sensor 1\n");
		}
	}
	if(n == 2){
		gpioConfig( GPIO4, GPIO_INPUT );
		while(gpioRead( GPIO4 ) == 0){
			delayWrite(&delay,750);
			if(delayRead(&delay)){
			}
			//printf("espera que llegue la confirmación del sensor 2\n");
		}
	}
	if(n == 3){
		gpioConfig( GPIO5, GPIO_INPUT );
		while(gpioRead( GPIO5 ) == 0){
			delayWrite(&delay,750);
			if(delayRead(&delay)){
			}
			//printf("espera que llegue la confirmación del sensor 3\n");
		}
	}

}

static void skip_Rom(uint8_t n){

	send_byte(SKIP_ROMCOM, n );
}

static bool_t Read_Scratchpad(uint8_t n){

	int8_t i;
	uint8_t var_aux;

	bool_t result;

	var_aux = 0;

	if(n == 1){
		for (i = 0; i < 9; i++) {

			scratchSensor1[i] = read_byte(n);

			if(i == 8){

				var_aux = crc8_scrat(n);

				if(var_aux == scratchSensor1[i]){

					result = TRUE;

				}else{

					result = FALSE;
					initialize_secuence(n);
				}
			}
		}

	}


	if(n == 2){
		for (i = 0; i < 9; i++) {

			scratchSensor2[i] = read_byte(n);

			if(i == 8){

				var_aux = crc8_scrat(n);

				if(var_aux == scratchSensor2[i]){

					result = TRUE;

				}else{

					result = FALSE;
					initialize_secuence(n);
				}
			}
		}

	}

	if(n == 3){
		for (i = 0; i < 9; i++) {

			scratchSensor3[i] = read_byte(n);

			if(i == 8){

				var_aux = crc8_scrat(n);

				if(var_aux == scratchSensor3[i]){

					result = TRUE;

				}else{

					result = FALSE;
					initialize_secuence(n);
				}
			}
		}

	}
	return result;
}


static uint8_t crc8_scrat (uint8_t n) {

	uint8_t div = 0b10001100; 							// Rotated poly
	uint8_t crc = 0;
	int8_t i;

	if(n == 1){

		for (i = 0; i < 8; i++) {

			uint8_t byte = scratchSensor1[i];
			int8_t j;

			for (j = 0; j < 8; j++) {

				uint8_t crc_carry = crc & 1;
				crc = crc >> 1;
				uint8_t byte_carry = byte & 1;
				byte = byte >>1;

				if (crc_carry ^ byte_carry){
					crc ^= div;
				}
			}
		}

	}

	if(n == 2){

		for (i = 0; i < 8; i++) {

			uint8_t byte = scratchSensor2[i];
			int8_t j;

			for (j = 0; j < 8; j++) {

				uint8_t crc_carry = crc & 1;
				crc = crc >> 1;
				uint8_t byte_carry = byte & 1;
				byte = byte >>1;

				if (crc_carry ^ byte_carry){
					crc ^= div;
				}
			}
		}

	}

	if(n == 3){

		for (i = 0; i < 8; i++) {

			uint8_t byte = scratchSensor3[i];
			int8_t j;

			for (j = 0; j < 8; j++) {

				uint8_t crc_carry = crc & 1;
				crc = crc >> 1;
				uint8_t byte_carry = byte & 1;
				byte = byte >>1;

				if (crc_carry ^ byte_carry){
					crc ^= div;
				}
			}
		}

	}
	return crc;

}



static void send_byte(uint8_t data, uint8_t n) {		//Sends 8 bit in a row, LSB first

	int8_t i;

	for (i = 0; i < 8; i++) {
		send_bit(data & 0x01, n);
		data = data >> 1;
	}
}


static void send_bit(uint8_t bit_data, uint8_t n) {


	if(n == 1){
		gpioConfig( GPIO3, GPIO_OUTPUT );
		gpioWrite( GPIO3, 0 );
		delayInaccurateUs( 2 );
		if (bit_data) {
			gpioWrite( GPIO3, 1 );
			delayInaccurateUs( 60 );
		} else {
			delayInaccurateUs( 60 );
			gpioWrite( GPIO3, 1 );
			delayInaccurateUs( 10 );
		}
	}

	if(n == 2){
		gpioConfig( GPIO4, GPIO_OUTPUT );
		gpioWrite( GPIO4, 0 );
		delayInaccurateUs( 2 );
		if (bit_data) {
			gpioWrite( GPIO4, 1 );
			delayInaccurateUs( 60 );
		} else {
			delayInaccurateUs( 60 );
			gpioWrite( GPIO4, 1 );
			delayInaccurateUs( 10 );
		}
	}

	if(n == 3){
		gpioConfig( GPIO5, GPIO_OUTPUT );
		gpioWrite( GPIO5, 0 );
		delayInaccurateUs( 2 );
		if (bit_data) {
			gpioWrite( GPIO5, 1 );
			delayInaccurateUs( 60 );
		} else {
			delayInaccurateUs( 60 );
			gpioWrite( GPIO5, 1 );
			delayInaccurateUs( 10 );
		}
	}

}


static uint8_t read_byte(uint8_t n) {					//Reads a byte, LSB first
	uint8_t byte = 0;
	int8_t i;
	uint8_t j;

	if(n == 1){
		for (i=0; i < 8; i++) {
			j = read_bit(n);
			j = j << i;
			byte = byte | j;
		}

	}

	if(n == 2){
		for (i=0; i < 8; i++) {
			j = read_bit(n);
			j = j << i;
			byte = byte | j;
		}

	}

	if(n == 3){
		for (i=0; i < 8; i++) {
			j = read_bit(n);
			j = j << i;
			byte = byte | j;
		}

	}

	return byte;
}

static uint8_t read_bit(uint8_t n) {

	uint8_t read;

	if(n == 1){
		gpioConfig( GPIO3, GPIO_OUTPUT );
		gpioWrite( GPIO3, 0 );
		delayInaccurateUs( 3 );
		gpioConfig( GPIO3, GPIO_INPUT );
		delayInaccurateUs( 10 );
		read = gpioRead( GPIO3 );
		delayInaccurateUs( 55 );
	}

	if(n == 2){
		gpioConfig( GPIO4, GPIO_OUTPUT );
		gpioWrite( GPIO4, 0 );
		delayInaccurateUs( 3 );
		gpioConfig( GPIO4, GPIO_INPUT );
		delayInaccurateUs( 10 );
		read = gpioRead( GPIO4 );
		delayInaccurateUs( 55 );
	}

	if(n == 3){
		gpioConfig( GPIO5, GPIO_OUTPUT );
		gpioWrite( GPIO5, 0 );
		delayInaccurateUs( 3 );
		gpioConfig( GPIO5, GPIO_INPUT );
		delayInaccurateUs( 10 );
		read = gpioRead( GPIO5 );
		delayInaccurateUs( 55 );
	}

	return read;
}

static bool_t initialize_secuence(uint8_t n){

	bool_t val;
	uint8_t bit;

	if(n == 1){
		gpioConfig( GPIO3, GPIO_OUTPUT );
		gpioWrite( GPIO3, 0 );
		delayInaccurateUs( 500 );
		gpioConfig( GPIO3, GPIO_INPUT );
		delayInaccurateUs( 60 );
		bit = gpioRead( GPIO3 );
		if (bit == 0){
			val = TRUE;
		}else{
			val = FALSE;
		}
		delayInaccurateUs( 410 );
	}

	if(n == 2){
			gpioConfig( GPIO4, GPIO_OUTPUT );
			gpioWrite( GPIO4, 0 );
			delayInaccurateUs( 500 );
			gpioConfig( GPIO4, GPIO_INPUT );
			delayInaccurateUs( 60 );
			bit = gpioRead( GPIO4 );
			if (bit == 0){
				val = TRUE;
			}else{
				val = FALSE;
			}
			delayInaccurateUs( 410 );
		}

	if(n == 3){
			gpioConfig( GPIO5, GPIO_OUTPUT );
			gpioWrite( GPIO5, 0 );
			delayInaccurateUs( 500 );
			gpioConfig( GPIO5, GPIO_INPUT );
			delayInaccurateUs( 60 );
			bit = gpioRead( GPIO5 );
			if (bit == 0){
				val = TRUE;
			}else{
				val = FALSE;
			}
			delayInaccurateUs( 410 );
		}

	return val;
}


static void Reading_Temperature(uint8_t n){

	float remaining_count, count_per_degree, answer;
	int reading;

	uint8_t dutyCycle = 0;

	if(n == 1){

		reading = (scratchSensor1[1] << 8) + scratchSensor1[0];

		if (reading & 0x8000) { 									// negative degrees C
			reading = 0-((reading ^ 0xffff) + 1); 					// 2's comp then convert to signed int
		}

		answer = reading +0.0; 										// convert to floating point
		answer = answer / 16.0f;
		printf("Temperatura sensor_1: %3.1fC\r\n", answer );

		Buffer1 = answer;
	}

	if(n == 2){

		reading = (scratchSensor2[1] << 8) + scratchSensor2[0];

		if (reading & 0x8000) { 									// negative degrees C
			reading = 0-((reading ^ 0xffff) + 1); 					// 2's comp then convert to signed int
		}

		answer = reading +0.0; 										// convert to floating point
		answer = answer / 16.0f;
		printf("Temperatura sensor_2: %3.1fC\r\n", answer );

		Buffer2 = answer;
	}

	if(n == 3){
		reading = (scratchSensor3[1] << 8) + scratchSensor3[0];

		if (reading & 0x8000) { 									// negative degrees C
			reading = 0-((reading ^ 0xffff) + 1); 					// 2's comp then convert to signed int
		}

		answer = reading +0.0; 										// convert to floating point
		answer = answer / 16.0f;
		printf("Temperatura sensor_3: %3.1fC\r\n", answer );

		Buffer3 = answer;

	}

}


static void Control_Temperature(uint8_t n){

	uint8_t dutyCycle = 0;

	if(n == 1){

		if(Buffer1 < VALUE_LOW_S1){
			turnOn(AMARILLO);
			dutyCycle = 200;
			pwmWrite( PWM0, dutyCycle );
			//printf("Se encuentra encendido el buzzer del sensor 1 por debajo del valor ajustado\r\n" );
		}else if(Buffer1 > VALUE_HIGH_S1){
			turnOn(AMARILLO);
			dutyCycle = 200;
			pwmWrite( PWM0, dutyCycle );
			//printf("Se encuentra encendido el buzzer del sensor 1 por encima del valor ajustado\r\n" );
		}else{
			turnOff(AMARILLO);
			dutyCycle = 0;
			pwmWrite( PWM0, dutyCycle );
			//printf("Se encuentra apago el buzzer del sensor 1\r\n" );
		}
	}

	if(n == 2){
		if(Buffer2 < VALUE_LOW_S2){
			turnOn(ROJO);
			dutyCycle = 200;
			pwmWrite( PWM0, dutyCycle );
			//printf("Se encuentra encendido el buzzer del sensor 2 por debajo del valor ajustado\r\n" );
		}else if(Buffer2 > VALUE_HIGH_S2){
			turnOn(ROJO);
			dutyCycle = 200;
			pwmWrite( PWM0, dutyCycle );
			//printf("Se encuentra encendido el buzzer del sensor 2 por encima del valor ajustado\r\n" );
		}else{
			turnOff(ROJO);
			dutyCycle = 0;
			pwmWrite( PWM0, dutyCycle );
			//printf("Se encuentra apago el buzzer del sensor 2\r\n" );
		}
	}

	if(n == 3){
		if(Buffer3 < VALUE_LOW_S3){
			turnOn(VERDE);
			dutyCycle = 200;
			pwmWrite( PWM0, dutyCycle );
			//printf("Se encuentra encendido el buzzer del sensor 3 por debajo del valor ajustado\r\n" );
		}else if(Buffer3 > VALUE_HIGH_S3){
			turnOn(VERDE);
			dutyCycle = 200;
			pwmWrite( PWM0, dutyCycle );
			//printf("Se encuentra encendido el buzzer del sensor 3 por encima del valor ajustado\r\n" );
		}else{
			turnOff(VERDE);
			dutyCycle = 0;
			pwmWrite( PWM0, dutyCycle );
			//printf("Se encuentra apago el buzzer del sensor 3\r\n" );
		}
	}


}

void sensorInit(conection_t *pcommand){

	rtcWrite( &rtc );									// Establecer fecha y hora
	delay(3000);
	pcommand->mode = INITIALIZATION;
	pcommand->contador_secuencia = 0;
	pcommand->contSensor = 1;
	counter_record = 0;
	counter_Transfer = 0;
	Buffer1 = 0;
	Buffer2 = 0;
	Buffer3 = 0;

 }

void masterTx_Control(conection_t *pcommand){

	switch(pcommand->mode){

	case INITIALIZATION:

		if(pcommand->contSensor != 0){
			if(initialize_secuence(pcommand->contSensor) == TRUE){
				pcommand->mode = SKIPROMCOM;
			}else{
				pcommand->mode = INITIALIZATION;
			}

		}

		break;

	case SKIPROMCOM:

		if(pcommand->contSensor != 0){
			skip_Rom(pcommand->contSensor);
		}

		if(pcommand->contador_secuencia  == 1){
			pcommand->mode = READSCRAT;
			pcommand->contador_secuencia = 0;
		}else{
			pcommand->mode = CONVERTTEMP;
			pcommand->contador_secuencia++;
		}
		break;

	case CONVERTTEMP:

		if(pcommand->contSensor != 0){
			send_byte(CONVERT_TEMP, pcommand->contSensor);
			Conver_Temp(pcommand->contSensor);
		}
		pcommand->mode = INITIALIZATION;
		break;

	case READSCRAT:

		send_byte(READ_SCRATCH, pcommand->contSensor);
		if(Read_Scratchpad(pcommand->contSensor)== TRUE){
			Reading_Temperature(pcommand->contSensor);
			Control_Temperature(pcommand->contSensor);

			delay(1000);

			if(counter_record == 60){
				//writeRegister(Buffer1, Buffer2, Buffer3);
				printf("Se guardo la temperatura\r\n" );
				counter_record = 0;
				counter_Transfer++;
			}else{
				counter_record++;
			}

			if(counter_Transfer == 5){

				XBEE_TX(&pack, Buffer1, Buffer2, Buffer3);
				printf("Se envio por radiofrecuencia la temperatura\r\n" );
				counter_Transfer = 0;
			}

		}

		pcommand->contSensor++;
		if(pcommand->contSensor == 4){
			pcommand->contSensor = 1;
		}

		pcommand->mode = INITIALIZATION;
		break;

	default:

		pcommand->mode = INITIALIZATION;
		break;

	}
}


