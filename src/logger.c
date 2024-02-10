#pragma message("***in logger.c")

#ifdef CLC_LOGGING
	#pragma message("CLC_LOGGING is defined")
#else
	#pragma message("CLC_LOGGING is undefined")
#endif

#pragma once

#ifndef LOGGER_C
#define LOGGER_C


#include "includes/log_level.h"
#include "includes/logger.h"
#include "includes/file_io.h"
#include <stdio.h>
#include <c64/types.h>

//#define PRINTFUNCTION(format,args...)      fprintf(stderr, format, args)

const char *_LOG_LEVEL_NAMES[]= {"ALL","DEBUG", "INFO", "WARN", "ERROR","NONE"};
#ifdef CLC_LOGGING
#define DO_DEBUG(LEVEL) 										\
		if (_log_level <= LEVEL) { 								\
			puts(_LOG_LEVEL_NAMES[LEVEL]);	putchar(':');		\
			sformat(_debug_buff, fmt, (int *)&fmt + 1, true);	\
		};
#else
#define DO_DEBUG(LEVEL) 
#endif

#ifdef CLC_LOGGING
volatile byte _log_level=ALL_LEVEL;
char _debug_buff[80];
#endif

inline void set_log_level(byte new_level) {
	#ifdef CLC_LOGGING
	if (new_level < ALL_LEVEL || new_level > NONE_LEVEL) {
		log_error("logger.set-log():bad log level %d",new_level);
		return;
	}
	_log_level=new_level;
	#endif
}

inline byte get_log_level() {
	#ifdef CLC_LOGGING
	return _log_level;
	#else
	return 0;
	#endif
};

inline bool would_log(int error_level) {
	#ifdef CLC_LOGGING
	return (_log_level <= error_level);
	#else
	return false;
	#endif
}
inline const char* get_log_level_name() {
	#ifdef CLC_LOGGING
	return _LOG_LEVEL_NAMES[_log_level];
	#else
	return NULL;
	#endif
}
//#pragma optimize(0,noinline,noasm,noconstparams)
inline void log_debug(const char *fmt, ...)
{
	DO_DEBUG(DEBUG_LEVEL);
	// if (_log_level >= DEBUG_LEVEL) {
	// 	sformat(_debug_buff, fmt, (int *)&fmt + 1, true);
	// }
} 

//#pragma optimize(0,noinline,noasm,noconstparams)
inline void log_info(const char *fmt, ...)
{
	DO_DEBUG(INFO_LEVEL);

// #if LOG_LEVEL <= INFO_LEVEL 				
// 	char buff[50];
// 	sformat(buff, fmt, (int *)&fmt + 1, true);
// #endif
}

//#pragma optimize(0,noinline,noasm,noconstparams)
inline void log_warn(const char *fmt, ...)
{
	DO_DEBUG(WARN_LEVEL);
// #if LOG_LEVEL <= WARN_LEVEL 				
// 	char buff[50];
// 	sformat(buff, fmt, (int *)&fmt + 1, true);
// #endif
}

//#pragma optimize(0,noinline,noasm,noconstparams)
inline void log_error(const char *fmt, ...)
{
	DO_DEBUG(ERROR_LEVEL);
// #if LOG_LEVEL <= ERROR_LEVEL 				
// 	char buff[50];
// 	sformat(buff, fmt, (int *)&fmt + 1, true);
// #endif
}

#endif