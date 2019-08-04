/*****************************************************************************************
* @file               : spi.h
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Head files for spi.
******************************************************************************************/

#ifndef _spi_h_
#define _spi_h_
/*includes ------------------------------------------------------------------------------*/
#include "gpio.h"
#include "typedef.h"
/*macros --------------------------------------------------------------------------------*/

/*typedefs ------------------------------------------------------------------------------*/
typedef struct spi{
	char*     name;
	gpio_t*   nsspin;
	gpio_t*   sckpin;
	gpio_t*   misopin;
	gpio_t*   mosipin;
	uint8_t   datadir;
	uint8_t   mode;
	uint8_t   datasize;
	uint8_t   clkpol;
	uint8_t   clkpha;
	uint8_t   chipsel;
	uint8_t   baudrate;
	uint8_t   firstbit;
	void*     rcvdata;
	void*     xmtdata; 
	uint8_t   datawidth;
	uint8_t   port;
}spi_t;

typedef enum spi_attr{
	/*data direction*/
	datadir_2line_trx,
	datadir_2line_rx,
	datadir_1line_rx,
	datadir_1line_tx,
	/*mode*/
	mode_master,
	mode_slave,
	/*data size*/
	datasize_8b,
	datasize_16b,
	/*clock polarity */
	clkpol_low,
	clkpol_high,
	/*clock phase*/
	clkpha_1edge,
	clkpha_2edge,
	/*slave select management*/
	chipsel_soft,
	chipsel_hard,
	/*baudrate prescaler*/
	baudrate_2,
	baudrate_4,
	baudrate_8,
	baudrate_16,
	baudrate_32,
	baudrate_64,
	baudrate_128,
	baudrate_256,
	/*first bit direction*/
	firstbit_msb,
	firstbit_lsb
}spi_attr_e;

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/
uint32_t spi_init(spi_t *spi);

uint32_t spi_send(spi_t *spi, void* xmtdata, uint32_t ndata);

uint32_t spi_receve(spi_t *spi, void* rcvdata, uint32_t ndata);

#endif //_spi_h_

