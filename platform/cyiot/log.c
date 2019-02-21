/*****************************************************************************************
* @file               : log.c
* @author             : ayangs
* @date               : 2019/02/19
* @brief              : Source files for log.
******************************************************************************************/
/*includes ------------------------------------------------------------------------------*/
#include "errno.h"
#include "assert.h"
#include "log.h"
/*macros --------------------------------------------------------------------------------*/

#define tag_verbose       "\n\r[V]> "
#define tag_debug         "\n\r[D]> "
#define tag_info          "\n\r[I]> "
#define tag_warning       "\n\r[W]> "
#define tag_error         "\n\r[E]> "
#define tag_fatal         "\n\r[F]> "
#define tag_silent        "\n\r[S]> "

/*typedefs ------------------------------------------------------------------------------*/

/*variables -----------------------------------------------------------------------------*/
static char log_tag[][10]={
	tag_verbose,
	tag_debug,
	tag_info,
	tag_warning,
	tag_error,
	tag_fatal,
	tag_silent
};

log_t* glog = null;
/*prototypes ----------------------------------------------------------------------------*/

/*private -------------------------------------------------------------------------------*/

/*public --------------------------------------------------------------------------------*/

uint32_t log_init(log_t* log)
{
	assert_return_err(log, log_err_parameter);

	glog = log;
	
	uart_init(glog->uart);

	return success;
}

uint32_t log_str(uint8_t level, uint32_t flags, char *s)
{
	assert_return_err(glog, log_err_parameter);
	assert_return_err(glog->uart, log_err_parameter);

	if(level < glog->level)
		return success;

	if(flags & tags){
		uart_send_str(glog->uart, log_tag[level]);
	}

	uart_send_str(glog->uart, s);

	return success;
}

uint32_t log_dec(uint8_t level, uint32_t flags, uint32_t l, uint8_t len)
{
	assert_return_err(glog, log_err_parameter);
	assert_return_err(glog->uart, log_err_parameter);

	if(level < glog->level)
		return success;

	if(flags & tags)
		uart_send_str(glog->uart, log_tag[level]);

	uart_send_dec(glog->uart, l, len);

	return success;
}


