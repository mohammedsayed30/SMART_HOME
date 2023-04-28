 /*********************************************************************
 *
 * Module:smart home applications
 *
 * File Name: app_functions.c
 *
 * Description: source file for the functions application layer
 * for smart home
 *
 * Created on:- 15/2/2022
 *
 * Author: Mohammed Sayed Shaaban
 *
 *********************************************************************/
#include "app_functions.h"
#include "spi.h"
#include "LCD.h"
#include "keypad.h"
#include <util/delay.h>
#include "std_types.h"
#include <avr/io.h>

/**********************************************************************
 *                                Global Variables
 **********************************************************************/
/*global variable that hold the mode that chosen by user and shared
 * to app.c file to keep the value of mode
 * */
/*Initialize to another value except( 0 , 1) */
 volatile uint8 g_mode=2;

/*to know which time available left to the user*/

 uint8 left_times=NUM_TRIES_ALLOWED;

 /* global variable to avoid asking Guest to enter the password
  * Thought the available time that detected by timers
  */

 volatile uint8 pass_check_count=0;

	/*to keep the system always on available options of guest
	 *or admin before the time is out
	 */
 volatile uint8 window_check=1;



/*
 * Function_Name:APP_SetCallBack
 *
 * description:this response for some actions when the timer count
 * the required time(40 seconds) in guest mode or (60 seconds) in admin_mode
 * the action-->return to the previously window
 */

void APP_SetCallBack(void){
   volatile uint8 rest=0;
	/*to leave the guest window and return to select mode window*/
	window_check=0;
	/*to ask the user about passwords after the timeout if he
	 * want to enter again
	 * */
	pass_check_count=0;
	/*clear the LCD screen display that the time is out*/
	LCD_clearScreen();
	LCD_displayString("Time Out");
	/*turn each led that indicate which state works now*/
	/*turn of the led that indicates the guest mode*/
	LED_OFF(GUEST_PORT,GUEST_PIN);
	/*turn of the led that indicates the admin_mode*/
	LED_OFF(ADMIN_PORT,ADMIN_PIN);
	/*turn of the led that indicates the block mode*/
	LED_OFF(BLOCK_PORT,BLOCK_PIN);
	/*stop the timer for counting*/
	Timer1_deInit();
	/*to enable the watch dog timer to reset the system when this interrupt occurs for (2.1) second*/
	WDTCR = (1<<WDE)|(1<<WDP2)|(1<<WDP1)|(1<<WDP0);	

}



/*
 * Function_Name:set_password
 *
 * description:set the passwords of admin_mode and guest_mode
 * in the first time only
 *
 */
void set_password(void){
	/*counter for the loop*/
	uint16 i;
	/*to hold the number that is coming from user*/
	uint8 digit;

	LCD_moveCursor(0,3);
	LCD_displayString("Welcome home");
	_delay_ms(1000);
	LCD_clearScreen();
	
	_delay_ms(10);
	/*configurations for set the password the admin*/

	LCD_moveCursor(0,0);
	LCD_displayString("Set Admin Pass");
	LCD_moveCursor(1,0);
	LCD_displayString("Admin Pass:");
	for(i=0;i<NO_OF_DIGIT;i++){
		/*get the digit from the user*/
		digit=KEYPAD_getPressedKey();
		/*display (*) instead the real digit*/
		LCD_displayCharacter('*');
		/*press delay*/
		_delay_ms(500);
		/*save this digit in EEPROM*/
		Internal_writeByte((ADMIN_PASS_FIRST_LOCATION+i),digit);
		_delay_ms(10);
	}

	/*save any number in this location like  (15) so then check
	 * if admin password is set or not
	 * this location if it contain this number or not
	 * if it not contain this number --->this is mean set password not done
	 * if it contained this number(15)-->this is mean we don't need
	 * this step any more
	 */
	Internal_writeByte(ADMIN_LOCATION_CHECK ,PASS_SET);
	_delay_ms(10);

	/*clear the screen*/
	LCD_clearScreen();
	/*display that the password is saved in EEPROM*/
	LCD_moveCursor(0,0);
	LCD_displayString("Admin Pass saved");
	_delay_ms(1000);


	/*configurations for set the password the Guest*/
	LCD_moveCursor(0,0);
	LCD_displayString("Set Guest Pass");
	LCD_moveCursor(1,0);
	LCD_displayString("Guest Pass:");
	for(i=0;i<NO_OF_DIGIT;i++){
		/*get the digit from user*/
		digit=KEYPAD_getPressedKey();
		/*display (*) instead the real digit*/
		LCD_displayCharacter('*');
		/*press delay*/
		_delay_ms(500);
		/*save this number to EEPROM*/
		Internal_writeByte((GUEST_PASS_FIRST_LOCATION+i),digit);
		_delay_ms(10);
	}


	/*save any number in this location like  (15) to know
	 * if guest password is set or not so then check
	 * this location if it contain this number or not
	 * if it not contain this number --->this is mean set password not done
	 * if it contained this number(15)-->this is mean we don't need
	 * this step any more
	 */
	Internal_writeByte(GUEST_LOCATION_CHECK,PASS_SET);
	_delay_ms(10);
	/*clear the screen*/
	LCD_clearScreen();
	/*move the cursor to this location row=0 column=0*/
	LCD_moveCursor(0,0);
	LCD_displayString("Guest Pass saved");
	_delay_ms(1000);

}

