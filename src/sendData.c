/*
 * sendData.c
 *
 *  Created on: 22 feb. 2021
 *      Author: freds
 */

#include "sendData.h"

static bool_t ChecksumTx (packet_t *pack);
static void PackageTx(packet_t *pack);
static void Escaped_Characters_Adequacy(packet_t *pack);

static void DelimInt(packet_t *pack);
static void PacketLength(packet_t *pack);
static void IdentApi(packet_t *pack);
static void Id(packet_t *pack);
static void AddDst64Bit(packet_t *pack);
static void AddDst16Bit(packet_t *pack);
static void BroadCast_radius(packet_t *pack);
static void Option(packet_t *pack);
static void DataRF(packet_t *pack, float buffer1, float buffer2, float buffer3);


static bool_t ChecksumTx (packet_t *pack){


	uint8_t i,j;
	uint16_t checkRsl, checkSm, ValChekSm;
	bool_t checkVal;

	checkRsl = 0;
	checkSm = 0;


	for(i = 0; i< (pack->dataTrans.DataTransmit[1] + pack->dataTrans.DataTransmit[2]); i++){
			checkSm += pack->dataTrans.DataTransmit[i + place_checksum];
	}

	checkRsl = checkSm & 0xFF;


	pack->dataTrans.DataTransmit[pack->dataTrans.DataTransmit[1] + pack->dataTrans.DataTransmit[2] + place_checksum] = 0xFF - checkRsl;

	for(j = (pack->dataTrans.DataTransmit[1] + pack->dataTrans.DataTransmit[2] + byte_Nocounter); j < counterTx; j++){
		pack->dataTrans.DataTransmit[j] = '\0';
	}

	// validacion del checksum

		ValChekSm = checkSm + pack->dataTrans.DataTransmit[(pack->dataTrans.DataTransmit[1] + pack->dataTrans.DataTransmit[2]) + place_checksum] ;
		ValChekSm = ValChekSm & 0xFF;

		if(ValChekSm == 0xFF){

			checkVal = TRUE;

		}else{

			checkVal = FALSE;
		}

		return checkVal;
}



static void DelimInt(packet_t *pack){

	pack->dataTrans.DataTransmit[0] = START_DELIMITER;

}


static void PacketLength(packet_t *pack) {

	uint16_t  val_Length;

	val_Length = 0;

	val_Length = TRANSMIT_REQUEST_TALL + FRAMEID_TALL + DESTINATION_ADRESS64_TALL
				+ DESTINATION_ADRESS16_TALL + BROADCAST_RADIUS_TALL + OPTION_TALL + pack->dataTrans.counterData;

	pack->dataTrans.DataTransmit[2] = (uint8_t)val_Length & 0xFF;
	pack->dataTrans.DataTransmit[1] = (uint8_t)(val_Length >>8) & 0xFF;

}

static void IdentApi(packet_t *pack){

	pack->dataTrans.DataTransmit[3] = TRANSMIT_REQUEST;

}

static void Id(packet_t *pack){

	pack->dataTrans.DataTransmit[4]= FRAMEID;

}

static void AddDst64Bit(packet_t *pack) {

	uint8_t i, j, var_aux, byte;
	uint64_t  var_addres;
	var_addres = 0;
	var_aux = 0;
	byte = 0;


	var_addres = DESTINATION_ADRESS64;

	for(i = 5; i < 13; i++){
		for(j = 0; j < 8; j++){

			var_aux = (uint8_t)var_addres & 0x8000000000000000;
			var_addres = var_addres << 1;
			byte = byte | var_aux;
		}
		pack->dataTrans.DataTransmit[i] = byte;
	}

}

static void AddDst16Bit(packet_t *pack) {

	pack->dataTrans.DataTransmit[14] = (uint8_t)DESTINATION_ADRESS16 & 0xFF;
	pack->dataTrans.DataTransmit[13] = (uint8_t)(DESTINATION_ADRESS16 >> 8) & 0xFF;

}

static void BroadCast_radius(packet_t *pack){

	pack->dataTrans.DataTransmit[15] = BROADCAST_RADIUS;

}

static void Option(packet_t *pack){

	pack->dataTrans.DataTransmit[16] = OPTION;

}

