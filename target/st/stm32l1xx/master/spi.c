/*****************************************************************************************
* @file               : spi.c
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Source files for spi.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "includes.h"
#include "errno.h"
#include "assert.h"
#include "spi.h"
/*macros --------------------------------------------------------------------------------*/
static uint32_t spi_clk[]={
	RCC_APB2Periph_SPI1,
	RCC_APB1Periph_SPI2,
	RCC_APB1Periph_SPI3
};

static void* spi_port[]={
	SPI1,
	SPI2,
	SPI3
};

static uint16_t spi_attrs[]={
	/*data direction*/
	SPI_Direction_2Lines_FullDuplex,
	SPI_Direction_2Lines_RxOnly,
	SPI_Direction_1Line_Rx,
	SPI_Direction_1Line_Tx,
	/*mode*/
	SPI_Mode_Master,
	SPI_Mode_Slave,
	/*data size*/
	SPI_DataSize_8b,
	SPI_DataSize_16b,
	/*clock polarity */
	SPI_CPOL_Low,
	SPI_CPOL_High,
	/*clock phase*/
	SPI_CPHA_1Edge,
	SPI_CPHA_2Edge,
	/*slave select management*/
	SPI_NSS_Soft,
	SPI_NSS_Hard,
	/*baudrate prescaler*/
	SPI_BaudRatePrescaler_2,
	SPI_BaudRatePrescaler_4,
	SPI_BaudRatePrescaler_8,
	SPI_BaudRatePrescaler_16,
	SPI_BaudRatePrescaler_32,
	SPI_BaudRatePrescaler_64,
	SPI_BaudRatePrescaler_128,
	SPI_BaudRatePrescaler_256,
	/*first bit direction*/
	SPI_FirstBit_MSB,
	SPI_FirstBit_LSB
};

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/
uint32_t stm32l1xx_spi_init(uint8_t port, uint8_t datadir, uint8_t mode, 
						uint8_t datasize, uint8_t clkpol, uint8_t clkpha,
						uint8_t chipsel, uint8_t baudrate, uint8_t firstbit)
{
	assert_return_err(port < dim(spi_port), spi_err_parameter);

	if(port == 0)
		RCC_APB2PeriphClockCmd(spi_clk[port], ENABLE);
	else
		RCC_APB1PeriphClockCmd(spi_clk[port], ENABLE);

	SPI_InitTypeDef spi_cfg = {};

	spi_cfg.SPI_Direction         = spi_attrs[datadir];
	spi_cfg.SPI_Mode              = spi_attrs[mode];
	spi_cfg.SPI_DataSize          = spi_attrs[datasize];
	spi_cfg.SPI_CPOL              = spi_attrs[clkpol];
	spi_cfg.SPI_CPHA              = spi_attrs[clkpha];
	spi_cfg.SPI_NSS               = spi_attrs[chipsel];
	spi_cfg.SPI_BaudRatePrescaler = spi_attrs[baudrate];
	spi_cfg.SPI_FirstBit          = spi_attrs[firstbit];
	spi_cfg.SPI_CRCPolynomial     = 7;
	
	SPI_Init(spi_port[port], &spi_cfg);

	SPI_Cmd(spi_port[port], ENABLE);

	return success;
}

uint32_t stm32l1xx_spi_send_byte(uint8_t port, uint16_t *xmtdata, uint16_t *rcvdata)
{
	assert_return_err(port < dim(spi_port), spi_err_parameter);
	
	while(SPI_I2S_GetFlagStatus(spi_port[port], SPI_I2S_FLAG_TXE) == RESET);

	SPI_I2S_SendData(spi_port[port], *xmtdata);

	while(SPI_I2S_GetFlagStatus(spi_port[port], SPI_I2S_FLAG_RXNE) == RESET);

	*rcvdata = SPI_I2S_ReceiveData(spi_port[port]);
	
	return success;
}

/*public --------------------------------------------------------------------------------*/
uint32_t spi_init(spi_t *spi)
{
	assert_return_err(spi, spi_err_parameter);

	if(null != spi->nsspin)
		gpio_init(spi->nsspin);

	if(null != spi->sckpin)
		gpio_init(spi->sckpin);

	if(null != spi->misopin)
		gpio_init(spi->misopin);

	if(null != spi->mosipin)
		gpio_init(spi->mosipin);

	stm32l1xx_spi_init(spi->port, spi->datadir, spi->mode, 
						spi->datasize, spi->clkpol, spi->clkpha,
						spi->chipsel, spi->baudrate, spi->firstbit);
	return success;
}

uint32_t spi_send(spi_t *spi, void* xmtdata, uint32_t ndata)
{
	uint16_t rcvdata = 0;
	
	assert_return_err(spi, spi_err_parameter);

	spi->xmtdata = xmtdata;

	stm32l1xx_spi_send_byte(spi->port, (uint16_t*)spi->xmtdata, &rcvdata);
	
	return success;
}

uint32_t spi_receve(spi_t *spi, void* rcvdata, uint32_t ndata)
{
	uint16_t xmtdata = 0;

	assert_return_err(spi, spi_err_parameter);

	stm32l1xx_spi_send_byte(spi->port, &xmtdata, (uint16_t*)rcvdata);

	rcvdata = spi->rcvdata;
	
	return success;
}

