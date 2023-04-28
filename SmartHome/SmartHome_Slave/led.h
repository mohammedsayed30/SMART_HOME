 /*************************************************************
 *
 * Module: LED
 *
 * File Name: led.h
 *
 * Description: Header file for the LED driver
 *
 *  Created on:30/1/2023
 *
 * Author: Mohammed Sayed
 *
 ************************************************************/

#ifndef LED_H_
#define LED_H_
#include "std_types.h"



/*************************************************************
 *                   Functions_Prototypes
 *************************************************************/

/*************************************************************
 * Function_Name:LED_SETUP
 *
 * Description:Set the direction of the Led (always out)
 *
 *************************************************************/
void LED_SETUP(uint8 ,uint8);

/*************************************************************
 * Function_Name:LED_ON
 *
 * Description:Turn on the Led
 *
 *************************************************************/
void LED_ON(uint8 ,uint8);

/*************************************************************
 * Function_Name:LED_OFF
 *
 * Description:Turn OFF the Led
 *
 *************************************************************/
void LED_OFF(uint8 ,uint8);

/*************************************************************
 * Function_Name:LED_Read
 *
 * Description:return the state of led
 *
 *************************************************************/
uint8 LED_Read(uint8 ,uint8);



#endif /* LED_H_ */
