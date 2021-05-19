/*
 * Record.h
 *
 *  Created on: 23 feb. 2021
 *      Author: freds
 */

#ifndef MYPROGRAM_CONTROL_DATA_INC_RECORD_H_
#define MYPROGRAM_CONTROL_DATA_INC_RECORD_H_


#include "sapi.h"
#include <stdint.h>
#include <stdbool.h>
#include "ff.h"       	// Biblioteca FAT FS
#include "fssdc.h"
#include "Leds.h"

#define FILENAME1 "SDC:/Sensor_1.txt"
#define FILENAME2 "SDC:/Sensor_2.txt"
#define FILENAME3 "SDC:/Sensor_3.txt"

static rtc_t rtc = {
   2021,			// año
   05,				// mes
   11,				// dia
   4,				// semana lunes martes etc
   23,				// hora
   40,				// minutos
   0				// segundos
};



void writeRegister(float Buffer1, float Buffer2, float Buffer3);
void diskTickHook( void *ptr );




#endif /* MYPROGRAM_CONTROL_DATA_INC_RECORD_H_ */
