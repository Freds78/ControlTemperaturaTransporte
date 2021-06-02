/*=============================================================================
 * Author: Freds Amundaray Cruz <freds.amundaray@gmail.com>
 * Date: 2021/01/13
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "Control_data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CardSD.h"
#include "sim900.h"
#include "sapi.h"


/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

static uint8_t DataReceiveRx[DATARECEIVE_RX_SIZE];
uint8_t indexBufferRx;
uint8_t counterpass;
uint8_t counterjump;
uint8_t counterSD;
uint8_t counterGSM;
static rtc_t rtc;

/*=====[Main function, program entry point after power on or reset]==========*/

// start delimiter , length , frame type ,  adress 64 , addres 16 , receive_option , RF_data, checksum

static void data_separation(package_t *sec);
static void Escaped_Characters_Adequacy(package_t *sec);
static bool_t Package_ChecksumRx(package_t *sec);
static float stringToFloat(char *string);



static float stringToFloat(char *string){

    float result = 0.0;
    int len = strlen(string);
    int dotPosition = 0;

    for (int i = 0; i < len; i++)
    {
        if (string[i] == '.')
        {
          dotPosition = len - i  - 1;
        }
        else
        {
          result = result * 10.0 + (string[i]-'0');
        }
      }

      while (dotPosition--)
      {
        result /= 10.0;
      }

    return result;
}

static void data_separation(package_t *sec){

	uint8_t i,j,k,l,m,c,s;

	j = 0;
	c = 0;
	l = 0;
	m = 0;
	s = 0;

	// Pxx,xSxx,xTxx,x

	for( i = 15; i < ((sec->DataXbee[1] + sec->DataXbee[2] + 3)); i++){
		sec->RFData[j] = sec->DataXbee[i];
		j++;
	}
	//printf( "\n%s\t\n", sec->RFData );

	for(k = 0 ; k < j ; k++){

		if(sec->RFData[k] != 'S' && c == 1){
			sec->Sensor_Value1[k -l] = sec->RFData[k];
		}else{
			c = 0;
			l++;
		}
		if(sec->RFData[k] == 'P'){
			c = 1;
		}

	}
	//printf( "%s\t\n", sec->Sensor_Value1 );

	for(k = 0 ; k < j ; k++){

		if(sec->RFData[k] != 'T' && c == 1){
			sec->Sensor_Value2[k-s] = sec->RFData[k];
		}else{
			c = 0;
			s++;
		}
		if(sec->RFData[k] == 'S'){
			c = 1;
		}

	}
	//printf( "%s\t\n", sec->Sensor_Value2 );

	for(k = 0 ; k < j ; k++){

		if(sec->RFData[k] != 'T' && c == 1){
			sec->Sensor_Value3[k - m] = sec->RFData[k];
		}else{
			c = 0;
			m++;
		}
		if(sec->RFData[k] == 'T'){
			c = 1;
		}

	}
	//printf( "%s\t\n", sec->Sensor_Value3);

}


static void Escaped_Characters_Adequacy(package_t *sec){

	uint8_t i, j, counter;

	counter = 0;

	for(i = 0; i <  counterjump; i++){
		sec->DataXbee[i] = DataReceiveRx[i];
	}

	for(i = 0; i < counterjump; i++){

		if(sec->DataXbee[i] == 0x7D ){
			i++;
			counter++;
			if(sec->DataXbee[i] == 0x5D){
				i--;
				sec->DataXbee[i] = 0x7D;
			}
			if(sec->DataXbee[i] == 0x31){
				i--;
				sec->DataXbee[i] = 0x11;
			}
			if(sec->DataXbee[i] == 0x33){
				i--;
				sec->DataXbee[i] = 0x13;
			}
			if(sec->DataXbee[i] == 0x5E){
				i--;
				sec->DataXbee[i] = 0x7E;
			}
		}

		if((sec->DataXbee[i] == 0x5D || sec->DataXbee[i] == 0x31 ||
				sec->DataXbee[i] == 0x33 || sec->DataXbee[i] == 0x5E)){
			i--;
			if(sec->DataXbee[i] == 0x7D || sec->DataXbee[i] == 0x11 ||
					sec->DataXbee[i] == 0x13 || sec->DataXbee[i] == 0x7E){
				i++;
				for(j = i; j < counterjump; j++){
					sec->DataXbee[j] = sec->DataXbee[j + 1];
				}
				i--;
			}else{
				i++;
			}
		}
	}

	for(i = counterjump - counter; i < counterjump; i++){

		sec->DataXbee[i] = '\0';
	}

}

