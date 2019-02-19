/*************************************************************************************
* Test-program for Olimex “STM32-H103”, header board for “STM32F103RBT6”.
* After program start green LED (LED_E) will blink.
*
* Program has to be compiled with optimizer setting "-O0".
* Otherwise delay via while-loop will not work correctly.
*************************************************************************************/

#include "delay.h"
#include "platform.h"

void example_process(void)
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
