#ifndef INCLUDE_PLATFORM_LINUX_TYPE_H
#define INCLUDE_PLATFORM_LINUX_TYPE_H

typedef struct {
	fb8    fb_terminal;
	fb8    fb_file;
	fb8    fb_network;
	usz    flags_terminal;
	usz    flags_file;
	usz    flags_network;
} Logger;

#endif // INCLUDE_PLATFORM_LINUX_TYPE_H

