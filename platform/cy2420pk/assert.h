/*****************************************************************************************
* @file               : assert.h
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Head files for assert.
******************************************************************************************/
#ifndef _assert_h_
#define _assert_h_
/*includes ------------------------------------------------------------------------------*/
#include "typedef.h"

/*macros --------------------------------------------------------------------------------*/
#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#define assert(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)filename(__FILE__), __LINE__))

#define assert_return_err(expr,err) \
do {\
	if(!(expr)) \
	{\
		return err;\
	}\
}while(0);

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/
void assert_failed(uint8_t* file, uint32_t line);


#endif //_assert_h_

