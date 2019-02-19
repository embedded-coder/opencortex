/*************************************************************************************
* Test-program for Olimex “STM32-H103”, header board for “STM32F103RBT6”.
* After program start green LED (LED_E) will blink.
*
* Program has to be compiled with optimizer setting "-O0".
* Otherwise delay via while-loop will not work correctly.
*************************************************************************************/

#include "delay.h"
#include "platform.h"
void example_process(void)
{
	log_str(debug, tags, "hello world! " );
	log_dec(fatal, null, 1024, 4);
	
	delay_ms(500);
}

