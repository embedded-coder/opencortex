/*****************************************************************************************
* @file               : led-blink.c
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Source files for led-blink.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "platform.h"

/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/

/*public --------------------------------------------------------------------------------*/

void example_process(void)
{
	while (1)
	{
		static uint8_t led_brightness = led_off;
#if 0
		uint8_t button_status = button_release;
	
		button_get_status(&buttons[0], &button_status);
		
		led_brightness = (button_status == button_press) ? led_on : led_off;
		led_brightness = (button_status == button_press) ? (led_brightness^1) : led_brightness;
#else
		led_brightness = led_brightness^1;
#endif
		led_set_brightness(&leds[0], &led_brightness);
		delay_ms(500);
	}
}
