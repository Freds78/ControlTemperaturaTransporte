/*
 * sim900.c
 *
 *  Created on: 24 mar. 2021
 *      Author: freds
 */

#include "sim900.h"




char Numeros_Telefonos_1[] = "1126941040";


static void enviar_mensaje_sms_S1(float Temperatura);
static void enviar_mensaje_sms_S2(float Temperatura);
static void enviar_mensaje_sms_S3(float Temperatura);
static void llamar();
static void Conexion_GPRS_Adox(float Temperatura1, float Temperatura2, float Temperatura3);


static void enviar_mensaje_sms_S1(float Temperatura){

	uint8_t i;

	char msm_send[SIZE_MSM];
	char cmgsCall[SIZE_CALL];


	for(i = 0; i< SIZE_MSM; i++){
		msm_send[i] = '\0';
	}

	for(i = 0; i< SIZE_CALL; i++){
		cmgsCall[i] = '\0';
	}

	char msm1I[] = "Temperatura sensor_1 = ";
	char msm1F[] = "C\r\n";
	char cmgs[] = "AT+CMGS=\"";
	char endsms[] = "\"\r\n";

	char conversor[5];
	sprintf(conversor, "%3.1f", Temperatura);
	strcpy(msm_send,msm1I);
	strcat(msm_send,conversor);
	strcat(msm_send,msm1F);

	strcpy(cmgsCall,cmgs);
	strcat(cmgsCall,Numeros_Telefonos_1);
	strcat(cmgsCall,endsms);

	uartWriteString( UART_GPIO,"AT+CMGF=1\r\n"); 				// AT command to send SMS message
	delay(1000);
	uartWriteString( UART_GPIO,cmgsCall); 				 	// recipient's mobile number, in international format
	delay(1000);

	uartWriteString( UART_GPIO,msm_send); 					// message to send
	delay(1000);
	uartWriteByte( UART_GPIO, 0x1A ); 					// End AT command with a ^Z, ASCII code 26 //Comando de finalizacion
	uartWriteByte( UART_GPIO, '\r' );
	uartWriteByte( UART_GPIO, '\n' );
	delay(5000);

}



static void enviar_mensaje_sms_S2(float Temperatura){

	uint8_t i;

	char msm_send[SIZE_MSM];
	char cmgsCall[SIZE_CALL];


	for(i = 0; i< SIZE_MSM; i++){
		msm_send[i] = '\0';
	}

	for(i = 0; i< SIZE_CALL; i++){
		cmgsCall[i] = '\0';
	}

	char msm1I[] = "Temperatura sensor_2 = ";
	char msm1F[] = "C\r\n";
	char cmgs[] = "AT+CMGS=\"";
	char endsms[] = "\"\r\n";

	char conversor[5];
	sprintf(conversor, "%3.1f", Temperatura);
	strcpy(msm_send,msm1I);
	strcat(msm_send,conversor);
	strcat(msm_send,msm1F);

	strcpy(cmgsCall,cmgs);
	strcat(cmgsCall,Numeros_Telefonos_1);
	strcat(cmgsCall,endsms);

	uartWriteString( UART_GPIO,"AT+CMGF=1\r\n"); 				// AT command to send SMS message
	delay(1000);
	uartWriteString( UART_GPIO,cmgsCall); 				 	// recipient's mobile number, in international format
	delay(1000);

	uartWriteString( UART_GPIO,msm_send); 					// message to send
	delay(1000);
	uartWriteByte( UART_GPIO, 0x1A ); 					// End AT command with a ^Z, ASCII code 26 //Comando de finalizacion
	uartWriteByte( UART_GPIO, '\r' );
	uartWriteByte( UART_GPIO, '\n' );
	delay(5000);

}



static void enviar_mensaje_sms_S3(float Temperatura){

	uint8_t i;

	char msm_send[SIZE_MSM];
	char cmgsCall[SIZE_CALL];


	for(i = 0; i< SIZE_MSM; i++){
		msm_send[i] = '\0';
	}

	for(i = 0; i< SIZE_CALL; i++){
		cmgsCall[i] = '\0';
	}

	char msm1I[] = "Temperatura sensor_3 = ";
	char msm1F[] = "C\r\n";
	char cmgs[] = "AT+CMGS=\"";
	char endsms[] = "\"\r\n";

	char conversor[5];
	sprintf(conversor, "%3.1f", Temperatura);
	strcpy(msm_send,msm1I);
	strcat(msm_send,conversor);
	strcat(msm_send,msm1F);

	strcpy(cmgsCall,cmgs);
	strcat(cmgsCall,Numeros_Telefonos_1);
	strcat(cmgsCall,endsms);

	uartWriteString( UART_GPIO,"AT+CMGF=1\r\n"); 				// AT command to send SMS message
	delay(1000);
	uartWriteString( UART_GPIO,cmgsCall); 				 	// recipient's mobile number, in international format
	delay(1000);

	uartWriteString( UART_GPIO,msm_send); 					// message to send
	delay(1000);
	uartWriteByte( UART_GPIO, 0x1A ); 					// End AT command with a ^Z, ASCII code 26 //Comando de finalizacion
	uartWriteByte( UART_GPIO, '\r' );
	uartWriteByte( UART_GPIO, '\n' );
	delay(5000);

}

