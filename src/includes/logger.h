#include <stdbool.h>

#include "log_level.h"

#define ALL_LEVEL		100
#define DEBUG_LEVEL     6
#define INFO_LEVEL      5
#define WARN_LEVEL		2
#define ERROR_LEVEL     1
#define NONE_LEVEL		0

//#pragma optimize(0)
void log_debug(const char *fmt, ...);
//#pragma optimize(0)
void log_info(const char *fmt, ...);
//#pragma optimize(0)
void log_warn(const char *fmt, ...);
//#pragma optimize(0)
void log_error(const char *fmt, ...);

extern char * sformat(char * buff, const char * fmt, int * fps, bool print);

