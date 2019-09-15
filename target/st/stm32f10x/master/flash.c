/*****************************************************************************************
* @file               : flash.c
* @author             : ayangs
* @date               : 2019/08/23
* @brief              : Source files for flash.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "includes.h"
#include "errno.h"
#include "assert.h"
#include "flash.h"
/*macros --------------------------------------------------------------------------------*/
#define FLASH_PAGE_BITS    (11)
#define FLASH_PAGE_SIZE    ((uint32_t)(1 << FLASH_PAGE_BITS))//2KByte

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/
#include "log.h"
/*private -------------------------------------------------------------------------------*/
void stm32f10x_flash_earse(uint32_t flash, uint32_t nwords)
{
	FLASH_Status status = FLASH_COMPLETE;
	
	uint32_t bytes = nwords << 2;
	
	uint32_t pages = (bytes & (FLASH_PAGE_SIZE-1)) ? ((bytes >> FLASH_PAGE_BITS) + 1) : (bytes >> FLASH_PAGE_BITS);

	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR | FLASH_FLAG_OPTERR);

	for(uint32_t i = 0; (i < pages) && (status == FLASH_COMPLETE); i++)
	{
		status = FLASH_ErasePage(flash + (FLASH_PAGE_SIZE * i));
	}

	FLASH_Lock();
}

void stm32f10x_flash_write(uint32_t flash, uint32_t* data, uint32_t nwords)
{
	FLASH_Status status = FLASH_COMPLETE;
	
	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR | FLASH_FLAG_OPTERR);

	for(uint32_t iwords = 0; (iwords < nwords) && (status == FLASH_COMPLETE); iwords++)
	{
		status = FLASH_ProgramWord(flash + iwords * 4, data[iwords]);
	}

	FLASH_Lock();
}

/*public --------------------------------------------------------------------------------*/

void flash_earse(uint32_t address, uint32_t nwords)
{
	stm32f10x_flash_earse(address, nwords);
}

void flash_write(uint32_t flash, uint32_t* data, uint32_t nwords)
{
	stm32f10x_flash_write(flash, data, nwords);
}

void flash_read(uint32_t flash, uint32_t* data, uint32_t nwords)
{
	for(uint32_t iwords = 0; iwords < nwords; iwords++)
	{
		*(data + iwords) = *(uint32_t*)(flash + iwords * 4);
	}
}


