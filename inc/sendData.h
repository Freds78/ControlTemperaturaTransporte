/*
 * sendData.h
 *
 *  Created on: 22 feb. 2021
 *      Author: freds
 */

#ifndef MYPROGRAM_SENSORTEMPERATURA_INC_SENDDATA_H_
#define MYPROGRAM_SENSORTEMPERATURA_INC_SENDDATA_H_

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "sapi.h"

/**********Delimitador de inicio ***************/

#define START_DELIMITER 0x7E

/********** Identificador API ******************/

#define TRANSMIT_REQUEST 0x10
#define TRANSMIT_REQUEST_TALL  1

/*******Indicador de trama*******/

#define FRAMEID 0x00 // Habilita trama de respuesta, 0 desabilita
#define FRAMEID_TALL 1

/*******Direccion destino*******/

#define DESTINATION_ADRESS64 0x0000000000000000
#define DESTINATION_ADRESS16 0xFFFE
#define DESTINATION_ADRESS64_TALL 8
#define DESTINATION_ADRESS16_TALL 2

/*******Broad Radius*******/

#define BROADCAST_RADIUS 0x00
#define BROADCAST_RADIUS_TALL 1

/*******Opciones de transmision*******/

#define OPTION 0x00
#define OPTION_TALL 1


/*=====[Definition macros of public constants]===============================*/
#define counterTx 50
#define counterDt 30
#define place_checksum 3
#define byte_Nocounter 4
/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

typedef struct{
	uint8_t DataTransmit[counterTx];
	uint8_t ValRFData[counterDt];
	uint8_t counterData;
	uint8_t counterDataTx;
}cmdDataTrans_t;


typedef struct{
	cmdDataTrans_t dataTrans;
	delay_t delay;
}packet_t;



/*=====[Prototypes (declarations) of public interrupt functions]=============*/
void XBEE_TX(packet_t *pack, float Buffer1, float Buffer2, float Buffer3);




#endif /* MYPROGRAM_SENSORTEMPERATURA_INC_SENDDATA_H_ */
