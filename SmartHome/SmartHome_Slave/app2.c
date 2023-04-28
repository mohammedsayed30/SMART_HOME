 /*********************************************************************
 *
 * Module:smart home applications
 *
 * File Name: app2.c
 *
 * Description: source file for the application layer
 * for Microcontroller2 for smart home
 *
 * Created on:- 15/2/2022
 *
 * Author: Mohammed Sayed Shaaban
 *
 *********************************************************************/
#include "app2_functions.h"
#include "spi.h"
#include "led.h"
#include "adc.h"
#include "lm.h"
#include <util/delay.h>
int main(void){
	/*setup the whole home (rooms & air condition &TV)*/
	Home_Setup();
	/*Initialize SPI driver as Slave for this Micro_controller*/
	SPI_initSlave();
	/*variable that hold the received data from master*/
	volatile uint8 recieved_mode;
	/*variable that hold the received data from master*/
	volatile uint8 Guest_recieved_data;
	/*to check the admin chose form first or second window*/
	volatile uint8 admin_window_check;
	/*variable that hold the received data from master(in admin mode) first window*/
	volatile Admin1_recieved_data;
	/*variable that hold the received data from master(in admin mode) first window*/
	volatile uint8 Admin2_recieved_data;
	/*to recieve the option of air condition*/
	uint8 air_option;
	/*run ADC module to use it for getting the Temperature*/
	ADC_ConfigType ADC_Config;       /*Structure hold the configuration of ADC*/
	ADC_Config.ref_volt=INTERNAL_AREF;       /*reference voltage(2.56) */
	ADC_Config.prescalar=8;         /*prescaler*/
	ADC_init(&ADC_Config);         /*to initialize the ADC*/
	_delay_ms(10);

	while(1){
		/*Receive the mode from other controller */
		recieved_mode=SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);
		/*this mean guest mode*/
		if(recieved_mode==1){
			/*Receive the Guest_Option that chosen */
			Guest_recieved_data=SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);
			switch (Guest_recieved_data){
			/*if guest want to access Room_1*/
			case 1:
				/*call the required options of room1*/
				Object_controle(ROOMS_PORT,ROOM1_PIN);
				break;
			/*if guest want to access Room_2*/
			case 2:
				/*call the required options of room1*/
				Object_controle(ROOMS_PORT,ROOM2_PIN);
				break;
			/*if guest want to access Room_3*/
			case 3:
				/*call the required options of room1*/
				Object_controle(ROOMS_PORT,ROOM3_PIN);
				break;
			/*if guest want to access Room_4*/
			case 4:
				/*call the required options of room1*/
				Object_controle(ROOMS_PORT,ROOM4_PIN);
				break;
			}

		}
		/*this mean admin mode*/
		else{
			/*Receive which window that admine chose from it*/
			admin_window_check=SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);
			/*this mean in the first window (room1&room2&room3)*/
			if(admin_window_check==0){
				/*to hold the received data from master(in admin mode) first window*/
				Admin1_recieved_data=SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);
				switch (Admin1_recieved_data){
				/*if admin want to access Room_1*/
				case 1:
					/*call the required options of room1*/
					Object_controle(ROOMS_PORT,ROOM1_PIN);
					break;
				/*if admin want to access Room_2*/
				case 2:
					/*call the required options of room2*/
					Object_controle(ROOMS_PORT,ROOM2_PIN);
					break;
				/*if admin want to access Room_3*/
				case 3:
					/*call the required options of room3*/
					Object_controle(ROOMS_PORT,ROOM3_PIN);
					break;
				}
			}
			else{
				/*to hold the received data from master(in admin mode) second window*/
				Admin2_recieved_data=SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);
				switch (Admin2_recieved_data){
				/*if admin want to access Room_4*/
				case 1:
					/*call the required options of room4*/
					Object_controle(ROOMS_PORT,ROOM4_PIN);
					break;
				/*if admin want to access TV*/
				case 2:
					/*call the required options of TV*/
					Object_controle(ROOMS_PORT,TV_PIN);
					break;
				/*if admin want to access air_condition*/
				case 3:
					/*call the required options of air_cond*/
					do{
					   air_option = Air_controle(ROOMS_PORT,COND_PIN);
					}while(air_option !=0);
					break;
				}
			}
		}
	}
}

