#ifndef _LOAD_H
#define _LOAD_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>

#include "libmain.h"

#define ELF_ENTRY_POINT_OFFSET 0x18
#define ELF_ENTRY_POINT_SIZE 0x08

#define ELF_SH_OFFSET 0x28
#define ELF_SH_OFFET_SIZE 0x08
#define ELF_SH_SIZE_OFFSET 0x3a
#define ELF_SH_SIZE_SIZE 0x02
#define ELF_SH_NUM_OFFSET 0x3c
#define ELF_SH_NUM_SIZE 0x02
#define ELF_SHSTRNDX_OFFSET 0x3E
#define ELF_SHSTRNDX_SIZE 0x02

#define SH_NAME_OFFSET 0x00
#define SH_NAME_SIZE 0X04
#define SH_ADDR_OFFSET 0x10
#define SH_ADDR_SIZE 0x08
#define SH_OFFSET_OFFSET 0x18
#define SH_OFFSET_SIZE 0x08
#define SH_SIZE_OFFSET 0x20
#define SH_SIZE_SIZE 0x08



int load_program(int, char [][100]);

#endif