static void DataRF(packet_t *pack, float Buffer1, float Buffer2, float Buffer3) {

	uint8_t i;

	pack->dataTrans.counterData = 0;

	sprintf(pack->dataTrans.ValRFData, "P%3.1fS%3.1fT%3.1f", Buffer1, Buffer2, Buffer3);

	for(i = 0; i < pack->dataTrans.ValRFData[i] != '\0'; i++){
		pack->dataTrans.DataTransmit[i + 17] = pack->dataTrans.ValRFData[i];
		pack->dataTrans.counterData++;
	}
}

static void Escaped_Characters_Adequacy(packet_t *pack){

	uint8_t i, j, counter_pass;
	counter_pass = 0;
	pack->dataTrans.counterDataTx = 0;

	for(i = 0; i < (pack->dataTrans.DataTransmit[1] + pack->dataTrans.DataTransmit[2] + byte_Nocounter + counter_pass); i++){

		if(pack->dataTrans.DataTransmit[i]  == 0x7D ){
			pack->dataTrans.DataTransmit[i] = 0x7D;
			counter_pass++;
			for(j = (pack->dataTrans.DataTransmit[1] + pack->dataTrans.DataTransmit[2] + byte_Nocounter + counter_pass) ; j > i; j-- ){
				pack->dataTrans.DataTransmit[j + 1] = pack->dataTrans.DataTransmit[j];
			}
			i++;
			pack->dataTrans.DataTransmit[i] = 0x5D;
			pack->dataTrans.counterDataTx++;
		}
		if(pack->dataTrans.DataTransmit[i]  == 0x7E && pack->dataTrans.counterDataTx != 0){
			pack->dataTrans.DataTransmit[i] = 0x7D;
			counter_pass++;
			for(j = (pack->dataTrans.DataTransmit[1] + pack->dataTrans.DataTransmit[2] + byte_Nocounter + counter_pass) ; j > i; j-- ){
				pack->dataTrans.DataTransmit[j + 1] = pack->dataTrans.DataTransmit[j];
			}
			i++;
			pack->dataTrans.DataTransmit[i] = 0x5E;
			pack->dataTrans.counterDataTx++;
		}
		if(pack->dataTrans.DataTransmit[i]  == 0x11 ){
			pack->dataTrans.DataTransmit[i] = 0x7D;
			counter_pass++;
			for(j = (pack->dataTrans.DataTransmit[1] + pack->dataTrans.DataTransmit[2] + byte_Nocounter + counter_pass) ; j > i; j-- ){
				pack->dataTrans.DataTransmit[j + 1] = pack->dataTrans.DataTransmit[j];
			}
			i++;
			pack->dataTrans.DataTransmit[i] = 0x31;
			pack->dataTrans.counterDataTx++;
		}
		if(pack->dataTrans.DataTransmit[i]  == 0x13 ){
			pack->dataTrans.DataTransmit[i] = 0x7D;
			counter_pass++;
			for(j = (pack->dataTrans.DataTransmit[1] + pack->dataTrans.DataTransmit[2] + byte_Nocounter + counter_pass) ; j > i; j-- ){
				pack->dataTrans.DataTransmit[j + 1] = pack->dataTrans.DataTransmit[j];
			}
			i++;
			pack->dataTrans.DataTransmit[i] = 0x33;
			pack->dataTrans.counterDataTx++;
		}

		pack->dataTrans.counterDataTx++;
	}
}



static void PackageTx(packet_t *pack){

	uint8_t i;

	for(i = 0; i < pack->dataTrans.counterDataTx; i++){
		uartWriteByte( UART_232, pack->dataTrans.DataTransmit[i]);
		//uartWriteByte( UART_USB, pack->dataTrans.DataTransmit[i]);
	}
}


/*****************************************************************************************/

void XBEE_TX(packet_t *pack, float Buffer1, float Buffer2, float Buffer3){

	uint8_t i;

	for(i = 0; i < counterTx; i++){
		pack->dataTrans.DataTransmit[i] = '\0';
	}

	for(i = 0; i <  counterDt; i++){

		pack->dataTrans.ValRFData[i] = '\0';
	}

	DelimInt(pack);
	IdentApi(pack);
	Id(pack);
	AddDst64Bit(pack);
	AddDst16Bit(pack);
	BroadCast_radius(pack);
	Option(pack);
	DataRF(pack,Buffer1,Buffer2, Buffer3);
	PacketLength(pack);
	if(ChecksumTx(pack)){
		//El paquete se creo de forma correcta
		Escaped_Characters_Adequacy(pack);
		PackageTx(pack);
		printf("Transmiti el paquete\r\n" );
	}

}




