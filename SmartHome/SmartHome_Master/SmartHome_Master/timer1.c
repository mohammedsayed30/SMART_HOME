 /**********************************************************
 *
 * Module: TIMER1
 *
 * File Name: timer1.c
 *
 * Description: Source file for the AVR TIMER1 driver
 *
 * Created on:28/10/2022
 *
 * Author: Mohammed Sayed Shaaban
 *
 **********************************************************/
#include "timer1.h"
#include <avr/interrupt.h>
#include <avr/io.h>
/************************************************************
 *                  Global Variables
 ***********************************************************/
/* Global variables to hold the address of the
 * call back function in the application
 */
static volatile void (*g_callBackPtr)(void) =NULL_PTR;

/***********************************************************
 *                Function_Definitions
 **********************************************************/

/*
 * ISR For TIMER1 Comapare_MODE
 */
ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}
/*
 * ISR For TIMER1 OVERFLOW_MODE
 */
ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}

/*
 * Function_Name:Timer1_init
 *
 * Description:
 *
 * to initialize the Timer1 in Normal mode or compare mode
 */

void Timer1_init(const Timer1_ConfigType *Config_ptr){

    if(Config_ptr->mode==NORMAL_MODE){
	    /*set the initial value*/
		TCNT1=Config_ptr->initial_value;
		/*as I don't want generate PWM Signal*/
		TCCR1A=(1<<FOC1A)|(1<<FOC1B);
		/*to active the Timer1_Overflow Interrupt*/
		TIMSK|=(1<<TOIE1);
		/*to set the required prescalar*/
		TCCR1B=(Config_ptr->prescalar);
    }
   else if(Config_ptr->mode==COMPARE_MODE){
    	/*set the initial value*/
		TCNT1=Config_ptr->initial_value;
		/*set the compare value*/
		OCR1A=Config_ptr->compare_value;
		/*as I don't want generate PWM Signal*/
		TCCR1A=(1<<FOC1A)|(1<<FOC1B);
		/*to active the Timer1_COMPARE Interrupt*/
		TIMSK|=(1<<OCIE1A);
		/*to set the required prescalar*/
		TCCR1B=(1<<WGM12)|(Config_ptr->prescalar);
   }
}

/*
 * Function_Name:Timer1_deInit
 *
 * Description:
 *
 * to disable the Timer1( stop the Timer1 from counting)
 */
void Timer1_deInit(void){
	/*Close the Clock Source to Stop The Timer*/
	TCCR1B=0;
}

/*
 * Function_Name:Timer1_setCallBack
 *
 * Description:
 *
 * to set the address of call back function the response
 * for handling the code of ISR
 */
void Timer1_setCallBack(void(*a_ptr)(void)){
	/*
	 * set the address of the
	 * call back function in the application
	 */
	g_callBackPtr=a_ptr;
}

