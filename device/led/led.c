/*****************************************************************************************
* @file               : led.c
* @author             : ayangs
* @date               : 2017/03/25
* @brief              : This file contains all the common data types used for the
*                       STM32L152 clock configuration.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "led.h"
#include "errno.h"
#include "assert.h"

/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/

/*public --------------------------------------------------------------------------------*/

uint32_t led_init(led_t *led)
{
	assert_return_err(led,led_err_parameter);

	uint8_t active_brightness = led->brightness;

	if(led->flags & led_flags_active_low)
		active_brightness = led->brightness ^ 1;

	gpio_init(led->gpio);

	gpio_set(led->gpio, &active_brightness);

	return success;
}

uint32_t led_set_brightness(led_t *led, uint8_t *brightness)
{
	assert_return_err(led,led_err_parameter);

	led->brightness = *brightness;

	uint8_t active_brightness = led->brightness;

	if(led->flags & led_flags_active_low)
		active_brightness = led->brightness ^ 1;

	gpio_set(led->gpio, &active_brightness);
	
	return success;
}

uint32_t led_get_brightness(led_t *led, uint8_t *brightness)
{
	assert_return_err(led,led_err_parameter);

	*brightness = led->brightness;

	return success;
}

