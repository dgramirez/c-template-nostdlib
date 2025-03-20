#ifndef INCLUDE_LOG_H
#define INCLUDE_LOG_H

#include "define.h"
#include "platform.h"

typedef enum {                 //         (AARRGGBB)
	LOG_LEVEL_GOOFY    = 0x01, // Cyan    (0000FFFF)
	LOG_LEVEL_DEBUG    = 0x02, // White   (00FFFFFF)
	LOG_LEVEL_INFO     = 0x04, // White   (00FFFFFF)
	LOG_LEVEL_SUCCESS  = 0x08, // Green   (0000FF00)
	LOG_LEVEL_ANOMALLY = 0x10, // Blue    (000000FF)
	LOG_LEVEL_WARNING  = 0x20, // Yellow  (00FFFF00)
	LOG_LEVEL_ERROR    = 0x40, // Magenta (00FF00FF)
	LOG_LEVEL_FATAL    = 0x80, // Red     (00FF0000)

	LOG_LEVEL_ASSERT   = 0x8000,
} LogLevel;

typedef enum {
	LOG_FORMAT_CONSOLE = 0x1,
	LOG_FORMAT_FILE    = 0x2,
	LOG_FORMAT_FILE_7Z = 0x4,
	LOG_FORMAT_NETWORK = 0x8
} LogFormat;

declfn(void,
       logsys,
       unref(level);unref(msg);unref(file);unref(linenum);unref(fnname);,
       u32 level,
       const char *msg,
       const char *file,
       usz linenum,
       const char *fnname);
 
#define log(level, msg) logsys(level, msg, __FILE__, __LINE__, __func__)
#define log_egg(msg)    log(LOG_LEVEL_GOOFY, msg)
#define log_debug(msg)  log(LOG_LEVEL_DEBUG, msg)
#define log_info(msg)   log(LOG_LEVEL_INFO, msg)
#define log_pass(msg)   log(LOG_LEVEL_SUCCESS, msg)
#define log_odd(msg)    log(LOG_LEVEL_ANOMALLY, msg)
#define log_warn(msg)   log(LOG_LEVEL_WARNING, msg)
#define log_error(msg)  log(LOG_LEVEL_ERROR, msg)
#define log_fatal(msg)  log(LOG_LEVEL_FATAL, msg)
#define log_assert(msg) log(LOG_LEVEL_FATAL | LOG_LEVEL_ASSERT, msg)

#endif // INCLUDE_LOG_H

