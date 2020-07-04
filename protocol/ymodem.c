/*****************************************************************************************
* @file               : ymodem.c
* @author             : ayangs
* @date               : 2019/10/06
* @brief              : Source files for ymodem.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "ymodem.h"
/*macros --------------------------------------------------------------------------------*/
#define ymodem_number_index                0
#define ymodem_number_complement_index     1
#define ymodem_data_index                  2

#define ymodem_datasize_128                128
#define ymodem_datasize_1024               1024
#define ymodem_crcsize                     2
#define ymodem_numbersize                  2

#define ymodem_timeout                     500

#define ymodem_file_name_size              128

#define ymodem_eot_req_cnt                 1
#define ymodem_packet_retry_cnt            10
#define ymodem_error_cnt                   10
#define ymodem_wait_mincnt                 10
#define ymodem_wait_maxcnt                 -1

/*typedefs ------------------------------------------------------------------------------*/
typedef enum ymodem_state{
	ymodem_state_idle,
	ymodem_state_rx,
	ymodem_state_ack,
	ymodem_state_exit
}ymodem_state_e;
/*variables -----------------------------------------------------------------------------*/
static char ymodem_rbuf[ymodem_datasize_1024 + ymodem_crcsize + ymodem_numbersize];
static char filenamebuf[ymodem_file_name_size];
static char filesizebuf[ymodem_file_name_size];
static uart_t* ymodem_uart = 0;
static uint32_t rcv_packet_number = 0;
static uint8_t rcv_packet_stage = 0;
/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/
uint32_t ymodem_getchar(char *c, uint32_t n, int32_t timeout)
{
	assert_return_err(ymodem_uart, ymodem_err_parameter);
	
	uint32_t ret = uart_getc(ymodem_uart, c, n, timeout);
	
	return ret;
}

uint32_t ymodem_putchar(char c)
{
	assert_return_err(ymodem_uart, ymodem_err_parameter);
	
	uint32_t ret = uart_putc(ymodem_uart, c);
	
	return ret;
}

uint16_t ymodem_crc(char *data, uint16_t length)
{
    uint16_t crc = 0u;
    while (length)
    {
        length--;
        crc = crc ^ ((uint16_t)*data++ << 8u);
        for (uint8_t i = 0u; i < 8u; i++)
        {
            if (crc & 0x8000u)
            {
                crc = (crc << 1u) ^ 0x1021u;
            }
            else
            {
                crc = crc << 1u;
            }
        }
    }
    return crc;
}

uint32_t ymodem_str2u32(char* str)  
{  
	char*    s = str;  
	uint32_t acc = 0;  
	int c;  

	do {  
		c = *s++;
	} while (c == ' ');
	
	for(acc = 0; (c >= '0') && (c <= '9'); c = *s++)
	{
		c -= '0';
		acc *= 10;
		acc += c;
	}  
	return acc;  
}

uint32_t ymodem_parse_file(char* buf, char* name,char* size)
{
	uint16_t fidx = 0;
	char*    fptr = 0;
	uint32_t filesz = 0;

	fptr = buf + ymodem_data_index;
	fidx = 0;
	while((*fptr != 0) && (fidx < ymodem_file_name_size))
	{
		name[fidx++] = *fptr++;
	}
	name[fidx++] = '\0';

	/* parse filesize */
	fptr = buf + ymodem_data_index + fidx;
	fidx = 0;
	while((*fptr != ' ') && (fidx < ymodem_file_name_size))
	{
		size[fidx++] = *fptr++;
	}
	size[fidx++] = '\0';

	filesz = ymodem_str2u32(size);

	return filesz;
}

uint32_t ymodem_show_progress(char* title,uint32_t cur, uint32_t total)
{
	uint8_t space = 20;
	uint32_t progress = cur*100/total;
	//initial state
	if(0 == cur)
	{
		log_printf("\n\r%*.*s: [",9,9,title);
		for(uint8_t i = 0; i < space + 1 + 4; i++)
		{
			log_printf(" ");
		}
	}

	//back space
	for(uint8_t i = 0; i < space + 1 + 4; i++)
	{
		log_printf("\b");
	}

	//show progress(20)
	for(uint8_t i = 0; i < space; i++)
	{
		if((progress*space) >= i*100)
		{
			log_printf("#");
		}
		else
		{
			log_printf("-");
		}
	}

	//show line ']' (1)
	log_printf("]");

	//show percent (4)
	log_printf("%3d%.",progress);

	return success;
}

