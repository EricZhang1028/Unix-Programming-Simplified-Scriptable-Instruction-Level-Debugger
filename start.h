#ifndef _START_H
#define _START_H

#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libmain.h"

int start(int, char [][100]);
int quit(int, char[][100]);

#endif