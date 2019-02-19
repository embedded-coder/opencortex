/*****************************************************************************************
* @file               : button.c
* @author             : ayangs
* @date               : 2017/03/25
* @brief              : This file contains all the common data types used for the
*                       STM32L152 clock configuration.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "button.h"
#include "errno.h"
#include "assert.h"
/*macros --------------------------------------------------------------------------------*/
#define button_event_cb(button) \
do{\
	if(button->cb[button->event])\
	{\
		button->cb[button->event]((button_t*)button);\
	}\
}while(0);

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/
static button_t* button_head = null;

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/

/*public --------------------------------------------------------------------------------*/
uint32_t button_init(button_t *newbutton)
{
	assert_return_err(newbutton, button_err_parameter);

	button_t *button = button_head;
	while(button)
	{
		if(button == newbutton)
			return button_err_parameter;

		button = button->next;
	}
	newbutton->next = button_head;
	button_head     = newbutton;

	gpio_init(newbutton->gpio);

	return success;
}

uint32_t button_attach(button_t *button, button_event_e event, button_callback callback)
{
	assert_return_err(button, button_err_parameter);
	assert_return_err(event < button_event_max, button_err_parameter);

	button->cb[event] = callback;
	return success;
}

uint32_t button_dettach(button_t *button, button_event_e event)
{
	assert_return_err(button, button_err_parameter);
	assert_return_err(event < button_event_max, button_err_parameter);

	button->cb[event] = null;
	return success;
}

uint32_t button_get_status(void)
{
	button_t *button        = null;
	uint8_t   button_status = 0;
	
	for(button = button_head; button != null; button = button->next)
	{
		gpio_get(button->gpio, &button_status);

		button->status.raw = button_status & 1;

		if(1 == button->flags.active_low)
			button->status.raw = button->status.raw ^ 1;

		button->status.trig = button->status.raw & (button->status.raw ^ button->status.cont);
		button->status.cont = button->status.raw;
	}

	return success;
}


uint32_t button_event_process(void)
{
	button_t *button = null;

	for(button = button_head; button != null; button = button->next)
	{
		switch(button->state)
		{
			case button_state_idle:
				if(button->status.trig)//press trigger
				{
					button->clicks++;
					button->ticks  = 0;
					button->event  = button_event_press_edge;
					button->state  = button_state_press_edge;
				}
				else//nothing happened,remain idle
				{
					button->clicks = 0;
					button->ticks  = 0;
					button->event  = button_event_none;
					button->state  = button_state_idle;
				}
			break;
			case button_state_press_check:
				if(!button->status.cont)//new release trigger, recount ticks
				{
					if(button->clicks >= button->multi_clicks)
					{
						button->ticks  = 0;
						button->clicks = 0;
						button->event  = button_event_multi_click;
						button->state  = button_state_multi_click;
					}
					else if(button->ticks >= button->long_click_ticks)
					{
						button->ticks	= 0;
						button->event	= button_event_long_click;
						button->state	= button_state_release_check;
					}
					else if((button->ticks >= button->short_click_ticks)
						  &&(button->ticks <  button->long_click_ticks))
					{
						button->ticks	= 0;
						button->event	= button_event_short_click;
						button->state	= button_state_release_check;
					}
					else if(button->ticks < button->short_click_ticks)//treat as burrs
					{
						button->event = button_event_none;
						button->state = button_state_release_check;
					}

				}
				else//still pressed, count ticks
				{
					button->ticks++;
					
					if((button->ticks >= button->click_press_ticks)
						&&(button->event != button_event_press_delay))
					{
						button->event	= button_event_press_delay;
						button->state	= button_state_press_delay;
					}
					else
					{
						button->state = button_state_press_check;
					}
				}
			break;
			case button_state_release_check:
				if(button->status.trig)//new press trigger, reset ticks
				{
					if(button->event != button_event_none)
					{
						button->clicks++;
						button->ticks = 0;
					}
					else
					{
						//no clicks & ticks continues.
					}
					button->state = button_state_press_check;
				}
				else//still released, count ticks
				{
					button->ticks++;
					
					if(button->ticks > button->click_release_ticks)//no more new press in this operation
					{
						button->ticks	= 0;
						button->clicks	= 0;
						if(button->event == button_event_short_click)
							button->state = button_state_short_click;
						else if(button->event == button_event_long_click)
							button->state = button_state_long_click;
						else
							button->state = button_state_idle;
					}
					else
					{
						button->state = button_state_release_check;//continue to check if there will be new press
					}
				}
			break;
			case button_state_press_edge:
				button_event_cb(button);
				button->state = button_state_press_check;
			break;
			case button_state_press_delay:
				button_event_cb(button);
				button->state = button_state_press_check;
			break;
			case button_state_short_click:
				button_event_cb(button);
				button->state = button_state_idle;
			break;
			case button_state_multi_click:
				button_event_cb(button);
				button->state  = button_state_idle;
			break;
			case button_state_long_click:
				button_event_cb(button);
				button->state  = button_state_idle;
			break;
			default:
			break;
		}
	}
	return success;
}




