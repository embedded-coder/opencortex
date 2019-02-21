/*****************************************************************************************
* @file               : platform.h
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Head files for platform.
******************************************************************************************/
#ifndef _platform_h_
#define _platform_h_
/*includes ------------------------------------------------------------------------------*/
#include "includes.h"
/* master drivers */
#include "gpio.h"
#include "uart.h"

/* device drivers */
#include "led.h"
#include "button.h"

/* others */
#include "delay.h"
#include "log.h"
#include "errno.h"
/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/
extern led_t leds[];
extern button_t buttons[];
extern log_t logs[];
/*prototypes ----------------------------------------------------------------------------*/
void platform_init(void);

#endif //_platform_h_

