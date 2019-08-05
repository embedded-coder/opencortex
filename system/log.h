/*****************************************************************************************
* @file               : log.h
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Head files for log.
******************************************************************************************/
#ifndef _log_h_
#define _log_h_
/*includes ------------------------------------------------------------------------------*/
#include "typedef.h"
#include "uart.h"
/*macros --------------------------------------------------------------------------------*/
#define tags      bit(0)

#define verbose   0   // all
#define debug     1   // debug
#define info      2   // information
#define warning   3   // warnings
#define error     4   // errors
#define fatal     5   // fatal errors
#define silent    6   // nothing

#define log_level   verbose

#define tag_verbose       "\n\r[V]"
#define tag_debug         "\n\r[D]"
#define tag_info          "\n\r[I]"
#define tag_warning       "\n\r[W]"
#define tag_error         "\n\r[E]"
#define tag_fatal         "\n\r[F]"

//#define log_longlong
#define log_special
#define log_precision

#define log_buf_size 128

#define log_out(level, fmt, ...)								\
	do{															\
		if(level >= log_level)									\
		{														\
			switch(level)										\
			{													\
				case verbose : log_printf(tag_verbose); break;	\
				case debug   : log_printf(tag_debug);   break;	\
				case info    : log_printf(tag_info);    break;	\
				case warning : log_printf(tag_warning); break;	\
				case error   : log_printf(tag_error);   break;	\
				case fatal   : log_printf(tag_fatal);   break;	\
			}													\
			log_printf(fmt, ##__VA_ARGS__);						\
		}														\
	}while(0);


/*typedefs ------------------------------------------------------------------------------*/
typedef struct log{
	char*   name;
	uart_t* uart;
	uint8_t level;
}log_t;
/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

uint32_t log_init(log_t* log);

void log_printf(const char *fmt, ...);

#endif //_log_h_