uint32_t ymodem_rx_process(char header)
{
	static uint32_t ymodem_filesize = 0;
	static uint32_t ymodem_rcvdsize = 0;
	static uint8_t  packet_index = 0;
	uint16_t size = 0, length = 0;
	uint16_t crc_recv = 0, crc_calc = 0;
	uint16_t rcv_size = 0;
	uint32_t ret = success;
	
	switch (header)
	{
		case y_soh:
		case y_stx:
			memset(ymodem_rbuf,0,sizeof(ymodem_rbuf));
			memset(filenamebuf,0,sizeof(filenamebuf));
			memset(filesizebuf,0,sizeof(filesizebuf));
			
			/* check reception */
			size   = (y_stx == header) ? ymodem_datasize_1024 : ymodem_datasize_128;
			length = size + ymodem_data_index + ymodem_crcsize;
			ret    = ymodem_getchar(&ymodem_rbuf[ymodem_number_index], length, ymodem_timeout);
			if(success != ret)
				break;

			log_out(verbose,"recv length: %d, index : %d.",length, ymodem_rbuf[ymodem_number_index]);
#if log_level <= verbose
			log_out(verbose,"data:");
			for(uint16_t i = 0; i < length; i++)
				log_printf(" %x",ymodem_rbuf[i]);
#endif

			/* check crc */
			crc_recv = ((uint16_t)ymodem_rbuf[length-2] << 8) | ((uint16_t)ymodem_rbuf[length-1]);
			crc_calc = ymodem_crc(&ymodem_rbuf[ymodem_data_index], size);
			if(crc_recv != crc_calc)
			{
				log_out(error,"crc error, recv : %x, calc : %x.",crc_recv, crc_calc);
				ret = ymodem_err_crc;
				break;
			}

			/* check number */
			if(0xff != ymodem_rbuf[ymodem_number_index] + ymodem_rbuf[ymodem_number_complement_index])
			{
				log_out(error,"number error : %d.",ymodem_rbuf[ymodem_number_index] + ymodem_rbuf[ymodem_number_complement_index]);
				ret = ymodem_err_number;
				break;
			}

			//check stage
			if(ymodem_rbuf[ymodem_number_index] == 0)
			{
				packet_index = 0;
				if(0 == crc_calc)//last packet
				{
					rcv_packet_stage = 2;
					ret = success;
					break;
				}
				else if(0 == ymodem_filesize)//idle state, check if first packet
				{
					ymodem_filesize = ymodem_parse_file(ymodem_rbuf, filenamebuf, filesizebuf);
					ymodem_rcvdsize = 0;
					
					if(('\0' != filenamebuf[0])&&(0 != ymodem_filesize))//first packet
					{
						rcv_packet_stage = 0;
						rcv_packet_number = 0;
					}
					else//last packet
					{
						ymodem_filesize = 0;
						memset(filenamebuf,0,sizeof(filenamebuf));
						rcv_packet_stage = 2;
						ret = success;
						break;
					}
				}
				else//middle packet
				{
					rcv_packet_stage = 1;
					rcv_packet_number = ((rcv_packet_number + 1)&(~0xff));
				}
			}
			else if(ymodem_rbuf[ymodem_number_index] == (packet_index + 1))	//new msg packets
			{
				packet_index = ymodem_rbuf[ymodem_number_index];
				rcv_packet_stage = 1;
				rcv_packet_number = (rcv_packet_number&(~0xff)) + packet_index;
			}
			else if(ymodem_rbuf[ymodem_number_index] == packet_index)	//same msg packets
			{
				ret = success;
				break;
			}
			else
			{
				log_out(error,"number error : %d, %d.",ymodem_rbuf[ymodem_number_index],packet_index);
				ret = ymodem_err_number;
				break;
			}

			/* stage process */
			if(0 == rcv_packet_stage)	//first packet
			{
				log_out(info,"file name: %s.",filenamebuf);
				log_out(info,"file size: %s byte.",filesizebuf);
				ymodem_rcv_file_info_handler(ymodem_filesize);
				ymodem_show_progress(filenamebuf,ymodem_rcvdsize,ymodem_filesize);
			}
			else if(1 == rcv_packet_stage)//middle packet
			{
				if(ymodem_rcvdsize + size <= ymodem_filesize)
				{
					rcv_size = size;
				}
				else
				{
					rcv_size = ymodem_filesize - ymodem_rcvdsize;
				}
				ymodem_rcv_file_data_handler(ymodem_rcvdsize,(uint32_t*)&ymodem_rbuf[ymodem_data_index],(uint32_t)rcv_size);
				ymodem_rcvdsize = ymodem_rcvdsize + rcv_size;
				
				ymodem_show_progress(filenamebuf,ymodem_rcvdsize,ymodem_filesize);
#if log_level <= verbose
				log_out(verbose,"rcvd %d byte, info:",ymodem_rcvdsize);
				for(uint16_t i = 0; i < rcv_size; i++)
					log_printf(" %x",ymodem_rbuf[ymodem_data_index + i]);
#endif
			}

		break;
		case y_eot:
			ymodem_rcv_file_dump_handler(ymodem_filesize);
			ymodem_filesize = 0;
			return success;
		break;
		default:
		break;
	}

	return ret;
}

