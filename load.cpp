#include "load.h"

unsigned long long get_value(unsigned long long offset, unsigned long long size, unsigned char* elf) {
    unsigned long long ret = 0;
    for(int i = offset+size-1; i >= int(offset); i--) {
        ret = ret * 16 * 16 + elf[i]; // ret is 64bit = 0x...len(16)... , elf[i] is 1 bytes = len(2) in hex
    }

    return ret;
}

int parse_elf(char* path) {
    int fd = open(path, O_RDONLY);
    if(fd == -1) { 
        perror("fopen");
        return -1;
    }

    // elf header is 64 bytes, so declare array "elf" of 0x40 length
    unsigned char elf[0x40];
    // read elf header to array
    read(fd, elf, 0x40); 

    // find entry point from elf (.text start position)
    unsigned long long address = get_value(ELF_ENTRY_POINT_OFFSET, ELF_ENTRY_POINT_SIZE, elf);
    program.load.entry_point = address;
    program.load.virtual_address = address;

    // in order to find the .text section size, we need to:
    // find sec_h offset(first entry) -> each entry size 
    // -> the number of entry
    unsigned long long sh_addr = get_value(ELF_SH_OFFSET, ELF_SH_OFFET_SIZE, elf);
    unsigned long long sh_size = get_value(ELF_SH_SIZE_OFFSET, ELF_SH_SIZE_SIZE, elf);
    unsigned long long sh_num = get_value(ELF_SH_NUM_OFFSET, ELF_SH_NUM_SIZE, elf);
    // there is a entry(index) in sec_h table which record the position of sec_h name table
    unsigned long long sh_str = get_value(ELF_SHSTRNDX_OFFSET, ELF_SHSTRNDX_SIZE, elf);

    // read sec_h table to "shs" array
    unsigned char shs[sh_size*sh_num];
    unsigned char* shs_ptr = shs;
    lseek(fd, sh_addr, SEEK_SET);
    read(fd, shs, sh_size*sh_num);
    
    // find the addr and offset of section name table in virtual memory from sec_h table entry
    unsigned char* str_ptr = sh_str * sh_size + shs;
    unsigned long long sh_str_size = get_value(SH_SIZE_OFFSET, SH_SIZE_SIZE, str_ptr);
    unsigned long long sh_str_offset = get_value(SH_OFFSET_OFFSET, SH_OFFSET_SIZE, str_ptr);

    // read all of section name table to "name" array
    char names[sh_str_size];
    lseek(fd, sh_str_offset, SEEK_SET);
    read(fd, names, sh_str_size);

    for(unsigned long long i = 0; i < sh_num; i++) {
        // find section_name, offset, addr and size in each sec_h table entry
        unsigned long s_name_idx = get_value(SH_NAME_OFFSET, SH_NAME_SIZE, shs_ptr);
        unsigned long long s_addr = get_value(SH_ADDR_OFFSET, SH_ADDR_SIZE, shs_ptr);
        unsigned long long s_offset = get_value(SH_OFFSET_OFFSET, SH_OFFSET_SIZE, shs_ptr);
        unsigned long long s_size = get_value(SH_SIZE_OFFSET, SH_SIZE_SIZE, shs_ptr);
        
        // find the ".text" section to calculate ".text" section size
        // names+s_name_idx is to find the real string in names(table)
        if(strncmp(".text", names+s_name_idx, 5) == 0){
            program.load.size = s_size;
            program.load.offset = s_offset;

            program.asm_info.cur_virtual_address = s_addr;
            program.asm_info.size = s_size;
            program.asm_info.offset = 0;
            lseek(fd, s_offset, SEEK_SET);
            program.asm_info.data = (char*)malloc(s_size);
            read(fd, program.asm_info.data, s_size);
        }
        shs_ptr += sh_size;
    }
    
    return 0;
}

int load_program(int argc, char argv[][100]) {
    char* path = argv[0];
    if(state != STATE_NONE) {
        printf("** state must be NOT LOADED\n");
        return 0;
    }

    if(parse_elf(path) != 0) {
        // printf("** load program error: parse error\n");
        return -1;
    }

    state = STATE_LOAD;
    program.name = strdup(argv[0]);
    printf("** program '%s' loaded. entry point 0x%-6llx\n", program.name, program.load.entry_point);

    return 0;
}