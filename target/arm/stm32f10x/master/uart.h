/*****************************************************************************************
* @file               : uart.h
* @author             : ayangs
* @date               : 2017/03/25
* @brief              : This file contains all the common data types used for the
*                       STM32L152 clock configuration.
******************************************************************************************/

#ifndef _uart_h_
#define _uart_h_
/*includes ------------------------------------------------------------------------------*/
#include "gpio.h"
#include "typedef.h"
/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/
typedef struct uart{
	char*   name;
	gpio_t  *txpin;
	gpio_t  *rxpin;
	uint32_t baudrate;
	uint8_t datawidth;
	uint8_t parity;
	uint8_t stopbit;
	uint8_t flowctrl;
	uint8_t port;
	char    value;
}uart_t;

typedef enum uart_attr{
	/*data width*/
	datawidth_8b,
	datawidth_9b,
	/*stop bits*/
	stopbits_0p5,
	stopbits_1,
	stopbits_1p5,
	stopbits_2,
	/*parity*/
	parity_none,
	parity_odd,
	parity_even,
	/*flow control*/
	flowctrl_none,
	flowctrl_rts,
	flowctrl_cts,
	flowctrl_rts_cts
}uart_attr_e;

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/
uint32_t uart_init(uart_t *uart);

uint32_t uart_send(uart_t *uart, char c);

uint32_t uart_send_str(uart_t *uart, char *s);

uint32_t uart_send_dec(uart_t *uart, uint32_t l, uint8_t len);


#endif //_uart_h_

