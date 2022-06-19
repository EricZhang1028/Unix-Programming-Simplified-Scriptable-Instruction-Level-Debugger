#ifndef _BREAK_H
#define _BREAK_H

#include <sys/ptrace.h>
#include <sys/wait.h>

#include "libmain.h"
#include "cont.h"

struct break_point {
    int id;
    unsigned long long addrress;
    unsigned char orig_cmd;
};

unsigned char change_byte(unsigned long long, unsigned char);
int brk(int, char [][100]);
int list(int, char [][100]);
int del(int, char [][100]);

#endif