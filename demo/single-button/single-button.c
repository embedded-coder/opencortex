/*****************************************************************************************
* @file               : single-button.c
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Source files for single-button.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "platform.h"

/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/
void usrbtn_short_click_handler(void *button)
{
	button_t * btn = (button_t *)button;
	log_out(info,"%s ,short click!\n\r", btn->name);
}

void usrbtn_multi_click_handler(void *button)
{
	button_t * btn = (button_t *)button;
	log_out(info,"%s ,multi(%d) click!\n\r", btn->name, btn->multi_clicks);
}

void usrbtn_long_click_handler(void *button)
{
	button_t * btn = (button_t *)button;
	log_out(info,"%s ,long click!\n\r", btn->name);
}

void usrbtn_press_edge_handler(void *button)
{
	button_t * btn = (button_t *)button;
	log_out(info,"%s ,press edge!\n\r", btn->name);
}

void usrbtn_press_delay_handler(void *button)
{
	button_t * btn = (button_t *)button;
	log_out(info,"%s ,press delay!\n\r", btn->name);
}

/*public --------------------------------------------------------------------------------*/

void example_process(void)
{
	button_attach(&buttons[0],button_event_short_click,usrbtn_short_click_handler);
	button_attach(&buttons[0],button_event_multi_click,usrbtn_multi_click_handler);
	button_attach(&buttons[0],button_event_long_click, usrbtn_long_click_handler);
	button_attach(&buttons[0],button_event_press_delay,usrbtn_press_delay_handler);
	button_attach(&buttons[0],button_event_press_edge, usrbtn_press_edge_handler);
	//button_dettach(&buttons[0],button_event_press_edge);
	//button_dettach(&buttons[0],button_event_press_delay);
	
	while (1)
	{
		button_get_status();
		button_event_process();
		delay_ms(20);
	}
}

