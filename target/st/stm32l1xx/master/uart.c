/*****************************************************************************************
* @file               : uart.c
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Source files for uart.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "includes.h"
#include "errno.h"
#include "assert.h"
#include "uart.h"
/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

static uint32_t uart_clks[]={
	RCC_APB2Periph_USART1,
	RCC_APB1Periph_USART2,
	RCC_APB1Periph_USART3,
	RCC_APB1Periph_UART4,
	RCC_APB1Periph_UART5
};

static void* uart_ports[]={
	USART1,
	USART2,
	USART3,
	UART4,
	UART5
};

static uint16_t uart_attrs[]={
	/*data width*/
	USART_WordLength_8b,
	USART_WordLength_9b,
	/*stop bits*/
	USART_StopBits_0_5,
	USART_StopBits_1,
	USART_StopBits_1_5,
	USART_StopBits_2,
	/*parity*/
	USART_Parity_No,
	USART_Parity_Odd,
	USART_Parity_Even,
	/*flow control*/
	USART_HardwareFlowControl_None,
	USART_HardwareFlowControl_RTS,
	USART_HardwareFlowControl_CTS,
	USART_HardwareFlowControl_RTS_CTS,
	/*irq resource*/
	0,
	USART_IT_PE,
	USART_IT_TXE,
	USART_IT_TC,
	USART_IT_RXNE,
	USART_IT_ORE_RX,
	USART_IT_IDLE,
	USART_IT_LBD,
	USART_IT_CTS,
	USART_IT_ERR,
	USART_IT_ORE_ER,
	USART_IT_NE,
	USART_IT_FE
};


/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/
uint32_t stm32l1xx_uart_init(uint8_t port, uint32_t baudrate, uint16_t parity, 
					uint16_t datawidth, uint16_t stopbit, uint16_t flowctrl, uint16_t irq)
{
	assert_return_err(port < dim(uart_ports), uart_err_parameter);
	
	if(port == 0)
		RCC_APB2PeriphClockCmd(uart_clks[port], ENABLE);
	else
		RCC_APB1PeriphClockCmd(uart_clks[port], ENABLE);

	USART_InitTypeDef uart_cfg={};
	
	uart_cfg.USART_BaudRate            = baudrate;
	uart_cfg.USART_WordLength          = uart_attrs[datawidth];
	uart_cfg.USART_StopBits            = uart_attrs[stopbit];
	uart_cfg.USART_Parity              = uart_attrs[parity];
	uart_cfg.USART_Mode 	           = USART_Mode_Rx | USART_Mode_Tx;
	uart_cfg.USART_HardwareFlowControl = uart_attrs[flowctrl];
	
	USART_Init(uart_ports[port], &uart_cfg);
	
	if(irq == irq_rxne)
	{
		USART_ITConfig(uart_ports[port], uart_attrs[irq], ENABLE);
	}

	USART_Cmd(uart_ports[port], ENABLE);

	USART_GetFlagStatus(uart_ports[port], USART_FLAG_TC);
	return success;
}

uint32_t stm32l1xx_uart_send(uint8_t port, char c)
{
	assert_return_err(port < dim(uart_ports), uart_err_parameter);
	
	USART_SendData(uart_ports[port], c);

	while (USART_GetFlagStatus(uart_ports[port], USART_FLAG_TXE) == RESET);

	return success;
}

/*public --------------------------------------------------------------------------------*/
uint32_t uart_init(uart_t *uart)
{
	assert_return_err(uart, uart_err_parameter);

	if(null != uart->txpin)
		gpio_init(uart->txpin);

	if(null != uart->rxpin)
		gpio_init(uart->rxpin);

	stm32l1xx_uart_init(uart->port, uart->baudrate, uart->parity,
			uart->datawidth, uart->stopbit, uart->flowctrl, uart->irq);
	return success;
}

uint32_t uart_puts(uart_t *uart, char *s)
{
	assert_return_err(uart, uart_err_parameter);

	while(*s != 0)
	{
		stm32l1xx_uart_send(uart->port, *s);
		s++;
	}
	return success;
}

