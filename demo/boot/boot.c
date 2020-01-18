/*****************************************************************************************
* @file               : boot.c
* @author             : ayangs
* @date               : 2019/09/15
* @brief              : Source files for boot.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "platform.h"
#include "boot.h"

/*macros --------------------------------------------------------------------------------*/
#define boot_bootloadr    flash_page(0)  //0x08000000
#define boot_application0 flash_page(64) //0x08020000
#define boot_application1 flash_page(128)//0x08040000
#define boot_info_page    flash_page(255)//0x080‭7f800‬

#define boot_magic_nbr    (0x4f50454e)
/*typedefs ------------------------------------------------------------------------------*/
typedef void (*boot_func_ptr)(void);
typedef struct _boot_info_t_{
	uint32_t magic_nbr;
	uint32_t application0_addr;
	uint32_t application1_addr;
	uint32_t effective_application:1;
	uint32_t reserved:30;
}boot_info_t;

/*variables -----------------------------------------------------------------------------*/
static uint8_t enter_boot_mode = 0;
static uint8_t boot_option = 1;
static uint8_t boot_option_effective = 0;
static boot_info_t boot_info;

/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/
void boot_logo(void)
{
	char* date = get_build_date();
	char* ver  = get_build_version();
	
	log_printf("\f\n\r+-----------------------------------+\n\r");
	log_printf("| _  _  _  _  _ _  __|_ _           |\n\r");
	log_printf("|(_)|_)(/_| |(_(_)|  | (/_><        |\n\r");
	log_printf("|   |                               |\n\r");
	log_printf("+-----------------------------------+\n\r");
	log_printf("|2018-2010 Copyright @ EmbeddedCoder|\n\r");
	log_printf("|           v%s Build %s|\n\r",ver,date);
	log_printf("+-----------------------------------+\n\r");
}

void bootbtn_pess_edge_handler(void *button)
{
	enter_boot_mode = 1;
}

void bootopt_short_press_handler(void *button)
{
	boot_option += 1;
	if(boot_option > 2)
	{
		boot_option = 1;
	}
}

void bootopt_press_delay_handler(void *button)
{
	boot_option_effective = 1;
}

void boot_enter(void)
{
	button_attach(&buttons[0],button_event_press_edge,bootbtn_pess_edge_handler);

	log_printf("Press Button to Enter BootLoader...%ds", 5);

	for(int32_t s = 4; s >= 0; s--)
	{
		for(uint32_t i = 0; i < 50; i++)
		{
			button_get_status();
			button_event_process();
			delay_ms(20);
		}
		log_printf("\b\b%ds", s);
		
		if(enter_boot_mode)
			break;
	}
	
	log_printf("\n\r");
	button_dettach(&buttons[0],button_event_press_edge);
}

void boot_menu(void)
{
	log_printf("[1] Jump To App\n\r");
	log_printf("[2] App Upgrade\n\r");
	
	log_printf("Press Button to Select Option: %d", boot_option);
	
	button_attach(&buttons[0],button_event_short_click,bootopt_short_press_handler);
	button_attach(&buttons[0],button_event_press_delay,bootopt_press_delay_handler);
	while(0 == boot_option_effective)
	{
		for(uint32_t i = 0; i < 50; i++)
		{
			button_get_status();
			button_event_process();
			delay_ms(20);
		}
		log_printf("\b%d", boot_option);
	}
	button_dettach(&buttons[0],button_event_short_click);
	button_dettach(&buttons[0],button_event_press_delay);
}

void boot_info_init(void)
{
	memset(&boot_info, 0, sizeof(boot_info));
	
	boot_info.magic_nbr              = boot_magic_nbr;
	boot_info.application0_addr      = boot_application0;
	boot_info.application1_addr      = boot_application1;
	boot_info.effective_application  = 0;
	
	flash_erase(boot_info_page, flash_page_size);
	flash_write(boot_info_page, (uint32_t*)&boot_info, sizeof(boot_info));
}

void boot_info_read(void)
{
	uint32_t data[flash_page_size/4]={};
	
	flash_read(boot_info_page, data, sizeof(boot_info));

	memcpy(&boot_info, data, sizeof(boot_info));

	if(boot_magic_nbr != boot_info.magic_nbr)
	{
		boot_info_init();
	}
}

void boot_info_update(void)
{

}

void boot_jump_to_app(void)
{
	//boot_func_ptr boot_to_app;
	uint32_t application_addr = 0;

	if(boot_info.effective_application)
		application_addr = boot_info.application1_addr;
	else
		application_addr = boot_info.application0_addr;
	
    if (((*(volatile uint32_t*)application_addr) & 0x2FFE0000 ) == 0x20000000)
    {
		//boot_to_app = (boot_func_ptr)(*(volatile uint32_t*) (application_addr + 4u));
		log_out(fatal, "msp:0x%x", __get_MSP());
		__set_MSP(*(volatile uint32_t*)application_addr);
		log_out(fatal, "msp:0x%x", __get_MSP());
		//boot_to_app();
    }
	else
	{
		log_out(fatal, "application address wrong:0x%x -> 0x%x\n\r", application_addr,*(volatile uint32_t*)application_addr);
	}
}

uint32_t ymodem_rcv_file_info_handler(uint32_t size)
{
#if 0
	return flash_erase(boot_application0, size);
#else
	return success;
#endif
}

uint32_t ymodem_rcv_file_data_handler(uint32_t base, uint32_t *data, uint32_t size)
{
#if 0
	return flash_write(boot_application0 + base, data, size);
#else
	return success;
#endif
}

uint32_t ymodem_rcv_file_dump_handler(uint32_t size)
{
#if 0
	log_out(debug,"dump:");
	for(uint32_t i = 0; i < size; i++)
		log_printf(" %d",*(char*)(boot_application0 + i));
#endif
	return success;
}
/*public --------------------------------------------------------------------------------*/

void example_process(void)
{
	boot_logo();
	boot_info_read();
	
	boot_enter();
	
	if(enter_boot_mode)
	{
		boot_menu();
	}
	
	if(2 == boot_option)
	{
		log_out(info,"enter upgrade mode...");
		ymodem_receive();
	}
	else
	{
		log_out(info,"enter application...");
		boot_jump_to_app();
	}

	boot_info_update();

}




