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
	{//led
		.name  = "GpioC0", 
		.value = low,
		.port  = port('C'),
		.pin   = 0,
		.dir   = out_pushpull
	},
	{//buttons
		.name  = "GpioC1",
		.value = low,
		.port  = port('C'), 
		.pin   = 1,
		.dir   = in_pullup
	},
	{//uart1 tx
		.name  = "GpioA9",
		.value = low,
		.port  = port('A'),
		.pin   = 9,
		.dir   = out_pushpull_mux
	},
	{//uart1 rx
		.name  = "GpioA10",
		.value = low,
		.port  = port('A'),
		.pin   = 10,
		.dir   = in_float
	}
};

uart_t uarts[] = {
	{
		.name      = "uart",
		.txpin     = &gpios[2],
		.rxpin     = &gpios[3],
		.baudrate  = 115200,
		.datawidth = datawidth_8b,
		.parity    = parity_none,
		.stopbit   = stopbits_1,
		.flowctrl  = flowctrl_none,
		.irq       = irq_rxne,
		.port      = 0,
		.value     = 0
	}
};

/* DEVICEs */
led_t leds[] = {
	{
		.name       = "Led0",
		.gpio       = &gpios[0],
		.brightness = led_off,
		.flags      = led_flags_active_low
	}
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
		.name  = "Log", 
		.uart  = &uarts[0], 
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

