/*****************************************************************************************************************************************************
 *
 * Module:smart home applications
 *
 * File Name: app2_functions.c
 *
 * Description: source file for the functions application layer
 * for Microcontroller2 for smart home
 *
 * Created on:- 15/2/2022
 *
 * Author: Mohammed Sayed Shaaban
 *
 *****************************************************************************************************************************************************/
#include "app2_functions.h"
#include "spi.h"
#include "lm.h"
#include <util/delay.h>

/*to receive  the temperature from other micro controller (it has default temperature equal 24 degree) */
uint8  g_temperature = 24;

/******************************************************************************************************************************************************

                                                             Functions Definitions

******************************************************************************************************************************************************/
/*
 * Function_Name:Home_Setup
 *
 * description:setup  the home (rooms & air_condition & TV)
 */

void Home_Setup(void){
	/*setup rooms & air condition & TV as output pins*/
	/*Turn them  off at the begging*/
	LED_SETUP(ROOMS_PORT,ROOM1_PIN);
	LED_SETUP(ROOMS_PORT,ROOM2_PIN);
	LED_SETUP(ROOMS_PORT,ROOM3_PIN);
	LED_SETUP(ROOMS_PORT,ROOM4_PIN);
	LED_SETUP(ROOMS_PORT,COND_PIN);
	LED_SETUP(ROOMS_PORT,TV_PIN);
}

/*
 * Function_Name:Object_controle
 *
 * description:to know the state of each object in this home and
 * controlling this object
 */
void Object_controle(uint8 object_port,uint8 object_pin){
	/*to hold the state of rooms & air conditioning &TV*/
	volatile uint8 state;
	/*to hold the required state from user*/
	uint8 req_state;
	/*read the state of the Room_1*/
	state=LED_Read(object_port,object_pin);
	/*send the state of the required object to the master*/
	SPI_sendReciveByte(state);
    /*receive an order from MC_master to do some actions to room1*/
    req_state=SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);
    /*if user press 1 -->they want turn on this room1*/
    if(req_state==1){
    	/*turn on this room*/
    	LED_ON(object_port,object_pin);
    }
    else if(req_state==2){
    	/*turn on this room*/
    	LED_OFF(object_port,object_pin);
    }
	else{
		//nothing
	}
}

/*
 * Function_Name:Object_controle
 *
 * description:to know the state of each object in this home and
 * controlling this object
 */
uint8 Air_controle(uint8 object_port,uint8 object_pin){
	/*to receive the option that chosen by user*/
	uint8 Air_option;
	/*receive temperature from the controller*/
	uint8 sensor_temperature;
	/*to hold the state of rooms & air conditioning &TV*/
	uint8 state;
	/*to hold the required state from user*/
	uint8 req_state=1;
    do{
		/*get the temperature of object by using LM sensor*/

		/*receive the option that chosen by user*/ 
		Air_option= SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);
		/*in case the user want to set the temperature*/
		if(Air_option==1){
			/*from here*/
			g_temperature=SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);
		}
		/*this mean the user want to Turn on / off the air condition*/
		else if(Air_option==2){
			state=LED_Read(object_port,object_pin);
			/*send the state of the required object to the master*/
			SPI_sendReciveByte(state);
			_delay_ms(10);
			/*receive an order from MC_master to do some actions to room1*/
			req_state=SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);
			/*if user press 1 -->they want turn on this room1*/
			
			if(req_state==1){
				/*to read the temperature*/
				sensor_temperature=LM35_GetTemperature();
				_delay_ms(10);
				if(sensor_temperature > g_temperature){
					/*turn on this room*/
					LED_ON(object_port,object_pin);
				}
				else{
					LED_OFF(object_port,object_pin);
				}
				
			}
			else if(req_state==2){
				/*turn on this room*/
				LED_OFF(object_port,object_pin);
			}
			else{
				//nothing
			}
		}
		/*this mean the user choose to return to the previous window*/
		else{
			//do nothing
		}
	}while(req_state==0);
	return Air_option;
}




