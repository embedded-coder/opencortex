/*****************************************************************************************
* @file               : platform.c
* @author             : ayangs
* @date               : 2017/03/25
* @brief              : This file contains all the common data types used for the
*                       STM32L152 clock configuration.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "platform.h"

/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/* MASTERs */
gpio_t gpios[]={
	{"GpioC0", low, port('C'), 0,  out_pushpull},		//led
	{"GpioC1", low, port('C'), 1,  in_pullup },			//buttons
	{"GpioA9", low, port('A'), 9,  out_pushpull_mux},	//uart1 tx
	{"GpioA10",low, port('A'), 10, in_float}			//uart1 rx
};
uart_t uarts[]={
	{"uart", &gpios[2], &gpios[3], 115200, datawidth_8b, parity_none, stopbits_1, flowctrl_none, 0, 0}
};

/* DEVICEs */
led_t leds[]={
	{"Led0", &gpios[0], led_off, led_flags_active_low}
};

button_t buttons[]={
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
log_t logs[]={
	{"Log", &uarts[0], log_level}
};

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/

/*public --------------------------------------------------------------------------------*/
void usrbtn_short_click_handler(void *button)
{
	button_t * btn = (button_t *)button;
	log_str(debug, tags, btn->name);
	log_str(debug, null, ",short_click!\n\r");
}

void usrbtn_multi_click_handler(void *button)
{
	button_t * btn = (button_t *)button;
	log_str(debug, tags, btn->name);
	log_str(debug, null, ",multi_click! clicks:");
	log_dec(debug, null, btn->multi_clicks,2);
	log_str(debug, null, "\n\r");
}

void usrbtn_long_click_handler(void *button)
{
	button_t * btn = (button_t *)button;
	log_str(debug, tags, btn->name);
	log_str(debug, null, ",long_click!\n\r");
}

void usrbtn_press_edge_handler(void *button)
{
	button_t * btn = (button_t *)button;
	log_str(debug, tags, btn->name);
	log_str(debug, null, ",press_edge!\n\r");
}

void usrbtn_press_delay_handler(void *button)
{
	button_t * btn = (button_t *)button;
	log_str(debug, tags, btn->name);
	log_str(debug, null, ",press_delay!\n\r");
}

void platform_init(void)
{
	SystemInit();

	delay_init(1000);

	for(uint8_t i = 0; i < dim(leds); i++)
	{
		led_init(&leds[i]);
	}
	for(uint8_t i = 0; i < dim(buttons); i++)
	{
		button_init(&buttons[i]);
		button_attach(&buttons[i],button_event_short_click,usrbtn_short_click_handler);
		button_attach(&buttons[i],button_event_multi_click,usrbtn_multi_click_handler);
		button_attach(&buttons[i],button_event_long_click, usrbtn_long_click_handler);
		button_attach(&buttons[i],button_event_press_delay,usrbtn_press_delay_handler);
		button_attach(&buttons[i],button_event_press_edge, usrbtn_press_edge_handler);
		button_dettach(&buttons[i],button_event_press_edge);
		button_dettach(&buttons[i],button_event_press_delay);
	}

	for(uint8_t i = 0; i < dim(logs); i++)
	{
		log_init(&logs[i]);
	}
}

