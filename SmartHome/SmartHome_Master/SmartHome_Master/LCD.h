 /**********************************************************************
 *
 * Module: LCD
 *
 * File Name: LCD.h
 *
 * Description: Header file for the AVR LCD driver
 *
 * Author: Mohammed Sayed Shaaban
 *
 ***********************************************************************/

#ifndef LCD_H_
#define LCD_H_
#include "std_types.h"
/***********************************************************************
 *                                Definitions                          *
*************************************************************************/
/* LCD Bits Configuration mode (4 or 8 bit modes) */
#define LCD_BIT_DATA_MODE 8
#if ((LCD_BIT_DATA_MODE) !=4 && (LCD_BIT_DATA_MODE) !=8)
#error "THE MODE SHOULD BE 4 OR 8"
#endif
/* LCD HW Ports and Pins Ids Configuration */
#define LCD_RS_PORT_ID     PORTB_ID
#define LCD_RS_PIN_ID      PIN1_ID
#define LCD_EN_PORT_ID     PORTB_ID
#define LCD_EN_PIN_ID      PIN0_ID
#define LCD_DATA_PORT_ID   PORTA_ID
/* configuration of 4 bit mode */
#if (LCD_BIT_DATA_MODE==4)
#define LCD_DB4_PIN_ID   PIN3_ID
#define LCD_DB5_PIN_ID   PIN4_ID
#define LCD_DB6_PIN_ID   PIN5_ID
#define LCD_DB7_PIN_ID   PIN6_ID
#endif
/* LCD COMMANDS */

#define LCD_CLEAR_DISPLAY                    0x01
#define LCD_RETURN_HOME                      0x02
#define LCD_CURSOR_ON                        0x0E
#define LCD_CURSOR_OFF                       0x0C
#define LCD_TWO_LINES_FOUR_BIT_MODE          0x28
#define LCD_TWO_LINES_EIGHT_BIT_MODE         0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   0x32
#define LCD_SET_CURSOR_LOCATION              0x80

/************************************************************************
*                       FUNCTIONS PROTOTYPES                             *
***********************************************************************/

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void);

/*
 * Description :
 * Send commands to  the LCD:
 */
void LCD_sendCommand(uint8 command);

/*
 * Description :
 * display a character to  the LCD:
 */
void LCD_displayCharacter(uint8 character);
/*
 * Description :
 * display a String to  the LCD:
 */
void LCD_displayString(const uint8 * str);

/*
 * Description :
 * Move the LCD's Cursor:
 */
void LCD_moveCursor(uint8 row,uint8 col);

/*
 * Description :
 * display a StringRowColumn to  the LCD:
 */
void LCD_displayStringRowColumn(uint8 row,uint8 col,uint8* st_row_col);

/*
 * Description :
 * to clear the screen of the LCD:
 */

void LCD_clearScreen(void);

/*
 * Description :
 * to convert an integer to string
 */
void LCD_intgerToString(int num);
#endif
