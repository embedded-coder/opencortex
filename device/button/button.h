/*****************************************************************************************
* @file               : button.h
* @author             : ayangs
* @date               : 2017/03/25
* @brief              : This file contains all the common data types used for the
*                       STM32L152 clock configuration.
******************************************************************************************/
#ifndef _button_h_
#define _button_h_
/*includes ------------------------------------------------------------------------------*/
#include "gpio.h"
/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/
typedef void (*button_callback)(void *);

typedef enum button_event{
	button_event_none,
	button_event_press_edge,
	button_event_press_delay,
	button_event_short_click,
	button_event_multi_click,
	button_event_long_click,
	button_event_max
}button_event_e;

typedef enum button_state{
	button_state_idle,
	button_state_press_check,
	button_state_release_check,
	button_state_press_edge,
	button_state_press_delay,
	button_state_short_click,
	button_state_multi_click,
	button_state_long_click,
	button_state_max
}button_state_e;

typedef struct button_flags{
	uint8_t active_low     :1;
	uint8_t reserved       :7;
}button_flags_t;

typedef struct button_status{
	uint8_t raw     :1;
	uint8_t trig    :1;
	uint8_t cont    :1;
	uint8_t reserved:5;
}button_status_t;

typedef struct button{
	char*           name;
	gpio_t*         gpio;
	button_flags_t  flags;
	button_status_t status;
	uint8_t         state : 4;
	uint8_t         event : 4;
	uint8_t         multi_clicks;
	uint32_t        short_click_ticks;
	uint32_t        long_click_ticks;
	uint32_t        click_release_ticks;
	uint32_t        click_press_ticks;
	uint32_t        ticks;
	uint32_t        clicks;
	button_callback cb[button_event_max];
	struct button*  next;
}button_t;

/*variables -----------------------------------------------------------------------------*/


/*prototypes ----------------------------------------------------------------------------*/
uint32_t button_init(button_t *newbutton);
uint32_t button_attach(button_t *button, button_event_e event, button_callback callback);
uint32_t button_dettach(button_t *button, button_event_e event);
uint32_t button_get_status(void);
uint32_t button_event_process(void);


#endif //_button_h_

