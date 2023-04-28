 /********************************************************************
 * File Name:spi.h
 *
 * Model:-Header File of SPI Driver
 *
 * Created on:12/10/2022
 *
 * Description:-SPI  to serial comunication Protocole
 *
 * Author:Mohammed Sayed Shaaban
 ********************************************************************/

#ifndef SPI_H_
#define SPI_H_

#include "std_types.h"
/****************************************************************************
 *                               Definitions
 ****************************************************************************/

/*
 * Default SPI data value used in case we need to receive a byte from the other device,
 * without need to send a data to it
 */
#define SPI_DEFAULT_DATA_VALUE 0xFF

#define SPI_PORT_ID               PORTB_ID
#define SPI_SS_PIN_ID             PIN4_ID
#define SPI_MOSI_PIN_ID           PIN5_ID
#define SPI_MISO_PIN_ID           PIN6_ID
#define SPI_SCK_PIN_ID            PIN7_ID



/****************************************************************************
 *                               Prototypes
 ****************************************************************************/
/*
 * Function_Name:SPI_initMaster
 *
 * Description:
 *
 *initialize the master
 *
 */

void SPI_initMaster();

/*
 * Function_Name:SPI_initSlave
 *
 * Description:
 *
 *initialize the Slave
 *
 */

void SPI_initSlave();

/*
 * Function_Name:SPI_sendReciveByte
 *
 * Description:
 *
 * to send the reciever byte
 *
 */

uint8 SPI_sendReciveByte(uint8);

/*
 * Function_Name:SPI_sendString
 *
 * Description:
 *
 * to send String
 *
 */

void SPI_sendString(const uint8*);

/*
 * Function_Name:SPI_reciveString
 *
 * Description:
 *
 * to recive String
 *
 */

void SPI_reciveString(uint8*);

#endif /* SPI_H_ */
