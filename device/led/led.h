/*****************************************************************************************
* @file               : led.h
* @author             : ayangs
* @date               : 2017/03/25
* @brief              : This file contains all the common data types used for the
*                       STM32L152 clock configuration.
******************************************************************************************/
#ifndef _led_h_
#define _led_h_
/*includes ------------------------------------------------------------------------------*/
#include "gpio.h"
/*macros --------------------------------------------------------------------------------*/
#define led_off		0
#define led_on		1

#define led_flags_active_low		bit(0)

/*typedefs ------------------------------------------------------------------------------*/
typedef struct led{
	char*   name;
	gpio_t* gpio;
	uint8_t brightness;
	uint8_t flags;
}led_t;

/*variables -----------------------------------------------------------------------------*/


/*prototypes ----------------------------------------------------------------------------*/
uint32_t led_init(led_t *led);
uint32_t led_set_brightness(led_t *led, uint8_t *brightness);
uint32_t led_get_brightness(led_t *led, uint8_t *brightness);


#endif //_led_h_

