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
#define DATARECEIVE_RX_SIZE	50
#define TIMERECORD 1
#define TIMESEND 1
#define TIMEDELAY 60
#define VALUE_MAX_S1 20
#define VALUE_MIN_S1 5
#define VALUE_MAX_S2 25
#define VALUE_MIN_S2 12
#define VALUE_MAX_S3 28
#define VALUE_MIN_S3 10
#define VALUE_REPEAT 2
#define AMARILLO LED1
#define ROJO LED2
#define VERDE LED3



/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/
typedef enum{
	ESCAPED_CHARACTERS,
	VALIDATE,
	ADECUACY,
	RECORD,
	TRANSMIT,
	CALLSMS,
}sequence_t;

typedef struct{
	uint8_t counter_clear1;
	uint8_t counter_clear2;
	uint8_t counter_clear3;
	delay_t delay1sec;
	uint8_t counterCallMsg1;
	uint8_t counterCallMsg2;
	uint8_t counterCallMsg3;
	float data1;
	float data2;
	float data3;
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
void diskTickHook( void *ptr );
/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __CONTROL_DATA_H__ */
