/*
 * adc.h
 *
 *  Created on: ٠٣‏/١٠‏/٢٠٢٢
 *  Author: Mohammed sayed shaaban
 */

#ifndef ADC_H_
#define ADC_H_
#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define ADC_MAXIMUM_VALUE    1023
#define ADC_REF_VOLT         2.56
typedef uint8                ADC_Prescalar;
/*enumeration for the reference Voltage*/
typedef enum{
	AREF_OFF,AVCC,INTERNAL_AREF
}ADC_ReverenceVolatge;
/*
 * Stucture name:ADC_ConfigType
 * To hold the required configurations
 * that response for initialize the ADC
 */
typedef struct{
	ADC_ReverenceVolatge ref_volt;  /*to hold the reference voltage*/
	ADC_Prescalar prescalar;        /*to hold the prescalar*/
}ADC_ConfigType;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Function name : ADC_Init
 * Description :
 * to initialize the ADC driver.
 * INput  ---> address of structure that hold the data for ADC
 */
void ADC_init(const ADC_ConfigType* Config_ptr);

/*
 * Function Name:-ADC_readChannel
 * Description :
 * Function responsible for read analog data from the world
 * from a specific pin
 * INput---->no of the channel that you wanna read it
 * return the value in digital form
 */
uint16 ADC_readChannel(uint8 num_of_channel);

#endif /* ADC_H_ */
