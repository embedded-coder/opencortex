/*****************************************************************************************
* @file               : irq.c
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Source files for irq.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "includes.h"
#include "errno.h"
#include "assert.h"
#include "irq.h"

/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/
static uint32_t irq_group[]={
	NVIC_PriorityGroup_0,
	NVIC_PriorityGroup_1,
	NVIC_PriorityGroup_2,
	NVIC_PriorityGroup_3,
	NVIC_PriorityGroup_4
};

static irq_t* girq = null;

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/
uint32_t stm32f10x_irq_init(irq_t *irq)
{
	assert_return_err(irq, irq_err_parameter);
	
	if(null == girq)
	{
		girq = irq;
		NVIC_PriorityGroupConfig(irq_group[irq->group]); 
	}
	return success;
}

uint32_t stm32f10x_irq_set(void)
{
	return success;
}
/*public --------------------------------------------------------------------------------*/


uint32_t irq_init(irq_t *irq)
{
	stm32f10x_irq_init(irq);
	
	return success;
}



