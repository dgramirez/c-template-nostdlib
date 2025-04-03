#ifndef INCLUDE_NOSTD_COMMON_LOG_H
#define INCLUDE_NOSTD_COMMON_LOG_H

declfn(void,
       logsz,
       unref(level);unref(msg);unref(file);unref(linenum);unref(fnname);,
       u32 level,
       const char *msg,
       const char *file,
       usz linenum,
       const char *fnname);

declfn(void,
       logs8,
       unref(level);unref(msg);unref(file);unref(linenum);unref(fnname);,
       u32 level,
       s8 msg,
       const char *file,
       usz linenum,
       const char *fnname);

#endif // INCLUDE_NOSTD_COMMON_LOG_H

