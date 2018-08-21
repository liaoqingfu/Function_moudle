#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>

void debug_log(const char *fileName, const char *format, ...);

#ifdef __cplusplus
}
#endif
#endif	// __DEBUG_LOG_H__
