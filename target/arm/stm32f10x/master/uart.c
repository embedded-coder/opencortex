/*****************************************************************************************
* @file               : uart.c
* @author             : ayangs
* @date               : 2017/03/25
* @brief              : This file contains all the common data types used for the
*                       STM32L152 clock configuration.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "includes.h"
#include "errno.h"
#include "assert.h"
#include "uart.h"
/*macros --------------------------------------------------------------------------------*/
static uint32_t uart_clk[]={
	RCC_APB2Periph_USART1,
	RCC_APB1Periph_USART2,
	RCC_APB1Periph_USART3,
	RCC_APB1Periph_UART4,
	RCC_APB1Periph_UART5};

static void* uart_port[]={
	USART1,
	USART2,
	USART3,
	UART4,
	UART5};

static uint16_t uart_attr[]={
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
	USART_HardwareFlowControl_RTS_CTS};

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/
uint32_t stm32f10x_uart_init(uint8_t port, uint32_t baudrate, uint16_t parity,
						uint16_t datawidth, uint16_t stopbit, uint16_t flowctrl)
{
	assert_return_err(port < dim(uart_port), uart_err_parameter);
	
	if(port == 0)
		RCC_APB2PeriphClockCmd(uart_clk[port], ENABLE);
	else
		RCC_APB1PeriphClockCmd(uart_clk[port], ENABLE);
	
	USART_InitTypeDef USART_InitStructure={};
	
	USART_InitStructure.USART_BaudRate 	 = baudrate;
	USART_InitStructure.USART_WordLength = uart_attr[datawidth];
	USART_InitStructure.USART_StopBits   = uart_attr[stopbit];
	USART_InitStructure.USART_Parity     = uart_attr[parity];
	USART_InitStructure.USART_Mode 	     = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = uart_attr[flowctrl];
	
	USART_Init(uart_port[port], &USART_InitStructure);

	USART_Cmd(uart_port[port], ENABLE);
	
	return success;
}

uint32_t stm32f10x_uart_send(uint8_t port, char c)
{
	assert_return_err(port < dim(uart_port), uart_err_parameter);
	
	USART_SendData(uart_port[port], c);

	while (USART_GetFlagStatus(uart_port[port], USART_FLAG_TXE) == RESET);

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

	stm32f10x_uart_init(uart->port, uart->baudrate, uart->parity,
						uart->datawidth, uart->stopbit, uart->flowctrl);
	return success;
}

uint32_t uart_send(uart_t *uart, char c)
{
	assert_return_err(uart, uart_err_parameter);

	stm32f10x_uart_send(uart->port, c);

	uart->value = c;

	return success;
}

uint32_t uart_send_str(uart_t *uart, char *s)
{
	assert_return_err(uart, uart_err_parameter);

	while(*s != 0)
	{
		uart_send(uart, *s);
		s++;
	}
	return success;
}

uint32_t uart_send_dec(uart_t *uart, uint32_t l, uint8_t len)
{
	assert_return_err(uart, uart_err_parameter);

	uint8_t  i = 0;
	uint32_t p = 1;

	for(i = 1; i < len; i++)
		p *= 10;

	while(p > 0)
	{
		i = (uint8_t)(l / p);
		uart_send(uart, '0' + i);
		l = l - (i * p);

		p = p / 10;
	}

	return success;
}






