/*****************************************************************************************
* @file               : uart.h
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Head files for uart.
******************************************************************************************/

#ifndef _uart_h_
#define _uart_h_
/*includes ------------------------------------------------------------------------------*/
#include "gpio.h"
#include "typedef.h"
/*macros --------------------------------------------------------------------------------*/
/* baud rate */
#define baud_rate_2400                  2400
#define baud_rate_4800                  4800
#define baud_rate_9600                  9600
#define baud_rate_19200                 19200
#define baud_rate_38400                 38400
#define baud_rate_57600                 57600
#define baud_rate_115200                115200
#define baud_rate_230400                230400
#define baud_rate_460800                460800
#define baud_rate_921600                921600
#define baud_rate_2000000               2000000
#define baud_rate_3000000               3000000

/* data bits */
#define data_bits_5                     0
#define data_bits_6                     1
#define data_bits_7                     2
#define data_bits_8                     3
#define data_bits_9                     4

/* stop bits */
#define stop_bits_0_5                   0
#define stop_bits_1                     1
#define stop_bits_1_5                   2
#define stop_bits_2                     3
#define stop_bits_3                     4
#define stop_bits_4                     5

/* parity */
#define parity_none                     0
#define parity_odd                      1
#define parity_even                     2

/* bits order */
#define bit_order_lsb                   0
#define bit_order_msb                   1

/* nrz mode */
#define nrz_normal                      0       /* Non Return to Zero : normal mode */
#define nrz_inverted                    1       /* Non Return to Zero : inverted mode */

/* hardware flow control */
#define flowctrl_none                   0
#define flowctrl_rts                    1
#define flowctrl_cts                    2
#define flowctrl_rts_cts                3

/* rx-tx mode */
#define mode_rx_only                    0
#define mode_tx_only                    1
#define mode_tx_rx                      2

#ifndef uart_rxbfr_size
#define uart_rxbfr_size                 64
#endif

#define uart_irq_none                   0
#define uart_irq_pe                     1
#define uart_irq_txe                    2
#define uart_irq_tc                     3
#define uart_irq_rxne                   4
#define uart_irq_ore_rx                 5
#define uart_irq_idle                   6
#define uart_irq_lbd                    7
#define uart_irq_cts                    8
#define uart_irq_err                    9
#define uart_irq_ore_err               10
#define uart_irq_ne                    11
#define uart_irq_fe                    12

/*typedefs ------------------------------------------------------------------------------*/
typedef struct{
    uint32_t baud_rate;
    uint32_t data_bits                 :3;
    uint32_t stop_bits                 :3;
    uint32_t parity                    :2;
    uint32_t bit_order                 :1;
    uint32_t invert                    :1;
	uint32_t flowctrl                  :2;
    uint32_t bufsz                     :16;
    uint32_t reserved                  :4;
}uart_cfg_t;

typedef struct{
	uint32_t resource                  :4;
	uint32_t priority                  :4;
	uint32_t subpriority               :4;
	uint32_t reserved                  :20;
}uart_irq_t;

typedef struct{
	uint8_t*               buffer;
	uint16_t               size;
	volatile uint16_t      count;
	volatile uint16_t      head;
	volatile uint16_t      tail;
}uart_fifo_t;

typedef struct{
	char*                  name;
	gpio_t*                txio;
	gpio_t*                rxio;
	uart_cfg_t             config;
	uart_irq_t             irq;
	uart_fifo_t*           rxfifo;
	uint8_t                port;
	uint8_t                mode;
	uint16_t               reserved;
}uart_t;

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/
uint32_t uart_init(uart_t *uart);

uint32_t uart_deinit(uart_t *uart);

uint32_t uart_putc(uart_t *uart, char c);

uint32_t uart_puts(uart_t *uart, char *s);

uint32_t uart_getc(uart_t *uart, char *c, uint32_t n, int32_t timeout);

uint32_t uart_isr(uart_t *uart);

#endif //_uart_h_

