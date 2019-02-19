/*****************************************************************************************
* @file               : main.c
* @author             : EmbeddedCoder
* @date               : 2017/03/25
* @brief              : This file contains main process.
******************************************************************************************/


#include "delay.h"
//#include "led.h"
//#include "key.h"
//#include "usart.h"
#include "platform.h"

extern void example_process(void);

int main(void)
{
	platform_init();
	
	while(1)
	{
		example_process();
	}
}