uint32_t ymodem_ack_process(char ack)
{
	uint32_t ret = success;
	switch (ack)
	{
		case y_soh:
		case y_stx:
			ymodem_putchar(y_ack);
			ymodem_putchar(y_c16);
			log_out(debug, "send ack & c.");
			break;
		case y_c16:
			ymodem_putchar(y_c16);
			log_out(debug, "send c.");
			break;
		case y_ack:
			ymodem_putchar(y_ack);
			log_out(debug, "send ack.");
			break;
		case y_eot:
			ymodem_putchar(y_ack);
			ymodem_putchar(y_c16);
			log_out(debug, "send ack & c.");
			break;
		case y_nak:
			ymodem_putchar(y_nak);
			log_out(debug, "send nak.");
			break;
		default:
			break;
	}
	return ret;
}

uint32_t ymodem_gethead(char *c)
{
	uint32_t ret = success;

	ret = ymodem_getchar(c, 1, ymodem_timeout);
	
	if(success != ret)
		return ret;
	
	if((*c != y_soh)&&(*c != y_stx)&&(*c != y_eot))
		ret = ymodem_err_head;
	
	return ret;
}
/*public --------------------------------------------------------------------------------*/
uint32_t ymodem_init(uart_t* uart)
{
	assert_return_err(uart, ymodem_err_parameter);

	ymodem_uart = uart;

	return success;
}

uint32_t ymodem_receive(void)
{
	uint8_t  state  = 0;
	char     header = 0;
	char     ackcmd = 0;
	char     resend = 0;
	char     errors = 0;
	char     waits  = 0;
	char     nwait  = ymodem_wait_maxcnt;
	uint32_t ret    = 0;
	rcv_packet_stage = 0;
	rcv_packet_number = 0;
	
	while(1)
	{
		log_out(verbose, "state:%d",state);
		switch(state)
		{
			case ymodem_state_idle:
				header = 0;
				ret = ymodem_gethead(&header);
				log_out(verbose, "header: %d, ret: 0x%x\n\r",header,ret);
				if(success == ret)
				{
					state = ymodem_state_rx;
					
					nwait  = ymodem_wait_mincnt;
					waits = 0;
				}
				else
				{
					ackcmd = y_c16;
					state = ymodem_state_ack;
					
					waits++;
					if(waits >= nwait)
					{
						ackcmd = y_can;
						ret = ymodem_err_overtime;
					}
				}
				break;
			case ymodem_state_rx:
				ret = ymodem_rx_process(header);
				state = ymodem_state_ack;
				if(success == ret)
				{
					if(y_eot == header)
					{
						if(resend > 0)
						{
							resend--;
							ackcmd = y_nak;
						}
						else
						{
							ackcmd = y_eot;
						}
					}
					else
					{
						if((0 == rcv_packet_number)&&(0 == rcv_packet_stage))//first packet
						{
							ackcmd = header;
							resend = ymodem_eot_req_cnt;
						}
						else
						{
							ackcmd = y_ack;
						}
					}
				}
				else
				{
					errors++;
					if(errors < ymodem_error_cnt)
					{
						ackcmd = y_nak;
					}
					else
					{
						ackcmd = y_can;
						ret = ymodem_err_retryfail;
					}
					
				}
				break;
			case ymodem_state_ack:
				ymodem_ack_process(ackcmd);
				if((y_can == ackcmd)||
					(2 == rcv_packet_stage))
				{
					state = ymodem_state_exit;
				}
				else
				{
					state = ymodem_state_idle;
				}
				break;
			case ymodem_state_exit:
				return ret;
				break;
			default:
				break;
		}
	}
}



