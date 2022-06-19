#ifndef _LIBMAIN_H
#define _LIBMAIN_H

#include<sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <sys/user.h>

#include "load.h"
#include "run_cmd.h"
#include "start.h"
#include "cont.h"
#include "break.h"
#include "reg.h"
#include "help.h"
#include "run.h"
#include "dump.h"
#include "disasm.h"
#include "vmmap.h"

#define STATE_NONE 0x00
#define STATE_RUN 0x01
#define STATE_LOAD 0x02
#define STATE_ANY 0x03

using namespace std;

typedef int (*fun)(int, char[][100]);

struct Command {
    char* name;
    char* shortcut;
    char* help_msg;
    fun cmd_fun;
};

struct disasm {
    char* data;
    int offset;
    int size;
    unsigned long long cur_virtual_address;
    int dumpped;
};


struct elf {
    unsigned long long entry_point;
    unsigned long long virtual_address;
    unsigned long long offset;
    size_t size;
};

struct program_info {
    char* name;
    int fd;
    unsigned int pid;
    struct elf load;
    unsigned long long base_address;
    vector<struct break_point> bp;
    int hit_bp_id;
    int increase_bp_id;
    struct disasm asm_info;
};

extern int state;
extern int len_commands;
extern struct Command commands[];
extern struct program_info program;

#endif