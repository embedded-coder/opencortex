/*****************************************************************************************
* @file               : platform.c
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Source files for platform.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "platform.h"

/*macros --------------------------------------------------------------------------------*/
/* default serial config */
#define uart_config_default                \
{                                          \
    baud_rate_115200, /* 115200 bits/s */  \
    data_bits_8,      /* 8 databits */     \
    stop_bits_1,      /* 1 stopbit */      \
    parity_none,      /* No parity  */     \
    bit_order_lsb,    /* LSB first sent */ \
    nrz_normal,       /* Normal mode */    \
    flowctrl_none,    /* Flow control */   \
    uart_rxbfr_size,  /* Buffer size */    \
    0                                      \
}

#define uart_irq_default                  \
{                                         \
	uart_irq_rxne,                        \
	0,                                    \
	0,                                    \
	0                                     \
}

#define uart_port_max                    5
#define uart_rxfifo_size                 128
/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/* MASTERs */
irq_t  irqs = {
		.name  = "irq",
		.group = irq_group_pro2_sub2,
};

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
	},
	{//uart2 tx
		.name  = "GpioA2",
		.value = low,
		.port  = port('A'),
		.pin   = 2,
		.dir   = out_pushpull_mux
	},
	{//uart2 rx
		.name  = "GpioA3",
		.value = low,
		.port  = port('A'),
		.pin   = 3,
		.dir   = in_float
	}
};

uint8_t uart_buffer[uart_port_max][uart_rxfifo_size];

uart_fifo_t uart_fifo[] = {
	{
		.buffer = &uart_buffer[0][0],
		.size	= uart_rxfifo_size,
		.count	= 0,
		.head	= 0,
		.tail	= 0
	},
	{
		.buffer = &uart_buffer[1][0],
		.size   = uart_rxfifo_size,
		.count  = 0,
		.head   = 0,
		.tail   = 0
	}
};

uart_t uarts[] = {
	{
		.name      = "uart1",
		.txio      = &gpios[2],
		.rxio      = &gpios[3],
        .config    = uart_config_default,
		.irq       = uart_irq_default,
		.rxfifo    = &uart_fifo[0],
		.port      = 0,
		.mode      = mode_tx_rx,
	},
	{
		.name      = "uart2",
		.txio      = &gpios[4],
		.rxio      = &gpios[5],
        .config    = uart_config_default,
		.irq       = uart_irq_default,
		.rxfifo    = &uart_fifo[1],
		.port      = 1,
		.mode      = mode_tx_rx,
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

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/

/*public --------------------------------------------------------------------------------*/

void platform_init(void)
{
	SystemInit();
	
	irq_init(&irqs);
	
	delay_init(1000);

	for(uint8_t i = 0; i < dim(uarts); i++)
	{
		uart_init(&uarts[i]);
	}
	for(uint8_t i = 0; i < dim(leds); i++)
	{
		led_init(&leds[i]);
	}
	for(uint8_t i = 0; i < dim(buttons); i++)
	{
		button_init(&buttons[i]);
	}
	
	log_init(&uarts[0]);
	ymodem_init(&uarts[1]);

}

void platform_deinit(void)
{
	irq_deinit();

	for(uint8_t i = 0; i < dim(uarts); i++)
	{
		uart_deinit(&uarts[i]);
	}
	for(uint8_t i = 0; i < dim(leds); i++)
	{
		led_deinit(&leds[i]);
	}
	for(uint8_t i = 0; i < dim(buttons); i++)
	{
		button_deinit(&buttons[i]);
	}
}

void USART1_IRQHandler(void)
{
	uart_isr(&uarts[0]);
}

void USART2_IRQHandler(void)
{
	uart_isr(&uarts[1]);
}

