#ifndef INCLUDE_GLOBAL_H
#define INCLUDE_GLOBAL_H

#include "define.h"
#include "platform.h"
#include "mem/s8.h"
#include "mem/arena.h"

global b8     mem_master;
global MArena ma_master;
global MArena ma_temp;

MArena ma_shlib;

#endif //INCLUDE_GLOBAL_H

