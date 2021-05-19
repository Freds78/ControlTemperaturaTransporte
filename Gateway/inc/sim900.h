/*
 * sim900.h
 *
 *  Created on: 24 mar. 2021
 *      Author: freds
 */
#include "sapi.h"
#include <string.h>
#include <stdio.h>

#ifndef MYPROGRAM_CONTROL_DATA_INC_SIM900_H_
#define MYPROGRAM_CONTROL_DATA_INC_SIM900_H_

#define BAUD_RATE 9600 // Baudrate para colocar al sim900
#define RX_SIZE	10
#define SIZE_MSM 35
#define SIZE_CALL 25

void GPRS_Sim900(float Temperatura1, float Temperatura2, float Temperatura3 );
void CALL_Sim900();
void MSG_S1_Sim900(float Temperatura);
void MSG_S2_Sim900(float Temperatura);
void MSG_S3_Sim900(float Temperatura);


#endif /* MYPROGRAM_CONTROL_DATA_INC_SIM900_H_ */
