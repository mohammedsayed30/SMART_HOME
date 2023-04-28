 /***************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.h
 *
 * Description: Header file for the Keypad driver
 *
 * Author: Mohammed Sayed Shaaban
 ****************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"

/*****************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/*
 * configuration of Keypad model
 * */
#define ROWS_NUMBER                       4
#define COLUMNS_NUMBER                    4
/*POrts and pins for rows and columns for keypad*/
#define KEYPAD_ROWS_PORT_ID              PORTD_ID
#define KEYPAD_ROWS_FIRST_PIN_ID         PIN0_ID
#define KEYPAD_COLUMNS_PORT_ID           PORTD_ID
#define KEYPAD_COLUMNS_FIRST_PIN_ID      PIN4_ID
/*to dictated the Button is pressed or released */
typedef enum{
	button_pressed,button_released
}button_state;

/*****************************************************************************
 *                                Prototypes                                  *
 *****************************************************************************/
/*
 * Function Name:KEYPAD_getButtonPressed
 * Description:
 * get the number of the button that pressed
 */
uint8 KEYPAD_getPressedKey(void);
#endif /* KEYPAD_H_ */
