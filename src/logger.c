#pragma once

#ifndef LOGGER_C
#define LOGGER_C

#include "includes/logger.h"
#include "includes/file_io.h"
#include <stdio.h>

//#define PRINTFUNCTION(format,args...)      fprintf(stderr, format, args)


//#pragma optimize(0,noinline,noasm,noconstparams)
void log_debug(const char *fmt, ...)
{
#if LOG_LEVEL >= DEBUG_LEVEL 				
	char buff[50];
	sformat(buff, fmt, (int *)&fmt + 1, true);
#endif
}

//#pragma optimize(0,noinline,noasm,noconstparams)
void log_info(const char *fmt, ...)
{
#if LOG_LEVEL >= INFO_LEVEL 				
	char buff[50];
	sformat(buff, fmt, (int *)&fmt + 1, true);
#endif
}

//#pragma optimize(0,noinline,noasm,noconstparams)
void log_warn(const char *fmt, ...)
{
#if LOG_LEVEL >= WARN_LEVEL 				
	char buff[50];
	sformat(buff, fmt, (int *)&fmt + 1, true);
#endif
}

//#pragma optimize(0,noinline,noasm,noconstparams)
void log_error(const char *fmt, ...)
{
#if LOG_LEVEL >= ERROR_LEVEL 				
	char buff[50];
	sformat(buff, fmt, (int *)&fmt + 1, true);
#endif
}

#endif