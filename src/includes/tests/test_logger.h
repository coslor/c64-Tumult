#pragma message("***in test_logger.h")
#ifdef CLC_LOGGING
	#pragma message("CLC_LOGGING is defined")
#else
	#pragma message("CLC_LOGGING is undefined")
#endif

#pragma once

#include "tests.h"

const bool test_log_levels(char *description,int num_args, const uint32_t args[MAX_TEST_ARGS]);