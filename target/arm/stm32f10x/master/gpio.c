/*****************************************************************************************
* @file               : gpio.c
* @author             : ayangs
* @date               : 2017/03/25
* @brief              : This file contains all the common data types used for the
*                       STM32L152 clock configuration.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "includes.h"
#include "errno.h"
#include "assert.h"
#include "gpio.h"
/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/
static uint32_t gpio_clk[]={
	RCC_APB2Periph_GPIOA,
	RCC_APB2Periph_GPIOB,
	RCC_APB2Periph_GPIOC,
	RCC_APB2Periph_GPIOD,
	RCC_APB2Periph_GPIOE,
	RCC_APB2Periph_GPIOF,
	RCC_APB2Periph_GPIOG};

static void* gpio_port[]={
	GPIOA,
	GPIOB,
	GPIOC,
	GPIOD,
	GPIOE,
	GPIOF,
	GPIOG};

static uint16_t gpio_pin[]={
	GPIO_Pin_0,
	GPIO_Pin_1,
	GPIO_Pin_2,
	GPIO_Pin_3,
	GPIO_Pin_4,
	GPIO_Pin_5,
	GPIO_Pin_6,
	GPIO_Pin_7,
	GPIO_Pin_8,
	GPIO_Pin_9,
	GPIO_Pin_10,
	GPIO_Pin_11,
	GPIO_Pin_12,
	GPIO_Pin_13,
	GPIO_Pin_14,
	GPIO_Pin_15};

static uint8_t gpio_dir[]={
	GPIO_Mode_AIN,        //Analog input, bypass to adc
	GPIO_Mode_IN_FLOATING,//Floating input, digt interface
	GPIO_Mode_IPD,        //Pull-down input, default low
	GPIO_Mode_IPU,        //Pull-up input, default high
	GPIO_Mode_Out_OD,     //Open-drain output, 0/3.3v
	GPIO_Mode_Out_PP,     //Push–pull output, 0/5v
	GPIO_Mode_AF_OD,      //Multiplex open drain output, wired and
	GPIO_Mode_AF_PP};     //Multiplexed push-pull output, digt interface

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/
uint32_t stm32f10x_gpio_init(uint8_t port, uint8_t pin, uint8_t dir)
{
	if(port >= dim(gpio_port) || pin >= dim(gpio_pin) || dir >= dim(gpio_dir))
	{
		return gpio_err_parameter;
	}

	RCC_APB2PeriphClockCmd(gpio_clk[port], enable);

	GPIO_InitTypeDef GPIO_InitStructure={};
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode  = gpio_dir[dir];
	GPIO_InitStructure.GPIO_Pin   = gpio_pin[pin];
	GPIO_Init(gpio_port[port], &GPIO_InitStructure);

	return success;
}

uint32_t stm32f10x_gpio_get(uint8_t port, uint8_t pin, uint8_t dir, uint8_t *value)
{
	if(port >= dim(gpio_port)|| pin >= dim(gpio_pin) || dir >= 4)
	{
		return gpio_err_parameter;
	}

	*value = GPIO_ReadInputDataBit(gpio_port[port],gpio_pin[pin]);

	return success;
}

uint32_t stm32f10x_gpio_set(uint8_t port, uint8_t pin, uint8_t dir, uint8_t *value)
{
	if(port >= dim(gpio_port)|| pin >= dim(gpio_pin) || dir >= dim(gpio_dir))
	{
		return gpio_err_parameter;
	}

	if(*value == 0)
		GPIO_ResetBits(gpio_port[port],gpio_pin[pin]);
	else
		GPIO_SetBits(gpio_port[port],gpio_pin[pin]);

	return success;
}

/*public --------------------------------------------------------------------------------*/
uint32_t gpio_init(gpio_t *gpio)
{

	stm32f10x_gpio_init(gpio->port, gpio->pin, gpio->dir);

	return success;
}


uint32_t gpio_set(gpio_t *gpio, uint8_t *value)
{

	gpio->value = *value;
	
	stm32f10x_gpio_set(gpio->port, gpio->pin, gpio->dir, &gpio->value);

	return success;
}


uint32_t gpio_get(gpio_t *gpio, uint8_t *value)
{
	
	stm32f10x_gpio_get(gpio->port, gpio->pin, gpio->dir, &gpio->value);

	*value = gpio->value;

	return success;
}