/*
 * Function_Name:select_mode
 *
 * description:select one mode from admin_mode or guest mode
 *
 */
void select_mode(void){
    //volatile uint8 mode;
	/*clear the screen*/
	LCD_clearScreen();
	/*move cursor to this location -->first row and first column*/
	LCD_moveCursor(0,0);
	/*display both modes so the user can select the mode that he want*/
	LCD_displayString("Select Mode:-");
	LCD_moveCursor(1,0);
	LCD_displayString("0:Admin  1:Guest");
    /*wait till the user select one mode from both by using keypad*/
	g_mode= KEYPAD_getPressedKey();
	/*time press*/
	_delay_ms(500);
	/*return the mode to use it select the operations that will be
	 * executed based on what mode was selected by user
	 *  */
	//return mode;
}


/*
 * Function_Name:check_pass_guest
 *
 * description:to check  the password for guest is correct or not
 *
 */
uint8 check_pass(uint8 mode){
	/*hold the count for (for loop)*/
	volatile uint16 i;
	/*hold the digit from guest*/
	volatile uint8 digit=0;
	/*to hold the digit that come from EEPROM*/
	volatile uint8 data;
	/*used for check if any digit in password wrong*/
	/*default the password correct*/
	volatile uint8 password_check=0;
    /*clear the screen*/
	LCD_clearScreen();
	/*move the cursor to first row and first column*/
	LCD_moveCursor(0,0);
	/*asked the user to enter the guest password*/
	LCD_displayString("plz enter pass");
	/*move the cursor to second row and first column*/
	LCD_moveCursor(1,0);
	LCD_displayString("Enter pass:");
	/*loop to  receive the four digit from guest (password)*/
	for(i=0;i<NO_OF_DIGIT;i++){
		/*get the digits that represent the password*/
		digit=KEYPAD_getPressedKey();
		/*display (*) instead the real digit*/
		LCD_displayCharacter('*');
		/*press delay*/
		_delay_ms(500);

		if(mode==ADMIN){
			/*get the digit that is saved in this location from eeprom*/
			data=Internal_readByte((ADMIN_PASS_FIRST_LOCATION+i));
			_delay_ms(10);
		}
		/**if the system in guest mode*/
		else{
			/*get the digit that is saved in this location from eeprom*/
			data=Internal_readByte((GUEST_PASS_FIRST_LOCATION+i));
			_delay_ms(10);
		}


		if(digit != data){
		/*if enter here at least one time this mean pass is wrong*/
			password_check++;
		}
	}

	/*to known that password is correct or not*/
	return password_check;
}

/*
 * Function_Name:wrong_passwords
 *
 * description:shows messages when the password is wrong
 *
 */



void wrong_passwords(void){
	left_times--;
	/*Clean the Screen of LCD*/
	LCD_clearScreen();
	/*display this messages on screen*/
	LCD_displayString("Wrong password");
	LCD_moveCursor(1,0);
	LCD_displayString("left times :");
	LCD_displayCharacter(left_times + ASCII_ZERO);
	/*Until  the user can see the message*/
	_delay_ms(1000);
}

/*
 * Function_Name:Admin_access
 *
 * description:to access the whole home (first window)
 *
 */
