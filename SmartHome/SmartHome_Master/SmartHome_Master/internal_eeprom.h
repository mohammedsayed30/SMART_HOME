 /**************************************************************
 * Module: Internal EEPROM
 *
 * File Name: Internal EEPROM.h
 *
 * Description: Header file for the In_EEPROM driver
 *
 * Author: Mohammed Sayed Shaaban
 *
 * Created on : 15/3/2023
 *
 **************************************************************/

#ifndef IN_EEPROM_H_
#define IN_EEPROM_H_
/***************************************************************
 *                     Includes needed files
 **************************************************************/
#include "std_types.h"

/**************************************************************
 *                      Functions Prototypes
 **************************************************************/

/*
 * Function_Name:Internal_writeByte
 *
 * description:
 * -->write byte to the Internal EEPROM Atmega32
 * -->take the address and data
 */

void Internal_writeByte(const uint16,const uint8);

/*
 * Function_Name:Internal_readByte
 *
 * description:
 * -->read byte from the Internal EEPROM Atmega32
 * -->take the address and variable that hold the needed data
 */

uint8 Internal_readByte(uint16);

#endif /* IN_EEPROM_H_ */
