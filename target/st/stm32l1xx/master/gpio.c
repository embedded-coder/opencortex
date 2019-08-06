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
#define gpio_dire(in_out,up_down,od_pp) (((in_out) << 16) | ((up_down) << 8) | (od_pp))
#define gpio_mode(mode) (((mode) >> 16) & 0xff)
#define gpio_pupd(mode) (((mode) >> 8) & 0xff)
#define gpio_otyp(mode) ((mode) & 0xff)
/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/
static uint32_t gpio_clks[]={
	RCC_AHBPeriph_GPIOA,
	RCC_AHBPeriph_GPIOB,
	RCC_AHBPeriph_GPIOC,
	RCC_AHBPeriph_GPIOD,
	RCC_AHBPeriph_GPIOE,
	RCC_AHBPeriph_GPIOF,
	RCC_AHBPeriph_GPIOG,
	RCC_AHBPeriph_GPIOH
};

static void* gpio_ports[]={
	GPIOA,
	GPIOB,
	GPIOC,
	GPIOD,
	GPIOE,
	GPIOF,
	GPIOG,
	GPIOH
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


static uint32_t gpio_dirs[]={
	gpio_dire(GPIO_Mode_AN ,null            ,null),           //GPIO Analog In/Out Mode 
	gpio_dire(GPIO_Mode_IN ,GPIO_PuPd_NOPULL,null),           //GPIO Input Mode,Floating
	gpio_dire(GPIO_Mode_IN ,GPIO_PuPd_DOWN  ,null),           //GPIO Input Mode,Pull-down
	gpio_dire(GPIO_Mode_IN ,GPIO_PuPd_UP    ,null),           //GPIO Input Mode,Pull-up
	gpio_dire(GPIO_Mode_OUT,GPIO_PuPd_NOPULL,GPIO_OType_OD), //GPIO Output Mode,Open-drain
	gpio_dire(GPIO_Mode_OUT,GPIO_PuPd_NOPULL,GPIO_OType_PP), //GPIO Output Mode,Push–pull
	gpio_dire(GPIO_Mode_AF ,GPIO_PuPd_UP    ,GPIO_OType_OD), //GPIO Alternate function Mode,Open-drain
	gpio_dire(GPIO_Mode_AF ,GPIO_PuPd_UP    ,GPIO_OType_PP)  //GPIO Alternate function Mode,Push–pull
};


/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/
uint32_t stm32l1xx_gpio_init(uint8_t port, uint8_t pin, uint8_t dir,uint16_t mux)
{
	assert_return_err(port < dim(gpio_ports), gpio_err_parameter);
	assert_return_err(pin < dim(gpio_pins), gpio_err_parameter);
	assert_return_err(dir < dim(gpio_dirs), gpio_err_parameter);

	RCC_AHBPeriphClockCmd(gpio_clks[port], enable);

	GPIO_InitTypeDef GPIO_InitStructure={};
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin   = gpio_pins[pin];
	GPIO_InitStructure.GPIO_Mode  = gpio_mode(gpio_dirs[dir]);
	GPIO_InitStructure.GPIO_OType = gpio_otyp(gpio_dirs[dir]);
	GPIO_InitStructure.GPIO_PuPd  = gpio_pupd(gpio_dirs[dir]);
	GPIO_Init(gpio_ports[port], &GPIO_InitStructure);

	if(mux != mux_none)
	{
		GPIO_PinAFConfig(gpio_ports[port], pin, mux);
	}
	return success;
}

uint32_t stm32l1xx_gpio_get(uint8_t port, uint8_t pin, uint8_t dir, uint8_t *value)
{
	assert_return_err(port < dim(gpio_ports), gpio_err_parameter);
	assert_return_err(pin < dim(gpio_pins), gpio_err_parameter);
	assert_return_err(dir < 4, gpio_err_parameter);

	*value = GPIO_ReadInputDataBit(gpio_ports[port],gpio_pins[pin]);

	return success;
}

uint32_t stm32l1xx_gpio_set(uint8_t port, uint8_t pin, uint8_t dir, uint8_t *value)
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

	stm32l1xx_gpio_init(gpio->port, gpio->pin, gpio->dir, gpio->mux);

	return success;
}


uint32_t gpio_set(gpio_t *gpio, uint8_t *value)
{

	gpio->value = *value;
	
	stm32l1xx_gpio_set(gpio->port, gpio->pin, gpio->dir, &gpio->value);

	return success;
}


uint32_t gpio_get(gpio_t *gpio, uint8_t *value)
{
	
	stm32l1xx_gpio_get(gpio->port, gpio->pin, gpio->dir, &gpio->value);

	*value = gpio->value;

	return success;
}




