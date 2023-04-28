 /*********************************************************************
 *
 * Module:smart home applications
 *
 * File Name: app.c
 *
 * Description: Source file for the application layer for smart home
 *
 * Created on:- 15/2/2022
 *
 * Author: Mohammed Sayed Shaaban
 *
 *********************************************************************/
 
#include "std_types.h" 
#include <util/delay.h>
/*to use some AVR_registers Like SREG*/
#include <avr/io.h>
/*to use the prototypes that belong to this applications*/
#include "app_functions.h"
/*to use eeprom_memory to save the passwords*/
#include "internal_eeprom.h"
/*to use them as rooms & air condition and TV*/
#include "led.h"
/*to use calculate the time out for guest mode and admin_mode*/
#include "timer1.h"

/*to communicate between two micro_controllers*/
#include "spi.h"

#include <avr/interrupt.h>

/***************************************************************************************************************************************************

                                                        Global Variables

****************************************************************************************************************************************************/
/*global variable that hold the mode that chosen by user and shared
 * to app.c file to keep the value of mode
 * */

extern uint8 g_mode;

/*to return the allowed number of passwords to the default value*/
extern uint8 left_times;

/* global variable to avoid asking Guest to enter the password
 * Thought the available time that detected by timers
 */

extern volatile uint8 pass_check_count;

/*to keep the system always on available options of guest
 * before the time is out
 */
extern uint8 window_check;


