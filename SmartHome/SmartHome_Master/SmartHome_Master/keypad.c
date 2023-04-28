 /***************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.c
 *
 * Description: Source file for the Keypad driver
 *
 * Author: Mohammed Sayed Shaaban
 ****************************************************************************/
#include "keypad.h"
#include "gpio.h"
#include <util/delay.h>

#ifndef STANDARD_KEYPAD
#if(COLUMNS_NUMBER==3)
/*to hand specials cases like(#,*) with 4x3 keypad*/
static uint8 KEYPAD_4X3_adjustKeyNumber(uint8);        /*private prototypes*/
#elif(COLUMNS_NUMBER==4)
/*to hand specials cases like(+,%) with 4x4 keypad*/
static uint8 KEYPAD_4X4_adjustKeyNumber(uint8);      /*private prototypes*/
#endif
#endif    /*for STANDARD_KEYPAD*/

/*
 * Function Name:KEYPAD_getButtonPressed
 * Description:
 * get the number of the button that pressed
 */
uint8 KEYPAD_getPressedKey(void){
	uint8 row_num,col_num;    /*indicate the number of row and number of column*/
	/*setup the rows_pins as input pin*/
	GPIO_setupPinDirection(KEYPAD_ROWS_PORT_ID,KEYPAD_ROWS_FIRST_PIN_ID ,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROWS_PORT_ID,KEYPAD_ROWS_FIRST_PIN_ID+1 ,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROWS_PORT_ID,KEYPAD_ROWS_FIRST_PIN_ID+2 ,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROWS_PORT_ID,KEYPAD_ROWS_FIRST_PIN_ID+3 ,PIN_INPUT);
	/*setup the columns_pins as input pin*/
	GPIO_setupPinDirection(KEYPAD_COLUMNS_PORT_ID,KEYPAD_COLUMNS_FIRST_PIN_ID ,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COLUMNS_PORT_ID,KEYPAD_COLUMNS_FIRST_PIN_ID+1 ,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COLUMNS_PORT_ID,KEYPAD_COLUMNS_FIRST_PIN_ID+2 ,PIN_INPUT);
#if(COLUMNS_NUMBER==4)
	GPIO_setupPinDirection(KEYPAD_COLUMNS_PORT_ID,KEYPAD_COLUMNS_FIRST_PIN_ID+3 ,PIN_INPUT);
#endif
	while(1){
		for(row_num=0;row_num<ROWS_NUMBER;row_num++){
			/*set the one row as ouput pin each time*/
			GPIO_setupPinDirection(KEYPAD_ROWS_PORT_ID,KEYPAD_ROWS_FIRST_PIN_ID+row_num,PIN_OUTPUT);
			/*write Zero to this row(pressed)*/
			GPIO_writePin(KEYPAD_ROWS_PORT_ID,KEYPAD_ROWS_FIRST_PIN_ID+row_num,button_pressed);
			for(col_num=0;col_num<COLUMNS_NUMBER;col_num++){
				if(GPIO_readPin(KEYPAD_COLUMNS_PORT_ID,KEYPAD_COLUMNS_FIRST_PIN_ID+col_num)==button_pressed){
					
                    #if(COLUMNS_NUMBER==3)
                        #ifdef STANDARD
                	       return ((row_num * COLUMNS_NUMBER)+col_num+1);
                        #else
                	       return KEYPAD_4X3_adjustKeyNumber((row_num * COLUMNS_NUMBER)+col_num+1);
                        #endif
                    #elif(COLUMNS_NUMBER==4)
                        #ifdef STANDARD
                           return ((row_num * COLUMNS_NUMBER)+col_num+1);
                        #else
                           return KEYPAD_4X4_adjustKeyNumber((row_num * COLUMNS_NUMBER)+col_num+1);
                       #endif
                    #endif

				}
			}
			/*back the row as input pin again*/

			GPIO_setupPinDirection(KEYPAD_ROWS_PORT_ID,KEYPAD_ROWS_FIRST_PIN_ID+row_num,PIN_INPUT);
		}
	}
}

#ifndef STANDARD_KEYPAD
#if(COLUMNS_NUMBER==3)
/*to hand specials cases like(#,*) with 4x3 keypad*/
static uint8 KEYPAD_4X3_adjustKeyNumber(uint8 num){

	switch(num){
	case 10:
		num='*';
		break;
	case 11:
	   num=0;
	   break;
	case 12:
		num='#';
		break;
	}
	return num;
}
#elif(COLUMNS_NUMBER==4)
/*to hand specials cases like(+,%) with 4x4 keypad*/
static uint8 KEYPAD_4X4_adjustKeyNumber(uint8 num){

	switch(num){
	case 1:
		num = 7;
		break;
	case 2:
		num = 8;
		break;
	case 3:
		num = 9;
		break;
	case 4:
		num = '/';
		break;
	case 5:
		num = 4;
		break;
	case 6:
		num = 5;
		break;
	case 7:
		num = 6;
		break;
	case 8:
		num = '*';
		break;
	case 9:
		num = 1;
		break;
	case 10:
		num = 2;
		break;
	case 11:
		num = 3;
		break;
	case 12:
		num = '-';
		break;
	case 13:
		num = 13;     /*ascii of enter*/
		break;
	case 14:
		num = 0;
		break;
	case 15:
		num = '=';
		break;
	case 16:
		num = '+';
		break;
	}
	return num;

}
#endif
#endif    /*for STANDARD_KEYPAD*/