static void llamar(){

	uint8_t i;

	char Comando_llamar[20];
	char ATD[]="ATD";					//comando AT para iniciar llamada
	char Coma[]=";\r\n";				//indica llamada de voz y no de dato (Fax)
	char ATH[]="ATH\r\n";

	for(i = 0; i< 20; i++){
		Comando_llamar[i] = '\0';
	}
	strcpy(Comando_llamar,ATD);

	strcat(Comando_llamar,Numeros_Telefonos_1);

	strcat(Comando_llamar,Coma);

	uartWriteString( UART_GPIO,Comando_llamar);

	delay(20000);

	uartWriteString( UART_GPIO,ATH); 						// Comando AT para colgar la llamada

	delay(1000);

}

static void Conexion_GPRS_Adox(float Temperatura1, float Temperatura2, float Temperatura3){

		uint8_t i,dato, id;
		id = 1;

		char url[140] = "GET http://demoadox.com/calidaddelairefiuba/services/Services.php?acc=";
		char placa_id[] = "AD&placa_id=";
		char field1[] ="&tempuno=";
		char field2[] ="&tempdos=";
		char field3[] ="&temptres=";
		char end[]= "HTTP/1.1\r\n";

		char conversor_0[5];
		char conversor_1[5];
		char conversor_2[5];
		char conversor_3[5];

		for(i=0; i< 5; i++){
			conversor_0[i] = '\0';
			conversor_1[i] = '\0';
			conversor_2[i] = '\0';
			conversor_3[i] = '\0';
		}

		sprintf(conversor_0, "%d", id);
		sprintf(conversor_1, "%3.1f", Temperatura1);
		sprintf(conversor_2, "%3.1f", Temperatura2);
		sprintf(conversor_3, "%3.1f", Temperatura3);


		strcat(url, placa_id);
		strcat(url, conversor_0);
		strcat(url, field1);
		strcat(url, conversor_1);
		strcat(url, field2);
		strcat(url, conversor_2);
		strcat(url, field3);
		strcat(url, conversor_3);
		strcat(url, end);


		uartWriteString( UART_GPIO,"AT+CGATT=1\r\n"); 										// Nos conectamos
		delay(5000);
		uartWriteString( UART_GPIO,"AT+CSTT=\"gprs.movistar.com.ar\",\"wap\",\"wap\"\r\n"); // configura el APN Nombre
		delay(5000);
		uartWriteString( UART_GPIO,"AT+CIICR\r\n"); 										// realiza una conexion inalambrica GPRS o CSD
		delay(5000);
		uartWriteString( UART_GPIO,"AT+CIFSR\r\n"); 										//  obtenemos nuestra IP local
		delay(5000);
		uartWriteString( UART_GPIO,"AT+CIPSPRT=0\r\n"); 									// establece un indicador '>' al enviar datos
		delay(5000);
		uartWriteString( UART_GPIO,"AT+CIPSTART=\"TCP\",\"159.203.150.67\",\"80\"\r\n"); // indicamos el tipo de conexion
		delay(5000);
		uartWriteString( UART_GPIO,"AT+CIPSEND\r\n"); 				// enviamos datos a traves de una conexion TCP o UDP
		delay(5000);
		uartWriteString( UART_GPIO,url); 							// envio el dato al servidor remoto
		uartWriteString( UART_GPIO,"Host: 159.203.150.67\r\n");
		uartWriteString( UART_GPIO,"\r\n");
		uartWriteByte( UART_GPIO, 0x1A ); 						// End AT command with a ^Z, ASCII code 26 //Comando de finalizacion
		uartWriteString( UART_GPIO,"\r\n");
		delay(5000);
		uartWriteString( UART_GPIO,"AT+CIPCLOSE\r\n"); 			// Cierra la conexión TCP o UDP
		delay(2000);



}


void GPRS_Sim900(float Temperatura1, float Temperatura2, float Temperatura3){
	Conexion_GPRS_Adox(Temperatura1, Temperatura2, Temperatura3);
}
void CALL_Sim900(){
	llamar();
}
void MSG_S1_Sim900(float Temperatura){
	enviar_mensaje_sms_S1(Temperatura);
}

void MSG_S2_Sim900(float Temperatura){
	enviar_mensaje_sms_S2(Temperatura);
}

void MSG_S3_Sim900(float Temperatura){
	enviar_mensaje_sms_S3(Temperatura);
}



