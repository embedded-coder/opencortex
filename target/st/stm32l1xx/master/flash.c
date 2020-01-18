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

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/
uint32_t stm32l1xx_flash_earse(uint32_t flash, uint32_t nbytes)
{
	assert_return_err(flash > flash_base, flash_err_parameter);

	uint32_t status = success;
	
	uint32_t npages = (nbytes & (flash_page_size-1)) ? ((nbytes >> flash_page_bits) + 1) : (nbytes >> flash_page_bits);

	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR |
					FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_OPTVERRUSR);

	for(uint32_t ipage = 0; (ipage < npages) && (status == success); ipage++)
	{
		if(FLASH_COMPLETE != FLASH_ErasePage(flash + (flash_page_size * ipage)))
		{
			status = flash_err_erase;
		}
	}

	FLASH_Lock();
	
	return status;
}

uint32_t stm32l1xx_flash_write(uint32_t flash, uint32_t* data, uint32_t nbytes)
{
	assert_return_err(flash > flash_base, flash_err_parameter);
	assert_return_err(data, flash_err_parameter);

	uint32_t status = success;

	uint32_t nwords = (nbytes & 3) ? ((nbytes >> 2) + 1) : (nbytes >> 2);
	
	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR |
					FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_OPTVERRUSR);

	for(uint32_t iwords = 0; (iwords < nwords) && (status == success); iwords++)
	{
		if(FLASH_COMPLETE != FLASH_FastProgramWord(flash + iwords * 4, data[iwords]))
		{
			status = flash_err_write;
		}
		else if(data[iwords] != *(volatile uint32_t*)(flash + iwords * 4))
		{
			status = flash_err_readback;
		}
	}

	FLASH_Lock();
	
	return status;
}

/*public --------------------------------------------------------------------------------*/

uint32_t flash_erase(uint32_t flash, uint32_t nbytes)
{
	assert_return_err(flash > flash_base, flash_err_parameter);
	
	return stm32l1xx_flash_earse(flash, nbytes);
}

uint32_t flash_write(uint32_t flash, uint32_t* data, uint32_t nbytes)
{
	assert_return_err(flash > flash_base, flash_err_parameter);
	assert_return_err(data, flash_err_parameter);
	
	return stm32l1xx_flash_write(flash, data, nbytes);
}

uint32_t flash_read(uint32_t flash, uint32_t* data, uint32_t nbytes)
{
	assert_return_err(flash > flash_base, flash_err_parameter);
	assert_return_err(data, flash_err_parameter);

	uint32_t status = success;
	
	uint32_t nwords = (nbytes & 3) ? ((nbytes >> 2) + 1) : (nbytes >> 2);

	for(uint32_t iwords = 0; iwords < nwords; iwords++)
	{
		*(data + iwords) = *(uint32_t*)(flash + iwords * 4);
	}
	return status;
}


