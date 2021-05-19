/*=============================================================================
 * Author: Freds Amundaray Cruz <freds.amundaray@gmail.com>
 * Date: 2021/01/13
 * Version: 1
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __CONTROL_DATA_H__
#define __CONTROL_DATA_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include "sapi.h"
#include <stdint.h>
#include <stddef.h>


/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/
#define DATA_RX_SIZE 50
#define DATA_SIZE 50
#define RFDATA_SIZE	10
#define SENSOR_VALUE_SIZE 8




/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/
typedef enum{
	ESCAPED_CHARACTERS,
	VALIDATE,
	ADECUACY,
	RECORD,
	TRANSMIT,
}sequence_t;

typedef struct{
	delay_t delay1;
	delay_t delay2;
	delay_t delay3;
	sequence_t mode;
	uint8_t DataReceive[DATA_RX_SIZE];
	uint8_t counterReceive;
	uint8_t DataXbee[DATA_SIZE];
	uint8_t Sensor_Value1[SENSOR_VALUE_SIZE];
	uint8_t Sensor_Value2[SENSOR_VALUE_SIZE];
	uint8_t Sensor_Value3[SENSOR_VALUE_SIZE];
	uint8_t RFData[RFDATA_SIZE];
}package_t;

/*=====[Prototypes (declarations) of public interrupt functions]=============*/
void onRx( void *noUsado);
void Control_init(package_t *sec);
void Control_data(package_t *sec);

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __CONTROL_DATA_H__ */
