/*****************************************************************************************
* @file               : typedef.h
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Head files for typedef.
******************************************************************************************/
#ifndef _typedef_h_
#define _typedef_h_
/*includes ------------------------------------------------------------------------------*/
#include <stdint.h>
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


#define _inline_				static __inline


/* the version of GNU GCC must be greater than 4.x */
#define va_list					__builtin_va_list
#define va_start(v,l)			__builtin_va_start(v,l)
#define va_end(v)				__builtin_va_end(v)
#define va_arg(v,l) 			__builtin_va_arg(v,l)

/*typedefs ------------------------------------------------------------------------------*/
typedef long					base_t;
typedef unsigned long			ubase_t;

typedef ubase_t					size_t;


/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

#endif //_typedef_h_

