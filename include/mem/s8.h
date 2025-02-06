#ifndef INCLUDE_S8_H
#define INCLUDE_S8_H

#define s8_cntof(x) (unsigned int)(sizeof(x) / sizeof(*(x)))
#define s8_lenof(x) (s8_cntof(x) - 1)
#define s8(x) (s8){(unsigned char *)x, s8_lenof(x)}
typedef struct {
	unsigned char *data;
	unsigned int   len;
} s8;

#endif // INCLUDE_S8_H

