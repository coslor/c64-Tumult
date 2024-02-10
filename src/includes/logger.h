#pragma once

#pragma message("***in logger.h")
#ifdef CLC_LOGGING
	#pragma message("CLC_LOGGING is defined")
#else
	#pragma message("CLC_LOGGING is undefined")
#endif

#ifndef LOGGER_H
#define LOGGER_H

#include <stdbool.h>
#include <c64/types.h>

//#include "log_level.h"

#define ALL_LEVEL		0
#define DEBUG_LEVEL     1
#define INFO_LEVEL      2
#define WARN_LEVEL		3
#define ERROR_LEVEL     4
#define NONE_LEVEL		100

void set_log_level(byte new_level);
byte get_log_level();


//#pragma optimize(0)
void log_debug(const char *fmt, ...);
//#pragma optimize(0)
void log_info(const char *fmt, ...);
//#pragma optimize(0)
void log_warn(const char *fmt, ...);
//#pragma optimize(0)
void log_error(const char *fmt, ...);

inline const char* get_log_level_name();

extern char * sformat(char * buff, const char * fmt, int * fps, bool print);


#endif