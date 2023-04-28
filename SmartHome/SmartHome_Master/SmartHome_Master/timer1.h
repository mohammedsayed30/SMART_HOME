 /**********************************************************
 *
 * Module: TIMER1
 *
 * File Name: timer1.h
 *
 * Description: Header file for the AVR TIMER1 driver
 *
 * Created on:28/10/2022
 *
 * Author: Mohammed Sayed Shaaban
 *
 ***********************************************************/

#ifndef TIMER1_H_
#define TIMER1_H_
#include "std_types.h"

/************************************************************
 *                        Definitions
 ***********************************************************/



/*
 * Name:Timer1_Mode enumeration
 *
 * Description:
 *
 * to detect the mode of Timer1(Overflow or Compare) Mode
 */
typedef enum{
	NORMAL_MODE,COMPARE_MODE
}Timer1_Mode;



/*
 * Name:Timer1_Prescalar enumeration
 *
 * Description:
 *
 * to detect the Required prescalar of Timer1
 */
typedef enum{
	NO_CLK,CLK_1,CLK_8,CLK_64,CLK_256,CLK_1024
}Timer1_Prescalar;


/*
 * Structions_Name:Timer1_ConfigType
 *
 * Description:
 *
 * hold the required Configuration of Timer1 Like:
 * 1-Initial value that timer start count from it
 * 2-compare value that timer stop to it(compare mode )
 * 3-The Required mode (OverFlow or Compare)
 * 4-detect The Required Prescelar
 */

typedef struct{
	uint16 initial_value;
	uint16 compare_value;
	Timer1_Mode mode;
	Timer1_Prescalar prescalar;
}Timer1_ConfigType;


/************************************************************
*                    Functions_Prototypes
 ***********************************************************/
/*
 * Function_Name:Timer1_init
 *
 * Description:
 *
 * to initialize the Timer1 in Normal mode or compare mode
 */
void Timer1_init(const Timer1_ConfigType *Config_ptr);

/*
 * Function_Name:Timer1_deInit
 *
 * Description:
 *
 * to disable the Timer1( stop the Timer1 from counting)
 */
void Timer1_deInit(void);

/*
 * Function_Name:Timer1_setCallBack
 *
 * Description:
 *
 * to set the address of call back function the response
 * for handling the code of ISR
 */
void Timer1_setCallBack(void(*a_ptr)(void));
#endif /* TIMER1_H_ */
