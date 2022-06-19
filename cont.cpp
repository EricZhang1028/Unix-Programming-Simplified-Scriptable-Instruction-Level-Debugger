#include "cont.h"

struct user_regs_struct get_regs_info() {
    struct user_regs_struct result;
    ptrace(PTRACE_GETREGS, program.pid, 0, &result);
    return result;
}

void disasemble_one(unsigned long long address) {
    long long offset = program.asm_info.offset + (address - program.asm_info.cur_virtual_address);
    unsigned char* pos = (unsigned char*)program.asm_info.data + offset;

    csh cshandle;
    cs_insn* insn;

    cs_open(CS_ARCH_X86, CS_MODE_64, &cshandle);
    cs_disasm(cshandle, pos, 100, address, 0, &insn);
        
    printf("\t%lx:\t", insn[0].address);
            
    for (size_t j = 0; j < 12; j++) {
        if (j < insn[0].size) {
            printf("%02hhx ", insn[0].bytes[j]);
        }else{
            printf("   ");
        }
    }

    printf("\t%s\t%s\n", insn[0].mnemonic, insn[0].op_str);

    cs_close(&cshandle);
}

void show_hit_bk_msg(unsigned long long address) {
    printf("** breakpoint @");
    disasemble_one(address);  
}

void restore_bp() {
    for(auto &b: program.bp){
        if(b.id == program.hit_bp_id) {
            change_byte(b.addrress, 0xcc);
            program.hit_bp_id = -1;
            break;
        }
    }
}

void remove_bp() {
    for(auto &b: program.bp){
        if(b.id == program.hit_bp_id) {
            change_byte(b.addrress, b.orig_cmd);
        }
    }
}

int wait_proc_run(){
    int wait_status = 0;
    waitpid(program.pid, &wait_status, 0);
    if(WIFSTOPPED(wait_status)) {
        if(WSTOPSIG(wait_status) != SIGTRAP) {
            printf("** %d stop by signal", program.pid);
            return -1;
        }
        return 1;
    }
    else if(WIFEXITED(wait_status)) {
        if (WIFSIGNALED(wait_status))
            printf("** %d stop by signal", program.pid);
        else
           printf("** chlid process %d terminiated normally (code %d)\n", program.pid, wait_status);
       program.pid = 0;
       state = STATE_LOAD;
       return 0;
    }
    return -1;
}

void check_bp() {
    struct user_regs_struct regs = get_regs_info();
    for(auto &b: program.bp) {
        if(regs.rip == b.addrress) {
            program.hit_bp_id = b.id;
            show_hit_bk_msg(b.addrress);
            return;
        }
    }
    program.hit_bp_id = -1;
}

int cont(int argc, char argv[][100]) {
    if(state != STATE_RUN) {
        printf("** si error: must in running state\n");
        return 0;
    }

    if(program.hit_bp_id != -1){
        remove_bp();
        ptrace(PTRACE_SINGLESTEP, program.pid, 0, 0);
        wait_proc_run();
        restore_bp();
    }
    
    ptrace(PTRACE_CONT, program.pid, 0, 0);
    
    int wait_result = wait_proc_run();
    if(wait_result == 1) {
        struct user_regs_struct regs = get_regs_info();
        regs.rip--;
        ptrace(PTRACE_SETREGS, program.pid, 0, &regs);
        check_bp();
    }

    return 0;
}

int si(int argc, char argv[][100]) {
    if(state != STATE_RUN) {
        printf("** si error: must in running state\n");
        return 0;
    }

    if(program.hit_bp_id != -1)
        remove_bp();

    ptrace(PTRACE_SINGLESTEP, program.pid, 0, 0);

    int wait_result = wait_proc_run();
    if(wait_result == 1) {
        if(program.hit_bp_id != -1)
            restore_bp();
        check_bp();
    }
    
    return 0;
}