uint8 Admin_First_access(void){
	/*to indicate if the first time to the admin that he enter the option*/
	uint8 count=0;
	/*to hold the option that the admin selected*/
	uint8 admin1_opt;
	/*clear the screen*/
	LCD_clearScreen();
	/*move the cursor to first row and first column*/
	LCD_moveCursor(0,0);
	/*display the available options for the guest*/
	LCD_displayString("1:Room1  2:Room2");
	LCD_moveCursor(1,0);
	LCD_displayString("3:Room3  4:More");
/*this loop only to handle if the user enter input among the options*/
	do {
		if(count>0){
			/*clear the screen*/
			LCD_clearScreen();
			/*display the available options for the guest*/
			LCD_displayString("Wrong input");
			/*delay 0.5 second so the user can see it*/
			_delay_ms(500);
			/*clear the screen*/
			LCD_clearScreen();
			/*display the available options for the guest*/
			LCD_displayString("1:Room1  2:Room2");
			LCD_moveCursor(1,0);
			LCD_displayString("3:Room3  4:More");
			
		}
		/*get the option that the guest selected by using keypad*/
		admin1_opt=KEYPAD_getPressedKey();
		count++;
		/*time pressed*/
		_delay_ms(500);
	}while(admin1_opt!=1 && admin1_opt!=2 && admin1_opt!=3 &&admin1_opt!=4);
	/*return which room was selected by guest*/
	return admin1_opt;
}

/*
 * Function_Name:Admin_access
 *
 * description:to access the whole home (second window)
 *
 */
uint8 Admin_Second_access(void){
	/*to indicate if the first time to the admin that he enter the option*/
	uint8 count=0;
	/*to hold the option that the guest selected*/
	uint8 admin2_opt;
	/*clear the screen*/
	LCD_clearScreen();
	/*move the cursor to first row and first column*/
	LCD_moveCursor(0,0);
	/*display the available options for the guest*/
	LCD_displayString("1:Room4  2:TV");
	LCD_moveCursor(1,0);
	LCD_displayString("3:Air_Cond 4:RET");
/*this loop only to handle if the user enter input among the options*/
	do {
		if(count>0){
			/*clear the screen*/
			LCD_clearScreen();
			/*display the available options for the guest*/
			LCD_displayString("Wrong input");
			/*delay 0.5 second so the user can see it*/
			_delay_ms(500);
			/*move the cursor to first row and first column*/
			LCD_moveCursor(0,0);
			/*display the available options for the guest*/
			LCD_displayString("1:Room4  2:TV");
			LCD_moveCursor(1,0);
			LCD_displayString("3:Air_Cond 4:RET");
		}
		/*get the option that the guest selected by using keypad*/
		admin2_opt=KEYPAD_getPressedKey();
		count++;
		/*time pressed*/
		_delay_ms(500);
	}while(admin2_opt!=1 && admin2_opt!=2 && admin2_opt!=3 && admin2_opt!=4);
	/*return which room was selected by guest*/
	return admin2_opt;
}


/*
 * Function_Name:guest_access
 *
 * description:to access the home_rooms only
 *
 */
uint8 Guest_access(void){
	/*to indicate if the first time to the guest that he enter the option*/
	volatile count=0;
	/*to hold the option that the guest selected*/
	volatile uint8 guest_option;
	/*clear the screen*/
	LCD_clearScreen();
	/*move the cursor to first row and first column*/
	LCD_moveCursor(0,0);
	/*display the available options for the guest*/
	LCD_displayString("1:Room1  2:Room2");
	LCD_moveCursor(1,0);
	LCD_displayString("3:Room3  4:Room4");
/*this loop only to handle if the user enter input among the options*/
	do {
		if(count>0){
			/*clear the screen*/
			LCD_clearScreen();
			/*display the available options for the guest*/
			LCD_displayString("Wrong input");
			/*delay 0.5 second so the user can see it*/
			_delay_ms(500);
			/*clear the screen*/
			LCD_clearScreen();
			/*display the available options for the guest*/
			LCD_displayString("1:Room1  2:Room2");
			LCD_moveCursor(1,0);
			LCD_displayString("3:Room3  4:Room4");
			count=0;
		}
		/*get the option that the guest selected by using keypad*/
		guest_option=KEYPAD_getPressedKey();
		/*time pressed*/
		_delay_ms(500);
		count++;
	}while((guest_option!=1) && (guest_option!=2) && (guest_option!=3) && (guest_option!=4));
	/*return which room was selected by guest*/
	return guest_option;
}





