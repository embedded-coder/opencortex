/*****************************************************************************************
* @file               : flash.h
* @author             : ayangs
* @date               : 2019/08/23
* @brief              : Head files for flash.
******************************************************************************************/

#ifndef _flash_h_
#define _flash_h_
/*includes ------------------------------------------------------------------------------*/
#include "typedef.h"
/*macros --------------------------------------------------------------------------------*/
#define flash_base        ((uint32_t)0x08000000u)
#define flash_page(page)  (flash_base + page * 0x800)

#define flash_page_bits    (11)
#define flash_page_size    ((uint32_t)(1 << flash_page_bits))//2KByte

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/
uint32_t flash_earse(uint32_t flash, uint32_t nbytes);

uint32_t flash_write(uint32_t flash, uint32_t* data, uint32_t nbytes);

uint32_t flash_read(uint32_t flash, uint32_t* data, uint32_t nbytes);

#endif //_flash_h_