int main(void){
	/*to receive the required state of Air condition*/
	uint8 air_option=1;
	uint8 Block_mode_state=0;
	/*to recieve the state of the required Object the coming form mc2*/
	volatile uint8 Obj_state=LOGIC_HIGH;
	/*the required state that chosen by the user*/
	uint8 req_state;
	/*enable the global interrupt for timer1*/
	SREG |=(1<<7);
    /*used as count for (loops)*/
	uint8 count;
	/*to hold the value for this location in EEPROM*/
    uint8 admin_data=0;
	/*to hold the value for this location in EEPROM*/
    uint8 guest_data=0;
	/*to hold what  mode was selected by user*/
	//volatile uint8 mode;
	/*to hold the state of guest or admin password is correct or not*/
	uint8 pass_state=0;
	/*to hold which option is selected by guest*/
	uint8 Guest_access_option=0;
	/*to hold which option is selected by admin in first window*/
	uint8 Admin1_access_option=0;
	/*to hold which option is selected by admin in second window*/
	uint8 Admin2_access_option=0;

	/*Initialize the LCD */
	LCD_init();
	_delay_ms(10);
	/*set the callBack Timer1 function*/
	Timer1_setCallBack(APP_SetCallBack);


	/*configuration the timer1 for admin_mode (60 second):
	 *  --->initialize value =0
	 *  --->compare value = 60000
	 *  --->compare mode
	 *  --->prescalar=1024
	 * */
	Timer1_ConfigType admin_conf;
	/*set initialize value =0*/
	admin_conf.initial_value=0;
	/*set compare value = 60000 (60 second)*/
	admin_conf.compare_value=60000;
	/*set the compare mode */
	admin_conf.mode=COMPARE_MODE;
	/*set the prescalar =1024*/
	admin_conf.prescalar=CLK_1024;

	/*configuration the timer1 for guest_mode (40 second):
	 *  --->initialize value =0
	 *  --->compare value = 40000
	 *  --->compare mode
	 *  --->prescalar=1024
	 * */
	Timer1_ConfigType guest_conf;
	/*set initialize value =0*/
	guest_conf.initial_value=0;
	/*set compare value = 40000--->40 seconds*/
	guest_conf.compare_value=40000;
	/*set the compare mode */
	guest_conf.mode=COMPARE_MODE;
	/*set the prescalar =1024*/
	guest_conf.prescalar=CLK_1024;
    /*Initialize SPI driver as master for this micro_controller */
	SPI_initMaster();

	/*set the three led to indicate for admin_mode or guest or block*/
	LED_SETUP(ADMIN_PORT,ADMIN_PIN);
	LED_SETUP(GUEST_PORT,GUEST_PIN);
	LED_SETUP(BLOCK_PORT,BLOCK_PIN);
	
	/*read state of block mode*/
	Block_mode_state=Internal_readByte(BLOCK_LOCATION_CHECK);
	_delay_us(10);
	if((Block_mode_state !=0xFF) && (Block_mode_state!=0)){
		Blocked_mode();
	}
    /*to read this location to check we will need set admin pass or not*/
	admin_data=Internal_readByte(ADMIN_LOCATION_CHECK);
	_delay_us(10);
    /*to read this location to check we will need set Guest_pass or not*/
	guest_data=Internal_readByte(GUEST_LOCATION_CHECK);
	_delay_us(10);

	/*if one of these conditions true -->this mean there is wrong
	 * in this operation so we need to run it again
	 * */

	if(admin_data !=PASS_SET || guest_data !=PASS_SET){
		/*set the passwords for the admin and the guest*/
		set_password();
	}
	/*to get the mode that was selcted by user*/
	select_mode();
	while(1){
		/*to indicate the time is over*/
		if(window_check==0){
		   select_mode();
		}
		/*if mode equal=0 this mean the user select admin_mode otherwise guest*/
		/*this is mean the user select the admin_mode*/
		if(g_mode==0){
			/*run the timer to count 60 seconds*/
			Timer1_init(&admin_conf);
	        /*to check if user pressed "More" option*/
			Admin2_access_option=0;
			/*turn on the led that represent the guest mode*/
			LED_OFF(GUEST_PORT,GUEST_PIN);
			/*turn off the others led */
			LED_ON(ADMIN_PORT,ADMIN_PIN);
			LED_OFF(BLOCK_PORT,BLOCK_PIN);
			/*to allow the user three times*/
			count=NUM_TRIES_ALLOWED;
			do{
				/*this mean this the first time asking password from the guest*/
				if(pass_check_count==0){
				   pass_state=check_pass(ADMIN);
			    }
			
			/*if this condition true this mean that password is correct*/
				if(pass_state==0){
					/*this mean we don't need the rest of tries any more*/
					break;
				}
				/*this mean the password is wrong*/
				else if(pass_state>0){
					/*this mean the user entered the password wrong*/
					wrong_passwords();
				}
			    count--;

		    }while(count!=0);
			/*to avoid the system for asking the password again*/
			pass_check_count++;
			/*return the allowed times of entering wrong passwords to default*/
			left_times=NUM_TRIES_ALLOWED;
			/*check if the user used the allowed maximum times of tries*/
			if(count==0){
				/*this mean that the user used the allowed three times*/
				/*go to the Blocked Mode-->stop the system for 25 seconds*/
				Blocked_mode();
				/*to left this steps and return to the check
				 * passwords again
				 */
				continue;
			}
/*to enter always the loop as always as the time is not out*/
			window_check=1;
			do{
				do{
					if(Admin2_access_option!=10){
						/*to get off the loop if the user choose any thing except RET option */ 
						Admin2_access_option=0;
						/*this mean the user choose to Previous window*/
						/*move to the available options for guest*/
						Admin1_access_option=Admin_First_access();
					}
					/*check if admin want to goes to the second window*/
					if(Admin1_access_option==4 || Admin2_access_option==10){
						Admin2_access_option=Admin_Second_access();
					}
				}while(Admin2_access_option==4);
				/*send the mode to other controller*/
				SPI_sendReciveByte(g_mode);
				/*this mean in the first window*/
				if(Admin2_access_option==0){
					/*send the window (0 mean the first window) (1 mean the second window)*/
					SPI_sendReciveByte(0);
					/*send the Guest_Option that chosen */
					SPI_sendReciveByte(Admin1_access_option);
					/*to the other controller get the state*/
					_delay_ms(100);
					/*receive the state of an object from the other MC*/
					Obj_state=SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);
					/*display the informations of the required room*/
					req_state= Admin1_menu(Obj_state,Admin1_access_option);
					/*send the required state*/
					SPI_sendReciveByte(req_state);
					_delay_ms(100);

				}
				/*this mean in the second window*/
				else{
					/*send the window (0 mean the first window) (1 mean the second window)*/
					SPI_sendReciveByte(1);
					/*send the Guest_Option that chosen */
					SPI_sendReciveByte(Admin2_access_option);
					/*to the other controller get the state*/
					_delay_ms(100);

					/*check if user want to access the air condition*/
					if(Admin2_access_option==3){
						do{
						   air_option=AirController_fun();
						}while(air_option!=0);
						/*to indicate the system must goes to the second window of this system*/
						Admin2_access_option=10;
						continue;                   
					}
					else{
							/*receive the state of an object from the other MC*/
						Obj_state=SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);
						/*display the informations of the required room*/
						req_state= Admin2_menu(Obj_state,Admin2_access_option);
						/*send the required state*/
						SPI_sendReciveByte(req_state);
						_delay_ms(100);
						/*this mean the user choose to Previous window*/
						/*move to the available options for guest*/
						Admin2_access_option=Admin_Second_access();
						/*check if user want to goes back to first window*/
						if(Admin2_access_option==4){
							/*reinitialize the value of this variable to enter the first window*/ 
							Admin2_access_option=0;
						}
					}
				}
			}while(window_check==1);
			/*this mean the  admin time  is out*/
			if(window_check==0)                    //
			    select_mode();
			
		}
		/*this is mean the user select the guest_mode*/
		else if(g_mode==1){
			/*run the timer to count 60 seconds (avaliable time)*/
			Timer1_init(&guest_conf);
			/*turn on the led that represent the guest mode*/
			LED_ON(GUEST_PORT,GUEST_PIN);
			/*turn off the others led */
			LED_OFF(ADMIN_PORT,ADMIN_PIN);
			LED_OFF(BLOCK_PORT,BLOCK_PIN);
			/*to allow the user three times*/
			count=NUM_TRIES_ALLOWED;
			do{

				/*this mean this the first time asking password
				 * from the guest
				 */
				if(pass_check_count==0){
				   pass_state=check_pass(GUEST);
			    }
			/*if this condition true this mean that password is correct*/
				if(pass_state==0){

					/*move to the available options for guest*/
					Guest_access_option=Guest_access();
				    /*this mean we don't need the rest of tries any more*/
					break;
			    }
				else if(pass_state>0){
					/*this mean the user entered the password wrong*/
					wrong_passwords();
				}
				count--;

		    }while(count!=0);
			/*to avoid the system for asking the password again*/
			pass_check_count++;
			/*return the allowed times of entering wrong passwords to default*/
			left_times=NUM_TRIES_ALLOWED;
			/*check if the user used the allowed maximum times of tries*/
			if(count==0){
				/*this mean that the user used the allowed three times*/
				/*go to the Blocked Mode-->stop the system for 25 seconds*/
				Blocked_mode();
				/*to left this steps and return to the check
				 * passwords again
				 */
				continue;
			}
/*to enter always the loop as always as the time is not out*/
			window_check=1;
			do{
				/*send the mode to other controller*/
				SPI_sendReciveByte(g_mode);
				/*send the Guest_Option that chosen */
				SPI_sendReciveByte(Guest_access_option);
				/*to the other controller get the state*/
				_delay_us(100);
				/*receive the state of an object from the other MC*/
				Obj_state=SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);

				/*display the informations of the required room*/
				req_state= Guest_menu(Obj_state,Guest_access_option);
			    /*send the required state*/
			    SPI_sendReciveByte(req_state);

				/*this mean the user choose to Previous window*/
				/*move to the available options for guest*/
				_delay_ms(100);

			    Guest_access_option=Guest_access();

				}while(window_check==1);
				/*this mean the time for guest is over*/
				select_mode();
 		}
		else{
			/*this mean the user select optional not exist*/
			/*so will back to selection list again*/
			select_mode();
		}
	}
}

