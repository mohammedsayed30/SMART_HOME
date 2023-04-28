 /***********************************************************************
 *
 * Module: LCD
 *
 * File Name: LCD.c
 *
 * Description: Source file for the AVR LCD driver
 *
 * Author: Mohammed Sayed Shaaban
 *
 *************************************************************************/
#include "common_macros.h"
#include "LCD.h"
#include <util/delay.h>
#include "gpio.h"
/* initialize LCD */
void LCD_init(void){
	GPIO_setupPinDirection(LCD_RS_PORT_ID, LCD_RS_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_EN_PORT_ID,LCD_EN_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_EN_PORT_ID,LCD_EN_PIN_ID+2,PIN_OUTPUT);
	 GPIO_writePin(LCD_EN_PORT_ID,LCD_EN_PIN_ID+2,LOGIC_LOW);
    _delay_ms(20);
#if(LCD_BIT_DATA_MODE == 4)
    /* set the four bits of LCD as output bin */
    GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,PIN_OUTPUT);
    GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,PIN_OUTPUT);
    /* sending the four bits_command initialization*/
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);
	/* choose the mode */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BIT_MODE);
#elif(LCD_BIT_DATA_MODE == 8)
	/* Set the Port as output */
	GPIO_setupPortDirection(LCD_DATA_PORT_ID,PORT_OUTPUT);
	/* choose the mode ...two lines and EIGHT MODE*/
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BIT_MODE);
#endif

	LCD_sendCommand(LCD_CURSOR_OFF); /* cursor off */
	LCD_sendCommand(LCD_CLEAR_DISPLAY); /* clear LCD at the beginning */
}
void LCD_sendCommand(uint8 command){
	GPIO_writePin(LCD_RS_PORT_ID ,LCD_RS_PIN_ID,LOGIC_LOW);
	_delay_ms(1);       /*delay for tas = 50ns*/
	GPIO_writePin(LCD_EN_PORT_ID ,LCD_EN_PIN_ID,LOGIC_HIGH);
	_delay_ms(1);    /*delay  process for tps-tdsw=290-190=100*/
#if(LCD_BIT_DATA_MODE == 4)
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(command,4));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(command,5));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(command,6));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(command,7));
	_delay_ms(1);   /*delay for tdsw=100ns*/
	GPIO_writePin(LCD_EN_PORT_ID ,LCD_EN_PIN_ID,LOGIC_LOW);
	_delay_ms(1);       /*delay for th=13ns */
	GPIO_writePin(LCD_EN_PORT_ID ,LCD_EN_PIN_ID,LOGIC_HIGH);
	_delay_ms(1);         /*delay  process for tps-tdsw=290-190=100*/
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(command,0));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(command,1));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(command,2));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(command,3));
	_delay_ms(1);           /*delay for tdsw=100ns*/
	GPIO_writePin(LCD_EN_PORT_ID ,LCD_EN_PIN_ID,LOGIC_LOW);
	_delay_ms(1);           /*delay for th=13ns */
#elif(LCD_BIT_DATA_MODE == 8)
	GPIO_writePort(LCD_DATA_PORT_ID,command);
	_delay_ms(1);         /*delay for tdsw=100ns*/
	GPIO_writePin(LCD_EN_PORT_ID ,LCD_EN_PIN_ID,LOGIC_LOW);
	_delay_ms(1);           /*delay for th=13ns */
#endif
}

void LCD_displayCharacter(uint8 character){
	GPIO_writePin(LCD_RS_PORT_ID ,LCD_RS_PIN_ID,LOGIC_HIGH);
	_delay_ms(1);       /*delay for tas = 50ns*/
	GPIO_writePin(LCD_EN_PORT_ID ,LCD_EN_PIN_ID,LOGIC_HIGH);
	_delay_ms(1);    /*delay  process for tps-tdsw=290-190=100*/
#if(LCD_BIT_DATA_MODE == 4)
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(character,4));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(character,5));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(character,6));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(character,7));
	_delay_ms(1);   /*delay for tdsw=100ns*/
	GPIO_writePin(LCD_EN_PORT_ID ,LCD_EN_PIN_ID,LOGIC_LOW);
	_delay_ms(1);       /*delay for th=13ns */
	GPIO_writePin(LCD_EN_PORT_ID ,LCD_EN_PIN_ID,LOGIC_HIGH);
	_delay_ms(1);         /*delay  process for tps-tdsw=290-190=100*/
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(character,0));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(character,1));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(character,2));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(character,3));
	_delay_ms(1);           /*delay for tdsw=100ns*/
	GPIO_writePin(LCD_EN_PORT_ID ,LCD_EN_PIN_ID,LOGIC_LOW);
	_delay_ms(1);           /*delay for th=13ns */
#elif(LCD_BIT_DATA_MODE == 8)
	GPIO_writePort(LCD_DATA_PORT_ID,character);
	_delay_ms(1);         /*delay for tdsw=100ns*/
	GPIO_writePin(LCD_EN_PORT_ID ,LCD_EN_PIN_ID,LOGIC_LOW);
	_delay_ms(1);           /*delay for th=13ns */
#endif
}
/*display the string to screen*/
void LCD_displayString(const uint8 * str){
	uint8 i=0;
	while(str[i]!='\0'){
		LCD_displayCharacter(str[i]);       //call the function display
		i++;
	}
}
/* Move the Cursor to specific location*/
void LCD_moveCursor(uint8 row,uint8 col){
	uint8 location;
	/* configuration to detect the specific location*/
	switch(row){
	case 0:
		location=col;
		break;
	case 1:
	   location=col+0x40;
	   break;
	case 2:
	   location=col+0x10;
	   break;
	case 3:
	   location=col+0x50;
	   break;
	}
	/* LCD_SET_CURSOR_LOCATION ---> to the LCD_sendCommand under stand that is address*/
	LCD_sendCommand(location | LCD_SET_CURSOR_LOCATION );
}
/* display string at begin of specific location*/
void LCD_displayStringRowColumn(uint8 row,uint8 col,uint8* st_row_col){
	LCD_moveCursor(row,col);
	LCD_displayString(st_row_col);
}
/* to display the numbers*/
void LCD_intgerToString(int num){
	 char buff[16];              /* String to hold the ascii result */
	 itoa(num,buff,10);          /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
	 LCD_displayString(buff);    /* Display the string */
}
/*to clear the screen*/
void LCD_clearScreen(void){
	LCD_sendCommand(LCD_CLEAR_DISPLAY);
}
/*the End*/