static bool_t Package_ChecksumRx(package_t *sec){


	uint8_t i;
	uint16_t checkRsl, checkSm, checksumRx;
	bool_t checkVal;

	checkRsl = 0;
	checkSm = 0;
	checksumRx = 0;

	for(i = 0; i < (sec->DataXbee[1] + sec->DataXbee[2]); i++){
		checkSm += sec->DataXbee[i + 3];
	}

	checkRsl = checkSm & 0xFF;
	checksumRx = 0xFF - checkRsl;

	// validacion del checksum

	if(checksumRx == sec->DataXbee[sec->DataXbee[1] + sec->DataXbee[2] + 3]){
		checkVal = TRUE;
	}else{
		checkVal = FALSE;
	}
	return checkVal;
}


void onRx( void *noUsado){

	uint8_t i,j;

	char c = uartRxRead( UART_232 );

	DataReceiveRx[indexBufferRx] = c;

	//uartWriteByte( UART_USB, DataReceiveRx[indexBufferRx] );


	if(DataReceiveRx[indexBufferRx] == 0x7E && counterpass == 1 ){

		counterjump = indexBufferRx;
		indexBufferRx = 0;
		counterpass = 0;
		DataReceiveRx[indexBufferRx] = c;
	}

	indexBufferRx++;

	if(indexBufferRx == 1 ){
		counterpass = 1;
		counterSD = 1;
	}

}



void Control_init(package_t *sec){

	uint8_t i;
	sec->mode = ESCAPED_CHARACTERS;
	indexBufferRx = 0;
	counterpass = 0;
	counterjump = 0;
	counterSD = 0;
	counterGSM = 0;
	sec->counterCallMsg1 = 0;
	sec->counterCallMsg2 = 0;
	sec->counterCallMsg3 = 0;
	sec->data1 = 0.0;
	sec->data2 = 0.0;
	sec->data3 = 0.0;

	sec->counter_clear1 = 0;
	sec->counter_clear2 = 0;
	sec->counter_clear3 = 0;


	delayWrite(&sec->delay1sec,1000);

	for(i = 0; i < DATARECEIVE_RX_SIZE; i++){
		DataReceiveRx[i] = '\0';
	}

	for(i = 0; i < DATA_SIZE; i++){
		sec->DataXbee[i] = '\0';
	}

	for(i = 0; i < RFDATA_SIZE; i++){
		sec->RFData[i] = '\0';
	}

	for(i = 0; i < SENSOR_VALUE_SIZE; i++){
		sec->Sensor_Value1[i] = '\0';
	}

	for(i = 0; i < SENSOR_VALUE_SIZE; i++){
		sec->Sensor_Value2[i] = '\0';
	}

	for(i = 0; i < SENSOR_VALUE_SIZE; i++){
		sec->Sensor_Value3[i] = '\0';
	}

	rtcWrite( &rtc );									// Establecer fecha y hora
	delay(3000);

}


