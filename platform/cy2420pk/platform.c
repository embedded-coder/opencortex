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
	{//led0
		.name  = "GpioC11",
		.value = low,
		.port  = port('C'),
		.pin   = 11,
		.dir   = out_pushpull,
		.mux   = mux_none,
	},
	
	{//led1
		.name  = "GpioC12",
		.value = low,
		.port  = port('C'),
		.pin   = 12,
		.dir   = out_pushpull,
		.mux   = mux_none,
	},
	
	{//uart1 tx
		.name  = "GpioA2",
		.value = low,
		.port  = port('A'),
		.pin   = 2,
		.dir   = out_opendrain_mux,
		.mux   = mux_uart1_3,
	},
	
	{//uart1 rx
		.name  = "GpioA3",
		.value = low,
		.port  = port('A'),
		.pin   = 3,
		.dir   = out_opendrain_mux,
		.mux   = mux_uart1_3,
	},

	{//spi nss
		.name  = "GpioB12",
		.value = low,
		.port  = port('B'),
		.pin   = 12,
		.dir   = out_opendrain_mux,
		.mux   = mux_spi1_2,
	},

	{//spi sck
		.name  = "GpioB13",
		.value = low,
		.port  = port('B'),
		.pin   = 13,
		.dir   = out_opendrain_mux,
		.mux   = mux_spi1_2,
	},

	{//spi miso
		.name  = "GpioB14",
		.value = low,
		.port  = port('B'),
		.pin   = 14,
		.dir   = out_opendrain_mux,
		.mux   = mux_spi1_2,
	},

	{//spi mosi
		.name  = "GpioB15",
		.value = low,
		.port  = port('B'),
		.pin   = 15,
		.dir   = out_opendrain_mux,
		.mux   = mux_spi1_2,
	},

/*
	{//buttons
		.name  = "GpioC1",
		.value = low,
		.port  = port('C'),
		.pin   = 1,
		.dir   = in_pullup,
		.mux   = mux_none
	},
*/

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
		.port      = 1,
		.value     = 0
	}
};

spi_t spis[] = {
	{
		.name     = "spi",
		.nsspin   = &gpios[4],
		.sckpin   = &gpios[5],
		.misopin  = &gpios[6],
		.mosipin  = &gpios[7],
		.datadir  = datadir_2line_trx,
		.mode     = mode_master,
		.datasize = datasize_8b,
		.clkpol   = clkpol_low,
		.clkpha   = clkpha_1edge,
		.chipsel  = chipsel_soft,
		.baudrate = baudrate_2,
		.firstbit = firstbit_msb,
		.port     = 1,
	}
};

/* DEVICEs */
led_t leds[] = {
	{
		.name       = "Led0",
		.gpio       = &gpios[0],
		.brightness = led_off,
		.flags      = led_flags_active_low,
	},
	
	{
		.name       = "Led1",
		.gpio       = &gpios[1],
		.brightness = led_off,
		.flags      = led_flags_active_low,
	}
};

button_t buttons[] = {
	{
		.name                = "usrBtn",
		.gpio                = &gpios[4],
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
		.level = log_level,
};

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/

/*public --------------------------------------------------------------------------------*/
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

void platform_init(void)
{
	SystemInit();

	delay_init(1000);

	log_init(&logs);
	
	for(uint8_t i = 0; i < dim(leds); i++)
	{
		led_init(&leds[i]);
	}
	
/*	for(uint8_t i = 0; i < dim(buttons); i++)
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
*/
	for(uint8_t i = 0; i < dim(spis); i++)
	{
		spi_init(&spis[i]);
	}

}

