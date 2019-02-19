/*****************************************************************************************
* @file               : typedef.h
* @author             : ayangs
* @date               : 2017/03/25
* @brief              : This file contains all the common data types used for the
*                       STM32L152 clock configuration.
******************************************************************************************/
#ifndef _typedef_h_
#define _typedef_h_
/*includes ------------------------------------------------------------------------------*/
#include "stdint.h"
/*macros --------------------------------------------------------------------------------*/
#define null	0

#define enable	1
#define disable	0

#define set      1
#define reset    0

#define dim(x)			(sizeof(x)/sizeof(x[0]))

#define bit(x)			(1 << (x))
#define set_bit(y,x)	((y) | bit(x))
#define clr_bit(y,x)	((y) &~ bit(x))

/*typedefs ------------------------------------------------------------------------------*/


/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

#endif //_typedef_h_

