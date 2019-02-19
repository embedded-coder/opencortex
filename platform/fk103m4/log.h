/*****************************************************************************************
* @file               : uart.h
* @author             : ayangs
* @date               : 2017/03/25
* @brief              : This file contains all the common data types used for the
*                       STM32L152 clock configuration.
******************************************************************************************/

#ifndef _log_h_
#define _log_h_
/*includes ------------------------------------------------------------------------------*/
#include "typedef.h"
#include "uart.h"
/*macros --------------------------------------------------------------------------------*/
#define tags      bit(0)

#define verbose   0   // all
#define debug     1   // debug
#define info      2   // information
#define warning   3   // warnings
#define error     4   // errors
#define fatal     5   // fatal errors
#define silent    6   // silient

#define log_level   verbose

/*typedefs ------------------------------------------------------------------------------*/
typedef struct log{
	char*   name;
	uart_t* uart;
	uint8_t level;
}log_t;
/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

uint32_t log_init(log_t* log);

uint32_t log_str(uint8_t level, uint32_t flags, char *s);

uint32_t log_dec(uint8_t level, uint32_t flags, uint32_t l, uint8_t len);



#endif //_log_h_

