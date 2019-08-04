/*****************************************************************************************
* @file               : hello-world.c
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Source files for hello-world.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "platform.h"

/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/

/*public --------------------------------------------------------------------------------*/

void example_process(void)
{
	log_str(debug, tags, "hello world! " );
	log_dec(fatal, null, 1024, 4);
	delay_ms(500);
}

