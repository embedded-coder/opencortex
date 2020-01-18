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
#include "delay.h"
/*macros --------------------------------------------------------------------------------*/
#define invalid_value    0xffff

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

static uint32_t uart_data_bits[]={
	invalid_value,
	invalid_value,
	invalid_value,
	USART_WordLength_8b,
	USART_WordLength_9b
};

static uint32_t uart_parity[]={
	USART_Parity_No,
	USART_Parity_Odd,
	USART_Parity_Even
};

static uint32_t uart_stop_bits[]={
	USART_StopBits_0_5,
	USART_StopBits_1,
	USART_StopBits_1_5,
	USART_StopBits_2,
	invalid_value,
	invalid_value
};

static uint32_t uart_flowctrl[]={
	USART_HardwareFlowControl_None,
	USART_HardwareFlowControl_RTS,
	USART_HardwareFlowControl_CTS,
	USART_HardwareFlowControl_RTS_CTS
};

static uint32_t uart_mode[]={
	USART_Mode_Rx,
	USART_Mode_Tx,
	USART_Mode_Rx | USART_Mode_Tx
};

static int32_t uart_irqs[]={
	USART1_IRQn,
	USART2_IRQn,
	USART3_IRQn,
	UART4_IRQn,
	UART5_IRQn
};

static uint16_t uart_irq_resources[]={
	invalid_value,
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
uint32_t stm32f10x_uart_set_rcc(uint8_t port, uint8_t state)
{
	if(port == 0)
		RCC_APB2PeriphClockCmd(uart_clks[port], state);
	else
		RCC_APB1PeriphClockCmd(uart_clks[port], state);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	return success;
}

uint32_t stm32f10x_uart_set_attr(uint8_t port, uint8_t mode, uint32_t baudrate, uint8_t databits, 
                                       uint8_t stopbits, uint8_t parity, uint8_t flowctrl)
{
	USART_InitTypeDef uart_cfg = {};
	
	uart_cfg.USART_BaudRate            = baudrate;
	uart_cfg.USART_WordLength          = uart_data_bits[databits];
	uart_cfg.USART_StopBits            = uart_stop_bits[stopbits];
	uart_cfg.USART_Parity              = uart_parity[parity];
	uart_cfg.USART_HardwareFlowControl = uart_flowctrl[flowctrl];
	uart_cfg.USART_Mode                = uart_mode[mode];
	
	USART_Init(uart_ports[port], &uart_cfg);
	
	return success;
}

uint32_t stm32f10x_uart_set_irq(uint8_t port, uint8_t resource, uint8_t priority, 
                                                    uint8_t subpriority, uint8_t state)
{
	NVIC_InitTypeDef irq_cfg = {};
	
	irq_cfg.NVIC_IRQChannel                   = uart_irqs[port];
	irq_cfg.NVIC_IRQChannelPreemptionPriority = priority;
	irq_cfg.NVIC_IRQChannelSubPriority        = subpriority;
	irq_cfg.NVIC_IRQChannelCmd                = state;

	NVIC_Init(&irq_cfg);

	USART_ITConfig(uart_ports[port], uart_irq_resources[resource], state);

	return success;
}

uint32_t stm32f10x_uart_set_state(uint8_t port, uint8_t state)
{
	USART_Cmd(uart_ports[port], state);
	
	return success;
}

uint32_t stm32f10x_uart_send(uint8_t port, char c)
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

	uart_cfg_t* config = &(uart->config);
	uart_irq_t* irq    = &(uart->irq);

	stm32f10x_uart_set_rcc(uart->port, ENABLE);

	if(null != uart->txio)
		gpio_init(uart->txio);

	if(null != uart->rxio)
		gpio_init(uart->rxio);
	
	stm32f10x_uart_set_attr(uart->port, uart->mode, config->baud_rate, config->data_bits, 
                                     config->stop_bits, config->parity, config->flowctrl);

	if(uart_irq_none == irq->resource)
		stm32f10x_uart_set_irq(uart->port, irq->resource, irq->priority, irq->subpriority, DISABLE);
	else
		stm32f10x_uart_set_irq(uart->port, irq->resource, irq->priority, irq->subpriority, ENABLE);

	stm32f10x_uart_set_state(uart->port, ENABLE);

	uart->rxfifo->head = 0;
	uart->rxfifo->tail = 0;
	
	return success;
}

uint32_t uart_putc(uart_t *uart, char c)
{
	assert_return_err(uart, uart_err_parameter);
	
	stm32f10x_uart_send(uart->port, c);
	
	return success;
}

uint32_t uart_puts(uart_t *uart, char *s)
{
	assert_return_err(uart, uart_err_parameter);

	while(*s != 0)
	{
		stm32f10x_uart_send(uart->port, *s);
		s++;
	}
	return success;
}
uint32_t uart_getc(uart_t *uart, char *c, uint32_t n, int32_t timeout)
{
	assert_return_err(uart, uart_err_parameter);
	
	uint32_t tick = 0;
	
	uart_fifo_t* rxfifo = uart->rxfifo;

	if(uart->config.data_bits == data_bits_8)
	{
		if(uart->config.parity == parity_none)
		{
			for(uint32_t i = 0; i < n; i++)
			{
				tick = (timeout <= 0) ? 0 : (uint32_t)timeout;
				while(rxfifo->tail == rxfifo->head)//fifo empty
				{
					if(timeout <= 0)
					{
						continue;
					}
					else
					{
						if(--tick == 0)
						{
							return uart_err_rxtimeout;
						}
						delay_ms(1);
					}
				}
				
				*(c + i) = *(uint8_t*)(rxfifo->buffer + rxfifo->tail);
				
				rxfifo->tail =  (rxfifo->tail + 1 >= rxfifo->size) ? 0 : (rxfifo->tail + 1);
				
				rxfifo->count -= 1;
			}
			
			return success;
		}
	}
	
	return uart_err_parameter;
}

uint32_t uart_isr(uart_t *uart)
{
	assert_return_err(uart, uart_err_parameter);
	assert_return_err(uart_irq_rxne == uart->irq.resource, uart_err_parameter);
	
	if(USART_GetITStatus(uart_ports[uart->port], uart_irq_resources[uart->irq.resource]) == SET)
	{
		USART_ClearITPendingBit(uart_ports[uart->port],uart_irq_resources[uart->irq.resource]);	

		uint16_t data = USART_ReceiveData(uart_ports[uart->port]);
		

		uart_fifo_t *rxfifo  = uart->rxfifo;
		
		uint16_t     headnxt = (rxfifo->head + 1 >= rxfifo->size) ? 0 : (rxfifo->head + 1);
		
		if(uart->config.data_bits == data_bits_8)
		{
			if(uart->config.parity == parity_none)
			{
				if(headnxt != rxfifo->tail)//not empty
				{
					*(uint8_t*)(rxfifo->buffer + rxfifo->head) = (uint8_t)(data & 0x00FF);
					
					rxfifo->head  = headnxt;
					rxfifo->count += 1;
				}
			}
		}
	}

	if(USART_GetFlagStatus(uart_ports[uart->port],USART_FLAG_ORE) == SET)
	{
		USART_ClearFlag(uart_ports[uart->port],USART_FLAG_ORE);
		
		USART_ReceiveData(uart_ports[uart->port]);
	}
	
	return success;
}

