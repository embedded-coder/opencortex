/*****************************************************************************************
* @file               : errno.h
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Head files for errno.
******************************************************************************************/
#ifndef _errno_h_
#define _errno_h_
/*includes ------------------------------------------------------------------------------*/

/*macros --------------------------------------------------------------------------------*/
/* APP */
#define moudle_app                  0x1000
/* PROTOCOL */
#define moudle_protocol             0x2000
#define moudle_protocol_ymodem      (moudle_protocol | 0x0100)
/* SYSTEM */
#define moudle_system               0x3000
/* PLATFORM */
#define moudle_platform             0x4000
#define moudle_platform_log         (moudle_platform | 0x0100)

/* DRIVER */
#define moudle_driver               0x5000
/* |-MASTER DRIVER*/
#define moudle_driver_master        (moudle_driver | 0x0100)
#define moudle_driver_master_gpio   (moudle_driver_master | 0x0010)
#define moudle_driver_master_uart   (moudle_driver_master | 0x0020)
#define moudle_driver_master_spi    (moudle_driver_master | 0x0030)
#define moudle_driver_master_flash  (moudle_driver_master | 0x0040)
#define moudle_driver_master_irq    (moudle_driver_master | 0x0050)

/* |-DEVICE DRIVER*/
#define moudle_driver_device        (moudle_driver | 0x0200)
#define moudle_driver_device_led    (moudle_driver_device | 0x0010)
#define moudle_driver_device_button (moudle_driver_device | 0x0020)


/*---------------------------------------------------------------------------------------*/
#define success                  0x0000

/* 
 *	[  4bit   ] [     4bit    ] [  8bit  ]
 *	[moudle id] [sub moudle id] [error id]
*/
#define gpio_err_parameter      (moudle_driver_master_gpio   | 0x0001)

#define uart_err_parameter      (moudle_driver_master_uart   | 0x0001)
#define uart_err_rxtimeout      (moudle_driver_master_uart   | 0x0002)

#define spi_err_parameter       (moudle_driver_master_spi    | 0x0001)

#define flash_err_parameter     (moudle_driver_master_flash  | 0x0001)
#define flash_err_erase         (moudle_driver_master_flash  | 0x0002)
#define flash_err_write         (moudle_driver_master_flash  | 0x0003)
#define flash_err_readback      (moudle_driver_master_flash  | 0x0004)

#define irq_err_parameter       (moudle_driver_master_irq    | 0x0001)

#define led_err_parameter       (moudle_driver_device_led    | 0x0001)

#define button_err_parameter    (moudle_driver_device_button | 0x0001)

#define log_err_parameter       (moudle_platform_log | 0x0001)

#define ymodem_err_parameter    (moudle_protocol_ymodem | 0x0001)
#define ymodem_err_readfail     (moudle_protocol_ymodem | 0x0002)
#define ymodem_err_writefail    (moudle_protocol_ymodem | 0x0003)
#define ymodem_err_crc          (moudle_protocol_ymodem | 0x0004)
#define ymodem_err_number       (moudle_protocol_ymodem | 0x0005)
#define ymodem_err_head         (moudle_protocol_ymodem | 0x0006)
#define ymodem_err_retryfail    (moudle_protocol_ymodem | 0x0007)
#define ymodem_err_overtime     (moudle_protocol_ymodem | 0x0007)

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

#endif //_errno_h_

