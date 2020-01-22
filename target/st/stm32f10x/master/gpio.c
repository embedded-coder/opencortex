/*****************************************************************************************
* @file               : gpio.c
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Source files for gpio.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "includes.h"
#include "errno.h"
#include "assert.h"
#include "gpio.h"
/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/
static uint32_t gpio_clks[]={
	RCC_APB2Periph_GPIOA,
	RCC_APB2Periph_GPIOB,
	RCC_APB2Periph_GPIOC,
	RCC_APB2Periph_GPIOD,
	RCC_APB2Periph_GPIOE,
	RCC_APB2Periph_GPIOF,
	RCC_APB2Periph_GPIOG
};

static void* gpio_ports[]={
	GPIOA,
	GPIOB,
	GPIOC,
	GPIOD,
	GPIOE,
	GPIOF,
	GPIOG
};

static uint16_t gpio_pins[]={
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
	GPIO_Pin_15
};

static uint8_t gpio_dirs[]={
	GPIO_Mode_AIN,        //Analog input, bypass to adc
	GPIO_Mode_IN_FLOATING,//Floating input, digt interface
	GPIO_Mode_IPD,        //Pull-down input, default low
	GPIO_Mode_IPU,        //Pull-up input, default high
	GPIO_Mode_Out_OD,     //Open-drain output, 0/3.3v
	GPIO_Mode_Out_PP,     //Push¨Cpull output, 0/5v
	GPIO_Mode_AF_OD,      //Multiplex open drain output, wired and
	GPIO_Mode_AF_PP       //Multiplexed push-pull output, digt interface
};

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/
uint32_t stm32f10x_gpio_init(uint8_t port, uint8_t pin, uint8_t dir)
{
	assert_return_err(port < dim(gpio_ports), gpio_err_parameter);
	assert_return_err(pin < dim(gpio_pins), gpio_err_parameter);
	assert_return_err(dir < dim(gpio_dirs), gpio_err_parameter);
	
	RCC_APB2PeriphClockCmd(gpio_clks[port], enable);

	GPIO_InitTypeDef GPIO_InitStructure={};
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode  = gpio_dirs[dir];
	GPIO_InitStructure.GPIO_Pin   = gpio_pins[pin];
	GPIO_Init(gpio_ports[port], &GPIO_InitStructure);

	return success;
}

uint32_t stm32f10x_gpio_deinit(uint8_t port)
{
	assert_return_err(port < dim(gpio_ports), gpio_err_parameter);
	
	GPIO_DeInit(gpio_ports[port]);

	return success;
}


uint32_t stm32f10x_gpio_get(uint8_t port, uint8_t pin, uint8_t dir, uint8_t *value)
{
	assert_return_err(port < dim(gpio_ports), gpio_err_parameter);
	assert_return_err(pin < dim(gpio_pins), gpio_err_parameter);
	assert_return_err(dir < 4, gpio_err_parameter);

	*value = GPIO_ReadInputDataBit(gpio_ports[port],gpio_pins[pin]);

	return success;
}

uint32_t stm32f10x_gpio_set(uint8_t port, uint8_t pin, uint8_t dir, uint8_t *value)
{
	assert_return_err(port < dim(gpio_ports), gpio_err_parameter);
	assert_return_err(pin < dim(gpio_pins), gpio_err_parameter);
	assert_return_err(dir < dim(gpio_dirs), gpio_err_parameter);

	if(*value == 0)
		GPIO_ResetBits(gpio_ports[port],gpio_pins[pin]);
	else
		GPIO_SetBits(gpio_ports[port],gpio_pins[pin]);

	return success;
}

/*public --------------------------------------------------------------------------------*/
uint32_t gpio_init(gpio_t *gpio)
{
	assert_return_err(gpio, gpio_err_parameter);
	
	stm32f10x_gpio_init(gpio->port, gpio->pin, gpio->dir);

	return success;
}

uint32_t gpio_deinit(gpio_t *gpio)
{
	assert_return_err(gpio, gpio_err_parameter);
	
	stm32f10x_gpio_deinit(gpio->port);

	return success;
}

uint32_t gpio_set(gpio_t *gpio, uint8_t *value)
{

	assert_return_err(gpio, gpio_err_parameter);

	gpio->value = *value;
	
	stm32f10x_gpio_set(gpio->port, gpio->pin, gpio->dir, &gpio->value);

	return success;
}


uint32_t gpio_get(gpio_t *gpio, uint8_t *value)
{
	
	assert_return_err(gpio, gpio_err_parameter);
	
	stm32f10x_gpio_get(gpio->port, gpio->pin, gpio->dir, &gpio->value);

	*value = gpio->value;

	return success;
}