/*
 * Function_Name:Blocked_mode
 *
 * description:to stop the system for 25 seconds
 *
 */


void Blocked_mode(void){
	/*used to counter for loop and indicate for the seconds*/
	uint8 count;
	/*display the state of systems on lcd*/
	LCD_clearScreen();
	LCD_displayString("system Blocked");
	/*to display the time on LCD*/
	LCD_moveCursor(1,0);
	LCD_displayString("wait 20 seconds");
	/*Turn all led except the block led*/
	/*turn on the led that represent the guest mode*/
	LED_ON(BLOCK_PORT,BLOCK_PIN);
	/*turn off the others led */
	LED_OFF(ADMIN_PORT,ADMIN_PIN);
	LED_OFF(GUEST_PORT,GUEST_PIN);
	/*it is mean the user enter the block mode*/
	Internal_writeByte(BLOCK_LOCATION_CHECK,1); 
	/*hold the systems for 20 seconds*/
	for(count=0;count<20;count++){
		/*wait for 1 second */
	  _delay_ms(1000);
	}
	/*this mean the block mode finished its time*/
	Internal_writeByte(BLOCK_LOCATION_CHECK,0);
	/*turn off the led that indicate Block mode */
	LED_OFF(BLOCK_PORT,BLOCK_PIN); 
}

/* menus of home objects */

/*
 * Function_Name:Guest_menu
 *
 * description:to show the available  menu of Rooms
 * and take parameter that represent the state of this object
 */
uint8 Guest_menu(uint8 object_state,uint8 no_of_room){
	uint8 room_req_state;
	/*clean the screen of LCD*/
	LCD_clearScreen();
	/*this for room_1*/
	if(no_of_room ==1){
	    /*display the relative things of Room1*/
	    if(object_state==LOGIC_HIGH){
			LCD_displayString("ROOM1  s:ON");
		}
		else if(object_state==LOGIC_LOW){
			LCD_displayString("ROOM1  s:OFF");
		}
	}
	/*This for Room_2*/
	else if(no_of_room ==2){
	    /*display the relative things of Room1*/
	    if(object_state==LOGIC_HIGH){
			LCD_displayString("ROOM2  s:ON");
		}
		else if(object_state==LOGIC_LOW){
			LCD_displayString("ROOM2  s:OFF");
		}
	}
	/*This for Room_3*/
	else if(no_of_room ==3){
	    /*display the relative things of Room1*/
	    if(object_state==LOGIC_HIGH){
			LCD_displayString("ROOM3  s:ON");
		}
		else{
			LCD_displayString("ROOM3  s:OFF");
		}
	}
	/*This for Room_3*/
	else if(no_of_room ==4){
	    /*display the relative things of Room1*/
	    if(object_state==LOGIC_HIGH){
			LCD_displayString("ROOM4  s:ON");
		}
		else{
			LCD_displayString("ROOM4  s:OFF");
		}
	}
	LCD_moveCursor(1,0);
	/*display the available options*/
	LCD_displayString("1:on  2:off 0:Ret");
	/*get the required state from guest by using keypad*/
	room_req_state=KEYPAD_getPressedKey();
	/*return this key to use it for doing something else*/
	return room_req_state;
}


/* Function_Name:admin1_menu
*
* description:to show the available  menu of admin in first window
*
*/


uint8 Admin1_menu(uint8 object_state,uint8 no_of_option){
	uint8 room_req_state;
	/*clean the screen of LCD*/
	LCD_clearScreen();
	/*this for room_1*/
	if(no_of_option ==1){
		/*display the relative things of Room1*/
		if(object_state==LOGIC_HIGH){
			LCD_displayString("ROOM1  s:ON");
		}
		else if(object_state==LOGIC_LOW){
			LCD_displayString("ROOM1  s:OFF");
		}
	}
	/*This for Room_2*/
	else if(no_of_option ==2){
	    /*display the relative things of Room1*/
	    if(object_state==LOGIC_HIGH){
			LCD_displayString("Room 2  s:ON");
		}
		else{
			LCD_displayString("Room 2  s:OFF");
		}
	}
	/*This for Room_3*/
	else if(no_of_option ==3){
	    /*display the relative things of Room1*/
	    if(object_state==LOGIC_HIGH){
			LCD_displayString("Room 3  s:ON");
		}
		else{
			LCD_displayString("Room 3  s:OFF");
		}
	}

	LCD_moveCursor(1,0);
	/*display the available options*/
	LCD_displayString("1:on  2:off 0:Ret");
	/*get the required state from guest by using keypad*/
	room_req_state=KEYPAD_getPressedKey();
	/*return this key to use it for doing something else*/
	return room_req_state;
}


