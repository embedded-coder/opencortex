/*****************************************************************************************
* @file               : assert.c
* @author             : ayangs
* @date               : 2017/03/25
* @brief              : This file contains all the common data types used for the
*                       STM32L152 clock configuration.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "assert.h"
#include "switchs.h"
/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/

/*public --------------------------------------------------------------------------------*/
#if assert_switch_en
void assert_failed(uint8_t* file, uint32_t line)
{
		/*print <flie>  <line>*/

}
#endif
