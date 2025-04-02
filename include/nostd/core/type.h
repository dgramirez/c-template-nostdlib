#ifndef INCLUDE_TYPE_H
#define INCLUDE_TYPE_H

struct s8;

typedef union {
	void *pv;
	u8   *pu8;
	u16  *pu16;
	u32  *pu32;
	usz  *pusz;
} MemUnion;

typedef struct {
	u8 *data;
	usz len;
} s8, b8;

typedef struct {
	union {
		b8 b;
		struct { u8 *data; usz len; };
	};

	usz cap;
} sb8, mb8;

typedef struct {
	union {
		mb8 mb;
		struct {
			union {
				b8 b;
				struct { u8 *data; usz len; };
			};
			usz cap;
		};
	};

	void *fd;
	usz err;
} fb8;

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

#endif // INCLUDE_TYPE_H

