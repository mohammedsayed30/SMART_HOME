 /*************************************************************
 *
 * Module: LED
 *
 * File Name: led.c
 *
 * Description: Source file for the LED driver
 *
 *  Created on:30/1/2023
 *
 * Author: Mohammed Sayed
 *
 ************************************************************/
#include "led.h"
#include "gpio.h"


/*************************************************************
 * Function_Name:LED_SETUP
 *
 * Description:Set the direction of the Led (always out)
 *
 *************************************************************/
void LED_SETUP(uint8 LED_PORT,uint8 LED_PIN){
	GPIO_setupPinDirection(LED_PORT,LED_PIN , PIN_OUTPUT);
	GPIO_writePin(LED_PORT,LED_PIN ,LOGIC_LOW);
}

/*************************************************************
 * Function_Name:LED_ON
 *
 * Description:Turn on the Led
 *
 *************************************************************/
void LED_ON(uint8 LED_PORT,uint8 LED_PIN){
	GPIO_writePin(LED_PORT,LED_PIN ,LOGIC_HIGH);
}

/*************************************************************
 * Function_Name:LED_OFF
 *
 * Description:Turn OFF the Led
 *
 *************************************************************/
void LED_OFF(uint8 LED_PORT,uint8 LED_PIN){
	GPIO_writePin(LED_PORT,LED_PIN ,LOGIC_LOW);
}


