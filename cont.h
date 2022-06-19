#ifndef _CONT_H
#define _CONT_H

#include <sys/ptrace.h>
#include <capstone/capstone.h>

#include "break.h"
#include "libmain.h"

int si(int, char[][100]);
int cont(int, char[][100]);
struct user_regs_struct get_regs_info();

#endif