/* Function_Name:Guest_menu
*
* description:to show the available  menu of admin
*
*/


uint8 Admin2_menu(uint8 object_state,uint8 no_of_option){
	/*to hold the required state of any object*/
	uint8 req_state;
	/*clean the screen of LCD*/
	LCD_clearScreen();
	/*this for room_1*/
	if(no_of_option ==1){
	    /*display the relative things of Room1*/
	    if(object_state==LOGIC_HIGH){
			LCD_displayString("Room 4  s:ON");
		}
		else{
			LCD_displayString("Room 4  s:OFF");
		}
	}
	/*This for Room_2*/
	else if(no_of_option ==2){
	    /*display the relative things of Room1*/
	    if(object_state==LOGIC_HIGH){
			LCD_displayString("TV  s:ON");
		}
		else{
			LCD_displayString("TV  s:OFF");
		}
	}

	/*display the available options*/
	LCD_moveCursor(1,0);
	LCD_displayString("1:on  2:off 0:Ret");
	/*get the required state from guest by using keypad*/
	req_state=KEYPAD_getPressedKey();

	/*return this key to use it for doing something else*/
    return req_state;
}


/*
 * Function_Name:AirController_fun
 *
 * description:to handle the statements of Air Conditional
 *
 */


uint8 AirController_fun(void){
	/*to hold the required state of Air condition*/
	uint8 req_state=1;
	/*to hold the state of air condition*/
	uint8 obj_state;
	/*of Air_options*/
	uint8 Air_option;
	/*that hold the wanted temperature*/
	uint8 temp=0;
	/* to hold keypad input*/
	uint8 keypad_input_temp=0;
	/*count for loop*/
	uint8 i;
	do{
		LCD_moveCursor(0,0);
		LCD_displayString("1:set temperature");
		LCD_moveCursor(1,0);
		LCD_displayString("2:Control  0:RET");
		/*get the required state from guest by using keypad*/
		Air_option=KEYPAD_getPressedKey();
		_delay_ms(100);
		/*send this option to the other microcontroller*/
		SPI_sendReciveByte(Air_option);
		_delay_ms(100);
		/*the user wants to set the temperature*/
		if(Air_option==1){
			LCD_clearScreen();
			LCD_displayString("set temp:-  C");
			/*move cursor of LCD to row 1 and column 11*/
			LCD_moveCursor(0,10);
			/*to enter the temperature consist of two digit*/
			for(i=0;i<2;i++){
				keypad_input_temp=KEYPAD_getPressedKey();
				_delay_ms(500);
				temp=temp*10 + keypad_input_temp;
				LCD_displayCharacter(keypad_input_temp + ASCII_ZERO);
			}
			_delay_ms(1000);
			/*send the required temp to other micro controller*/
			SPI_sendReciveByte(temp);
		}
		else if(Air_option==2){
			/*receive the state of an object from the other MC*/
			obj_state=SPI_sendReciveByte(SPI_DEFAULT_DATA_VALUE);
			/*display the informations of the required room*/
			LCD_clearScreen();
			if(obj_state==LOGIC_HIGH){
				LCD_displayString("Air_Cond.  s:ON");
			}
			else{
				LCD_displayString("Air_Cond. s:OFF");
			}
			/*move cursor of LCD to row 1 */
			LCD_moveCursor(1,0);
			LCD_displayString("1:on 2-off 0:RET");
			req_state=KEYPAD_getPressedKey();
			_delay_ms(100);
			SPI_sendReciveByte(req_state);

		}
	}while (((Air_option !=0)  && (Air_option != 2))||(req_state==0));
	return Air_option;
}