void Control_data(package_t *sec){




	switch(sec->mode){

	case ESCAPED_CHARACTERS:

		Escaped_Characters_Adequacy(sec);
		sec->mode = VALIDATE;
		break;

	case VALIDATE:

		if(Package_ChecksumRx(sec)){
			//printf("El paquete se recibio de forma correcta\n");
			sec->mode = ADECUACY;
		}else{
			//printf("El paquete no se recibio de forma correcta\n");
			sec->mode = ESCAPED_CHARACTERS;
		}
		break;

	case ADECUACY:										// adecuamos el dato

		data_separation(sec);
		sec->mode = RECORD;
		break;

	case RECORD:										// Guardamos el dato

		sec->data1 = stringToFloat(sec->Sensor_Value1);
		sec->data2 = stringToFloat(sec->Sensor_Value2);
		sec->data3 = stringToFloat(sec->Sensor_Value3);


		if(counterSD == TIMERECORD){
			writeRegisterGateway(sec->data1, sec->data2, sec->data3);
			printf("Sensor_1: %3.1f\t\n", sec->data1);
			printf("Sensor_2: %3.1f\t\n", sec->data2);
			printf("Sensor_3: %3.1f\t\n", sec->data3);

			printf("Guarda los datos en la tarjeta de memoria SD\n");
			counterGSM++;
			counterSD = 0;
		}

		sec->mode = TRANSMIT;

		break;

	case TRANSMIT:

		if(counterGSM == TIMESEND){
			printf("Transmite via GSM/GPRS desde el sim900 al servidor de ADOX\n");
			GPRS_Sim900(sec->data1,sec->data2,sec->data3);
			counterGSM = 0;
		}

		sec->mode = CALLSMS;

		break;

	case CALLSMS:

		if(delayRead(&sec->delay1sec)){
			if(sec->data1 > VALUE_MAX_S1){
				while(sec->counterCallMsg1 != VALUE_REPEAT){
					CALL_Sim900();
					MSG_S1_Sim900(sec->data1);
					turnOn(AMARILLO);
					printf("Llama y envia mensaje de la medicion del sensor 1\n");
					sec->counterCallMsg1++;
				}
				turnOn(AMARILLO);

				if(sec->counter_clear1 == TIMEDELAY){
					sec->counterCallMsg1 = 0;
					sec->counter_clear1 = 0;
				}

				sec->counter_clear1++;


			}else if(sec->data1 < VALUE_MIN_S1){
				while(sec->counterCallMsg1 != VALUE_REPEAT){
					CALL_Sim900();
					MSG_S1_Sim900(sec->data1);
					turnOn(AMARILLO);
					printf("Llama y envia mensaje de la medicion del sensor 1\n");
					sec->counterCallMsg1++;
				}
				turnOn(AMARILLO);

				if(sec->counter_clear1 == TIMEDELAY){
					sec->counterCallMsg1 = 0;
					sec->counter_clear1 = 0;
				}

				sec->counter_clear1++;


			}else{
				sec->counterCallMsg1 = 0;
				turnOff(AMARILLO);
			}

			if(sec->data2 > VALUE_MAX_S2){
				while(sec->counterCallMsg2 != VALUE_REPEAT){
					CALL_Sim900();
					MSG_S2_Sim900(sec->data2);
					turnOn(ROJO);
					printf("Llama y envia mensaje de la medicion del sensor 2\n");
					sec->counterCallMsg2++;
				}
				turnOn(ROJO);

				if(sec->counter_clear2 == TIMEDELAY){
					sec->counterCallMsg2 = 0;
					sec->counter_clear2 = 0;
				}

				sec->counter_clear2++;


			}else if( sec->data2 < VALUE_MIN_S2){
				while(sec->counterCallMsg2 != VALUE_REPEAT){
					CALL_Sim900();
					MSG_S2_Sim900(sec->data2);
					turnOn(ROJO);
					printf("Llama y envia mensaje de la medicion del sensor 2\n");
					sec->counterCallMsg2++;
				}
				turnOn(ROJO);

				if(sec->counter_clear2 == TIMEDELAY){
					sec->counterCallMsg2 = 0;
					sec->counter_clear2 = 0;
				}

				sec->counter_clear2++;

			}else{
				sec->counterCallMsg2 = 0;
				turnOff(ROJO);
			}

			if(sec->data3 > VALUE_MAX_S3){
				while(sec->counterCallMsg3 != VALUE_REPEAT){
					CALL_Sim900();
					MSG_S3_Sim900(sec->data3);
					sec->counterCallMsg3++;
					turnOn(VERDE);
					printf("Llama y envia mensaje de la medicion del sensor 3\n");
				}
				turnOn(VERDE);

				if(sec->counter_clear3 == TIMEDELAY){
					sec->counterCallMsg3 = 0;
					sec->counter_clear3 = 0;
				}

				sec->counter_clear3++;

			}else if(sec->data3 < VALUE_MIN_S3){
				while(sec->counterCallMsg3 != VALUE_REPEAT){
					CALL_Sim900();
					MSG_S3_Sim900(sec->data3);
					sec->counterCallMsg3++;
					turnOn(VERDE);
					printf("Llama y envia mensaje de la medicion del sensor 3\n");
				}
				turnOn(VERDE);

				if(sec->counter_clear3 == TIMEDELAY){
					sec->counterCallMsg3 = 0;
					sec->counter_clear3 = 0;
				}

				sec->counter_clear3++;

			}else{
				sec->counterCallMsg3 = 0;
				turnOff(VERDE);
			}

		}

		sec->mode = ESCAPED_CHARACTERS;
		break;

	default:

		sec->mode = ESCAPED_CHARACTERS;
		break;

	}

}
