/*
 * main.c
 *
 *  Created on: 13 ene. 2021
 *      Author: freds
 */

#include "sapi.h"
#include "Control_data.h"
#include "CardSD.h"
#include "ff.h"       // <= Biblioteca FAT FS
#include "fssdc.h"	// API de bajo nivel para unidad "SDC:" en FAT FS


int main( void )
{
   // ----- Setup -----------------------------------
   boardInit();
   /* Inicializar la UART_USB junto con las interrupciones de Tx y Rx */
   uartConfig(UART_GPIO, 9600);
   uartConfig(UART_USB, 115200);
   uartConfig( UART_232, 115200 );

   // Seteo un callback al evento de recepcion y habilito su interrupcion
   uartCallbackSet(UART_232, UART_RECEIVE, onRx, NULL);

   // Habilito todas las interrupciones de UART_USB
   uartInterrupt(UART_232, true);
/**/
   // SPI configuration
   spiConfig( SPI0 );

   // Inicializar el conteo de Ticks con resolucion de 10ms,
   // con tickHook diskTickHook
   tickConfig( 10 );
   tickCallbackSet( diskTickHook, NULL );

   FSSDC_InitSPI ();
   FATFS fs;
   if( f_mount( &fs, "SDC:", 0 ) != FR_OK ){
	   gpioToggle( LEDR );
	   printf("SD no disponible\n");
	   delay(1000);
   }

   rtcInit(); // Inicializar RTC


   package_t gprs;

   Control_init(&gprs);

   // ----- Repeat for ever -------------------------
   while( true ) {

	  //gpioToggle(LEDB);
	  //delay(500);
      Control_data(&gprs);

   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the
   // case of a PC program.
   return 0;
}


