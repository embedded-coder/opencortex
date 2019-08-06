/*****************************************************************************************
* @file               : platform.c
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Source files for platform.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "platform.h"

/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/* MASTERs */
gpio_t gpios[] = {
	{"GpioC0", low, port('C'), 0,  out_pushpull},		//led
	{"GpioC1", low, port('C'), 1,  in_pullup },			//buttons
	{"GpioA9", low, port('A'), 9,  out_pushpull_mux},	//uart1 tx
	{"GpioA10",low, port('A'), 10, in_float}			//uart1 rx
};
uart_t uarts[] = {
	{"uart", &gpios[2], &gpios[3], 115200, datawidth_8b, parity_none, stopbits_1, flowctrl_none, 0, 0}
};

/* DEVICEs */
led_t leds[] = {
	{"Led0", &gpios[0], led_off, led_flags_active_low}
};

button_t buttons[] = {
	{
		.name                = "usrBtn",
		.gpio                = &gpios[1],
		.flags.active_low    = 1,
		.multi_clicks        = 2,
		.short_click_ticks   = 2,
		.long_click_ticks    = 20,
		.click_release_ticks = 10,
		.click_press_ticks   = 15,
	}
};
log_t logs = {
	.name = "Log", 
	.uart = &uarts[0], 
	.level = log_level
};

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/

/*public --------------------------------------------------------------------------------*/

void platform_init(void)
{
	SystemInit();

	delay_init(1000);

	log_init(&logs);
	
	for(uint8_t i = 0; i < dim(leds); i++)
	{
		led_init(&leds[i]);
	}
	
	for(uint8_t i = 0; i < dim(buttons); i++)
	{
		button_init(&buttons[i]);
	}

}

