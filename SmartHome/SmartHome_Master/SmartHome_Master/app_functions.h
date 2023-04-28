 /*********************************************************************
 *
 * Module:smart home applications
 *
 * File Name: app_functions.h
 *
 * Description: Header file for the functions application prototypes
 *   for smart home
 *
 * Created on:- 15/2/2022
 *
 * Author: Mohammed Sayed Shaaban
 *
 *********************************************************************/

#ifndef APP_FUNCTIONS_H_
#define APP_FUNCTIONS_H_

#include "std_types.h"
#include "gpio.h"

/*********************************************************************
 *                         definitions
 *********************************************************************/
#define ASCII_ZERO      (uint8)'0'
/*define the GUEST Mode and Admin mode*/
#define GUEST       1
#define ADMIN       0
/*the number of digits that represent the password*/
#define NO_OF_DIGIT                           3
/*the first location in the EEPROM for admin_password*/
#define ADMIN_PASS_FIRST_LOCATION             0x0010
/*the first location in the EEPROM for guest_password*/
#define GUEST_PASS_FIRST_LOCATION             0x0016
/*the location that hold value to check if this first time to set password
 *  or not to avoid step1 (set the password if not first time)for admin
 */
#define ADMIN_LOCATION_CHECK                 0x0030
/*the location that hold value to check if this first time to set password
 *  or not to avoid step1 (set the password if not first time)for guest
 */
#define GUEST_LOCATION_CHECK                 0x0031
/*that responsible   for block mode*/
#define BLOCK_LOCATION_CHECK                 0x0040
/*password set or not*/
#define PASS_SET                             (uint8)0x01
/*detect the number of pin in which port for admin_mode */
#define ADMIN_PORT                           PORTC_ID
#define ADMIN_PIN                            PIN0_ID
/*detect the number of pin in which port for Guest_mode */
#define GUEST_PORT                           PORTC_ID
#define GUEST_PIN                            PIN1_ID
/*detect the number of pin in which port for Guest_mode */
#define BLOCK_PORT                           PORTC_ID
#define BLOCK_PIN                            PIN2_ID
/*the number tries for enter passwords*/
#define NUM_TRIES_ALLOWED                    3

/*********************************************************************
 *                               functions_Prototypes
 **********************************************************************/

/*
 * Function_Name:APP_SetCallBack
 *
 * description:this response for some actions when the timer count
 * the required time(40 seconds) in guest mode or (60 seconds) in admin_mode
 * the action-->return to the previously window
 */

void APP_SetCallBack(void);



/*
 * Function_Name:set_password
 *
 * description:set the passwords of admin_mode and guest_mode
 * in the first time only
 *
 */
void set_password(void);

/*
 * Function_Name:select_mode
 *
 * description:select one mode from admin_mode or guest mode
 *
 */
void select_mode(void);

/*
 * Function_Name:check_pass_guest
 *
 * description:to check  the password for guest is correct or not
 * it has one parameter that represent the mode
 */
uint8 check_pass(uint8);





/*
 * Function_Name:wrong_passwords
 *
 * description:shows messages when the password is wrong
 *
 */



void wrong_passwords(void);

/*
 * Function_Name:Admin_access
 *
 * description:to access the whole home (first window)
 *
 */
uint8 Admin_First_access(void);

/*
 * Function_Name:Admin_access
 *
 * description:to access the whole home (second window)
 *
 */
uint8 Admin_Second_access(void);

/*
 * Function_Name:guest_access
 *
 * description:to access the home_rooms only
 *
 */
uint8 Guest_access(void);

/*
 * Function_Name:Blocked_mode
 *
 * description:to stop the system for 25 seconds
 *
 */


void Blocked_mode(void);

/*
 * Function_Name:Guest_menu
 *
 * description:to show the available  menu of guest
 *
 */


uint8 Guest_menu(uint8,uint8);

/*
 * Function_Name:Guest_menu
 *
 * description:to show the available  menu of admin
 *
 */


uint8 Admin1_menu(uint8,uint8);


/*
 * Function_Name:Admin2_menu
 *
 * description:to show the available  menu of admin
 *
 */


uint8 Admin2_menu(uint8,uint8);


/*
 * Function_Name:AirController_fun
 *
 * description:to handle the statements of Air Conditional
 *
 */


uint8 AirController_fun(void);

#endif /* APP_FUNCTIONS_H_ */
