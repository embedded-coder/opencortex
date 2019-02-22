/*****************************************************************************************
* @file               : gpio.h
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Head files for gpio.
******************************************************************************************/
#ifndef _gpio_h_
#define _gpio_h_
/*includes ------------------------------------------------------------------------------*/
#include "typedef.h"
/*macros --------------------------------------------------------------------------------*/
#define port(x)    ((x)-'A')

#define high	1
#define low	    0

/*typedefs ------------------------------------------------------------------------------*/
typedef struct gpio{
	char*   name;
	uint8_t value;
	uint8_t port;
	uint8_t pin;
	uint8_t dir;
	uint8_t mux;
}gpio_t;

typedef enum gpio_dir{
	in_out_analog,
	in_float,
	in_pulldown,
	in_pullup,
	out_opendrain,
	out_pushpull,
	out_opendrain_mux,
	out_pushpull_mux,
}gpio_dir_e;

typedef enum gpio_mux{
	mux_rtc_mco_wkup_swj_trace,
	mux_tim2,
	mux_tim3_5,
	mux_tim9_11,
	mux_i2c1_2,
	mux_spi1_2,
	mux_spi3,
	mux_uart1_3,
	mux_uart4_5,
	mux_usb,
	mux_lcd,
	mux_fsmc_sdio,
	mux_ri,
	mux_eventout,
	mux_none
}gpio_mux_e;

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/
uint32_t gpio_init(gpio_t *gpio);

uint32_t gpio_set(gpio_t *gpio, uint8_t *value);

uint32_t gpio_get(gpio_t *gpio, uint8_t *value);


#endif //_gpio_h_


