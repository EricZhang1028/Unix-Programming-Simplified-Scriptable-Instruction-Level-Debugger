#include "break.h"

// chagne the address first byte to targe, and return original byte
unsigned char change_byte(unsigned long long addr, unsigned char b) {
    auto code = ptrace(PTRACE_PEEKTEXT, program.pid, addr, 0);
    ptrace(PTRACE_POKETEXT, program.pid, addr, (code & 0xffffffffffffff00) | b);
    return code & 0xff;
}

// check access address in the .text section
int address_in_text(unsigned long long address) {
    if(address > program.load.virtual_address && address < (program.load.virtual_address+program.load.size))
        return 1;
    else
        return -1;
}

int brk(int argc, char argv[][100]) {
    if(argc == 0) {
        printf("** no address is given\n");
        return 0;
    }

    if(state != STATE_RUN){
        printf("** state must be RUNNING\n");
        return 0;
    }
    
    unsigned long long address = strtol(argv[0], NULL, 16);
    
    if(address_in_text(address) < 0) {
        printf("** the address is out of the range of the text segment\n");
    }
    else {
        // check bp exist
        int idx = 0;
        for(auto &b: program.bp){
            if(b.addrress == address){
                printf("** the breakpoint is already exists. (breakpoint %d)\n", idx);
                return 0;
            }
            idx++;
        }

        unsigned char t = 0xcc;
        unsigned char tmp = change_byte(address, t);
        program.bp.push_back({ program.increase_bp_id++, address, tmp });

         // check bp is not rip
        struct user_regs_struct regs = get_regs_info();
        if(regs.rip == address) {
            for(auto &b: program.bp){
                if(b.addrress == address){
                    program.hit_bp_id = b.id;
                }
            }
        }
    }

    return 0;
}

int list(int argc, char argv[][100]) {
    int count = 0;
    for(auto &b: program.bp){
        printf("%3d: %llx\n", count++, b.addrress);
    }

    return 0;
}

int del(int argc, char argv[][100]) {
    if(argc == 0){
        printf("** no break-point-id is given\n");
        return 0;
    }

    int idx = strtol(argv[0], NULL, 10);
    int cur = 0;
    vector<break_point>::iterator itr;
    for(itr = program.bp.begin(); itr != program.bp.end(); ++itr) {
        if(cur == idx) {
            change_byte((*itr).addrress, (*itr).orig_cmd);
            program.bp.erase(itr);
            printf("** breakpoint %d deleted.\n", cur);

            return 0;
        }
        cur++;
    }
    printf("** breakpoint %d does not exist\n", idx);

    return 0;
}