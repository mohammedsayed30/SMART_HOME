 /*********************************************************************
 *
 * Module:smart home applications
 *
 * File Name: app2_functions.h
 *
 * Description: header file for the functions application layer
 * for Microcontroller2 for smart home
 *
 * Created on:- 15/2/2022
 *
 * Author: Mohammed Sayed Shaaban
 *
 *********************************************************************/

#ifndef APP2_FUNCTIONS_H_
#define APP2_FUNCTIONS_H_
#include "std_types.h"
#include "gpio.h"
/**********************************************************************
 *                           Definitions
 **********************************************************************/
/*Default temperature*/
#define SET_TEMPERATURE            24
/*the whole home will connected to PORTD*/
#define  ROOMS_PORT                 PORTD_ID
/*
 * Air condition---->PD2
 * TV---->PD3
 * Room1---->PD4
 * Room2---->PD5
 * Room3---->PD6
 * Room4---->PD7
 */
#define  ROOM1_PIN                  PIN4_ID
#define  ROOM2_PIN                  PIN5_ID
#define  ROOM3_PIN                  PIN6_ID
#define  ROOM4_PIN                  PIN7_ID
#define  COND_PIN                   PIN2_ID
#define  TV_PIN                     PIN3_ID

/***********************************************************************
 *                           functions_prototypes
 **********************************************************************/


/*
 * Function_Name:Home_Setup
 *
 * description:setup  the home (rooms & air_condition & TV)
 */

void Home_Setup(void);
/*
 * Function_Name:Object_controle
 *
 * description:to know the state of each object in this home and
 * controlling this object
 */
void Object_controle(uint8,uint8);

/*
 * Function_Name:Air_controle
 *
 * description:to know the state of each object in this home and
 * controlling this object
 */
uint8 Air_controle(uint8,uint8);


#endif /* APP2_FUNCTIONS_H_ */
