/*****************************************************************************************
* @file               : irq.h
* @author             : ayangs
* @date               : 2019/09/22
* @brief              : Head files for irq.
******************************************************************************************/
#ifndef _irq_h_
#define _irq_h_
/*includes ------------------------------------------------------------------------------*/
#include "typedef.h"

/*macros --------------------------------------------------------------------------------*/
#define irq_group_pro0_sub4   0
#define irq_group_pro1_sub3   1
#define irq_group_pro2_sub2   2
#define irq_group_pro3_sub1   3
#define irq_group_pro4_sub0   4

/*typedefs ------------------------------------------------------------------------------*/
typedef struct irq{
	char*    name;
	uint32_t group            :3;
	uint32_t reserved         :29;
}irq_t;


/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/
uint32_t irq_init(irq_t *irq);

#endif //_irq_h_

