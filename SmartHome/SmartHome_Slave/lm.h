/*
 * Model:lm.h
 *
 * Created on:4/10/2022
 * Author: Mohammed Sayed Shaaban
 */

#ifndef LM_H_
#define LM_H_
#include "std_types.h"
/********************************************************************
 *                       Definitions:-
**********************************************************************/

#define SENSOR_CHANNEL_ID         2
#define SENSOR_MAX_VOLT_VALUE     1.5
#define SENSOR_MAX_TEMPERATURE    150
/*********************************************************************
 *                    Prototypes:-
 *********************************************************************/
uint8 LM35_GetTemperature(void);
#endif /* LM_H_ */
