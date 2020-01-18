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
//level
#define verbose        0   // all
#define debug          1   // debug
#define info           2   // information
#define warning        3   // warnings
#define error          4   // errors
#define fatal          5   // fatal errors
#define silent         6   // nothing

//tag
#define tag_verbose       "\n\r[V]"
#define tag_debug         "\n\r[D]"
#define tag_info          "\n\r[I]"
#define tag_warning       "\n\r[W]"
#define tag_error         "\n\r[E]"
#define tag_fatal         "\n\r[F]"

//front color
#define log_fc_default   "\033[0m"
#define log_fc_black     "\033[30m"
#define log_fc_red       "\033[31m"
#define log_fc_green     "\033[32m"
#define log_fc_yellow    "\033[33m"
#define log_fc_blue      "\033[34m"
#define log_fc_purple    "\033[35m"
#define log_fc_dark      "\033[36m"
#define log_fc_white     "\033[37m"

#define log_level   info

#define log_color

//#define log_longlong //if open, there will be problem,but why?
#define log_special
#define log_precision

#define log_buf_size 128

#ifdef log_color
#define log_set_color(c) log_printf(c)
#else
#define log_set_color(c)
#endif

#define log_out(level, fmt, ...)								\
	do{															\
		if(level >= log_level)									\
		{														\
			switch(level)										\
			{													\
				case verbose:                                   \
					log_set_color(log_fc_default);              \
					log_printf(tag_verbose);                    \
					break;	                                    \
				case debug:                                     \
					log_set_color(log_fc_default);              \
					log_printf(tag_debug);                      \
					break;	                                    \
				case info:                                      \
					log_set_color(log_fc_green);                \
					log_printf(tag_info);                       \
					break;	                                    \
				case warning:                                   \
					log_set_color(log_fc_yellow);               \
					log_printf(tag_warning);                    \
					break;	                                    \
				case error:                                     \
					log_set_color(log_fc_purple);               \
					log_printf(tag_error);                      \
					break;	                                    \
				case fatal:                                     \
					log_set_color(log_fc_red);                  \
					log_printf(tag_fatal);                      \
					break;	                                    \
			}													\
			log_printf(fmt, ##__VA_ARGS__);						\
			log_set_color(log_fc_default);                      \
		}														\
	}while(0);

#define log_here	\
        log_printf("\n\rHere %s:%d", __FUNCTION__, __LINE__);

/*typedefs ------------------------------------------------------------------------------*/
typedef struct log{
	char*   name;
	uart_t* uart;
	uint8_t level;
}log_t;
/*variables -----------------------------------------------------------------------------*/

/*prototypes ----------------------------------------------------------------------------*/

uint32_t log_init(uart_t* uart);

void log_printf(const char *fmt, ...);

int log_scanf(const char * buf, const char * fmt, ...);

uint32_t log_getchar(char *c, int32_t timeout);

#endif //_log_h_

