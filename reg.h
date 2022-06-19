#ifndef _REG_H
#define _REG_H

#include <sys/ptrace.h>
#include <sys/wait.h>
#include "libmain.h"

int get_regs(int, char[][100]);
int get_reg(int, char[][100]);
int set_reg(int, char[][100]);

#endif