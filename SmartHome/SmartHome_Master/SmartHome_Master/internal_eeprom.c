 /**************************************************************
 * Module: Internal EEPROM
 *
 * File Name: Internal EEPROM.c
 *
 * Description: Source file for the In_EEPROM driver
 *
 * Author: Mohammed Sayed Shaaban
 *
 * Created on : 15/3/2023
 *
 **************************************************************/
/**************************************************************
 *                    include needed files
 **************************************************************/
#include "internal_eeprom.h"
#include "common_macros.h"
#include <avr/io.h>
#include <util/delay.h>

/***************************************************************
 *                      Function Definitions
 **************************************************************/

/*
 * Function_Name:Internal_writeByte
 *
 * description:
 * -->write byte to the Internal EEPROM Atmega32
 * -->take the address and data
 */

void Internal_writeByte(const uint16 uiAddress,const uint8 uiData){

	/* Wait for completion of previous write process*/
	while ( ( (EECR & (1 << EEWE) ) >>EEWE ) == 1)
	;
	/* Move the required address to address register */
	EEAR = uiAddress;

	/* Read the byte in the address of EEAR */
	EECR |= (1 << EERE);
	/*this mean the data already exist*/ 
	if (EEDR != uiData)
	{
		
		EEDR = uiData;/*move tha data to data registers*/

		/* Write logical one to EEMWE */
		EECR |= (1 << EEMWE);
		/* Start EEPROM write by setting EEWE */
		EECR |= (1 << EEWE);
	}
}

/*
 * Function_Name:Internal_readByte
 *
 * description:
 * -->read byte from the Internal EEPROM Atmega32
 * -->take the address and variable that hold the needed data
 * Return the required data from data registers
 */

uint8 Internal_readByte(uint16 add){
	while(EECR&(1<<EEWE));
	/*detect  the required address*/
	EEAR=add;
	EECR|=(1<<EERE);
	/*get the data from data Register*/
	return EEDR;
}





