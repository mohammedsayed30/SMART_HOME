 /***********************************************************************
 *
 * Module: ADC
 *
 * File Name: adc.c
 *
 * Description: Source file for the ATmega32 ADC driver
 *
 * Author: Mohammed Sayed Shaaban
 *
 * Created on : 5/10/2022
 *
 ***********************************************************************/

#include <avr/io.h>          /* To use the ADC Registers */
#include "adc.h"
#include "common_macros.h"   /* To use the preprocessor macros */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * to initialize the ADC driver.
 */
void ADC_init(const ADC_ConfigType* Config_ptr)
{
	/*configuration of ADC with polling*/
	if(Config_ptr->ref_volt==INTERNAL_AREF){
		ADMUX = (1<<REFS0)|(1<<REFS1);
	}
	else if(Config_ptr->ref_volt==AVCC){
		ADMUX = (1<<REFS0);
	}
	else{
		ADMUX=0;
	}
	switch(Config_ptr->prescalar){
	case 2:                           /*prescalar (2)*/
		ADCSRA =(1<<ADEN);
		break;
	case 4:                           /*prescalar (4)*/
		ADCSRA =(1<<ADPS1)|(1<<ADEN);
		break;
	case 8:                            /*prescalar (8)*/
		ADCSRA =(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
		break;
	case 16:                            /*prescalar (16)*/
		ADCSRA =(1<<ADPS2)|(1<<ADEN);
		break;
	case 32:                            /*prescalar (32)*/
		ADCSRA =(1<<ADPS0)|(1<<ADEN);
		break;
	case 64:                            /*prescalar (64)*/
		ADCSRA =(1<<ADPS1)|(1<<ADPS2)|(1<<ADEN);
		break;
	case 128:                            /*prescalar (128)*/
		ADCSRA =(1<<ADPS1)|(1<<ADPS0)|(1<<ADPS2)|(1<<ADEN);
		break;
	}

}

uint16 ADC_readChannel(uint8 num_of_channel)
{
	ADMUX= ADMUX &(0xE0);                  /*as we need only the five least bits*/
	num_of_channel=num_of_channel &(0x07);  /*as 7_bins can represent in  only three  bits*/
	ADMUX|=num_of_channel;                 /*to put the num of channel to MUX0-->MUX4*/
	SET_BIT(ADCSRA,ADSC);                  /*Start conversion data from the analog to digital*/
	while(BIT_IS_CLEAR(ADCSRA,ADIF));     /*polling(waiting for complete the conversion*/
	SET_BIT(ADCSRA,ADIF);               /*clear the flag to the next time*/
	return ADC;                         /*return the required data*/
}
