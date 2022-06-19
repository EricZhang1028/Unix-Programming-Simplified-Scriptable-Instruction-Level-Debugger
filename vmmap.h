#ifndef _VVMAP_H
#define _VVMAP_H

#include "libmain.h"

#define VMMAP_FORMAT "%016llx-%016llx %c%c%c %-10llx %s\n"

int vmmap(int, char[][100]);

#endif