 /********************************************************************
 * File Name:spi.c
 *
 * Model:-Source File of SPI Driver
 *
 * Created on:12/10/2022
 *
 * Description:-SPI  to serial comunication Protocole
 *
 * Author:Mohammed Sayed Shaaban
 ********************************************************************/
#include "spi.h"
#include <avr/io.h>
#include "common_macros.h"
#include "gpio.h"
/*
 * Function_Name:SPI_initMaster
 *
 * Description:
 *
 *initialize the master
 *
 */

void SPI_initMaster(){
	/*set:
	 * MISO--->input pin
	 * MOSI---->output pin
	 * SCK---->output pin
	 * SS---->output pin
	 */
	GPIO_setupPinDirection(SPI_PORT_ID,SPI_MISO_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(SPI_PORT_ID,SPI_MOSI_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(SPI_PORT_ID,SPI_SCK_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(SPI_PORT_ID,SPI_SS_PIN_ID, PIN_OUTPUT);
	/*
	 * Disable interrupt
	 * SCK is low when idle
	 */
	SPCR=(1<<MSTR)|(1<<SPE);
	/*to use f_osc/4 */
	CLEAR_BIT(SPSR,SPI2X);
}

/*
 * Function_Name:SPI_initSlave
 *
 * Description:
 *
 *initialize the Slave
 *
 */

void SPI_initSlave(){
	GPIO_setupPinDirection(SPI_PORT_ID,SPI_MISO_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(SPI_PORT_ID,SPI_MOSI_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(SPI_PORT_ID,SPI_SCK_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(SPI_PORT_ID,SPI_SS_PIN_ID, PIN_INPUT);
	SPCR=(1<<SPE);
	/*to use f_osc/4 */
	CLEAR_BIT(SPSR,SPI2X);
}

/*
 * Function_Name:SPI_sendReciveByte
 *
 * Description:
 *
 * to send the reciever byte
 *
 */

uint8 SPI_sendReciveByte(uint8 data){
	SPDR=data;
	while(BIT_IS_CLEAR(SPSR,SPIF));
	return SPDR;
}

/*
 * Function_Name:SPI_sendString
 *
 * Description:
 *
 * to send String
 *
 */

void SPI_sendString(const uint8* str){
	uint8 i=0;
	uint8 data=0;      /*has the data from the other device*/
	while(str[i] !='\0'){
		data=SPI_sendReciveByte(str[i]);
		i++;
	}
}

/*
 * Function_Name:SPI_reciveString
 *
 * Description:
 *
 * to recive String
 *
 */

void SPI_reciveString(uint8* str){
	uint8 i=0;
	do{
	 str[i]=SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);
	 i++;
	}while(str[i] !='#');
	str[i]='\0';
}


