/*
 * CardSD.c
 *
 *  Created on: 1 jun. 2021
 *      Author: freds
 */

#include "CardSD.h"
#include "Leds.h"
#include "ff.h"       	// Biblioteca FAT FS
#include "fssdc.h"


static FIL fp;             			// <-- File object needed for each open file

typedef unsigned int	UINT;


 void writeRegisterGateway(float Buffer1, float Buffer2, float Buffer3){

	 rtcRead( &rtc );
	 char bufferRegistre_1[100];
	 char bufferRegistre_2[100];
	 char bufferRegistre_3[100];

	 if( f_open( &fp, FILENAME1, FA_WRITE | FA_OPEN_APPEND  ) == FR_OK ){ //

		 UINT n = sprintf( bufferRegistre_1, "Temperatura sensor_1: %3.1f C %04d-%02d-%02d %02d:%02d:%02d\r\n",
				 Buffer1,
				 rtc.year,
				 rtc.month,
				 rtc.mday,
				 rtc.hour,
				 rtc.min,
				 rtc.sec
		 );

		 UINT nbytes;
		 f_write( &fp, bufferRegistre_1, n, &nbytes );

		 f_close(&fp);

		 if( nbytes == n ){
			 // printf("Escribio correctamente\n");
			 gpioWrite( LEDG, ON );
		 } else {
			 gpioWrite( LEDR, ON );
			 // printf("Escribio %d bytes\n", nbytes);
		 }

	 } else{
		 // printf("Error al abrir el archivo\n");
		 gpioWrite( LEDR, ON );

	 }



	 if( f_open( &fp, FILENAME2, FA_WRITE | FA_OPEN_APPEND  ) == FR_OK ){ //| FA_OPEN_APPEND

		 UINT n = sprintf( bufferRegistre_2, "Temperatura sensor_2: %3.1f C %04d-%02d-%02d %02d:%02d:%02d\r\n",
				 Buffer2,
				 rtc.year,
				 rtc.month,
				 rtc.mday,
				 rtc.hour,
				 rtc.min,
				 rtc.sec
		 );

		 UINT nbytes;
		 f_write( &fp, bufferRegistre_2, n, &nbytes );

		 f_close(&fp);

		 if( nbytes == n ){
			 // printf("Escribio correctamente\n");
			 gpioWrite( LEDG, ON );
		 } else {
			 gpioWrite( LEDR, ON );
			 // printf("Escribio %d bytes\n", nbytes);
		 }

	 } else{
		 //printf("Error al abrir el archivo\n");
		 gpioWrite( LEDR, ON );
	 }


	 if( f_open( &fp, FILENAME3, FA_WRITE | FA_OPEN_APPEND  ) == FR_OK ){ // | FA_OPEN_APPEND

		 UINT n = sprintf( bufferRegistre_3, "Temperatura sensor_3: %3.1f C %04d-%02d-%02d %02d:%02d:%02d\r\n",
				 Buffer3,
				 rtc.year,
				 rtc.month,
				 rtc.mday,
				 rtc.hour,
				 rtc.min,
				 rtc.sec
		 );

		 UINT nbytes;
		 f_write( &fp, bufferRegistre_3, n, &nbytes );

		 f_close(&fp);

		 if( nbytes == n ){
			 //printf("Escribio correctamente\n");
			 gpioWrite( LEDG, ON );
		 } else {
			 gpioWrite( LEDR, ON );
			 //printf("Escribio %d bytes\n", nbytes);
		 }

	 } else{
		 //printf("Error al abrir el archivo\n");
		 gpioWrite( LEDR, ON );
	 }

 }

 // FUNCION que se ejecuta cada vezque ocurre un Tick
  void diskTickHook( void *ptr )
 {
    disk_timerproc();   // Disk timer process
 }






