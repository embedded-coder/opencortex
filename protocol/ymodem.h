/*****************************************************************************************
* @file               : ymodem.h
* @author             : ayangs
* @date               : 2019/10/06
* @brief              : Head files for ymodem.
******************************************************************************************/
#ifndef _ymodem_h_
#define _ymodem_h_
/*includes ------------------------------------------------------------------------------*/
#include "platform.h"
#include "typedef.h"
/*macros --------------------------------------------------------------------------------*/
#define y_soh (0x01)  //Start Of Header (128 bytes).
#define y_stx (0x02)  //Start Of Header (1024 bytes).
#define y_eot (0x04)  //End Of Transmission.
#define y_ack (0x06)  //Acknowledge.
#define y_nak (0x15)  //Not Acknowledge.
#define y_can (0x18)  //Cancel.
#define y_c16 (0x43)  //ASCII "C", to notify the host, we want to use CRC16.

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/
uint32_t ymodem_init(uart_t* uart);

uint32_t ymodem_receive(void);

uint32_t ymodem_rcv_file_info_handler(uint32_t size);

uint32_t ymodem_rcv_file_data_handler(uint32_t base, uint32_t *data,uint32_t size);

uint32_t ymodem_rcv_file_dump_handler(uint32_t size);

#endif //_ymodem_h_


