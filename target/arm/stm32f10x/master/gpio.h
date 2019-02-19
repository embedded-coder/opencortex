/*****************************************************************************************
* @file               : gpio.h
* @author             : ayangs
* @date               : 2017/03/25
* @brief              : This file contains all the common data types used for the
*                       STM32L152 clock configuration.
******************************************************************************************/
#ifndef _gpio_h_
#define _gpio_h_
/*includes ------------------------------------------------------------------------------*/
#include "typedef.h"
/*macros --------------------------------------------------------------------------------*/
#define port(x)    ((x)-'A')

#define high	1
#define low	    0

/*typedefs ------------------------------------------------------------------------------*/
typedef struct gpio{
	char*   name;
	uint8_t value;
	uint8_t port;
	uint8_t pin;
	uint8_t dir;
}gpio_t;

typedef enum gpio_dir{
	in_analog,
	in_float,
	in_pulldown,
	in_pullup,
	out_opendrain,
	out_pushpull,
	out_opendrain_mux,
	out_pushpull_mux,
}gpio_dir_e;
/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/
uint32_t gpio_init(gpio_t *gpio);

uint32_t gpio_set(gpio_t *gpio, uint8_t *value);

uint32_t gpio_get(gpio_t *gpio, uint8_t *value);


#endif //_gpio_h_


