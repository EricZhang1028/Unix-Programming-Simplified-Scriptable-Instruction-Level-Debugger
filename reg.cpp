#include "reg.h"

int get_regs(int argc, char argv[][100]) {
    if(state != STATE_RUN){
        printf("** state must be RUNNING\n");
        return 0;
    }

    struct user_regs_struct regs;

    if(ptrace(PTRACE_GETREGS, program.pid, 0, &regs ) == 0) {
        printf("%-3s %-15llx", "RAX", regs.rax);
        printf("%-3s %-15llx", "RBX", regs.rbx);
        printf("%-3s %-15llx", "RCX", regs.rcx);
        printf("%-3s %-15llx\n", "RDX", regs.rdx);

        printf("%-3s %-15llx", "R8", regs.r8);
        printf("%-3s %-15llx", "R9", regs.r9);
        printf("%-3s %-15llx", "R10", regs.r10);
        printf("%-3s %-15llx\n", "R11", regs.r11);

        printf("%-3s %-15llx", "R12", regs.r12);
        printf("%-3s %-15llx", "R13", regs.r13);
        printf("%-3s %-15llx", "R14", regs.r14);
        printf("%-3s %-15llx\n", "R15", regs.r15);

        printf("%-3s %-15llx", "RDI", regs.rdi);
        printf("%-3s %-15llx", "RSI", regs.rsi);
        printf("%-3s %-15llx", "RBP", regs.rbp);
        printf("%-3s %-15llx\n", "RSP", regs.rsp);

        printf("%-3s %-15llx", "RIP", regs.rip);
        printf("%-5s %016llx\n", "FLAGS", regs.eflags);
        
    }
    
    return 0;
}

int get_reg(int argc, char argv[][100]) {
    if(argc == 0) {
        printf("** no register is given\n");
        return 0;
    }

    if(state != STATE_RUN) {
        printf("** state must be RUNNING\n");
        return 0;
    }

    char* reg_name = argv[0];
    struct user_regs_struct regs;

    if(ptrace(PTRACE_GETREGS, program.pid, 0, &regs ) == 0) {
        if(strcmp("rax", reg_name) == 0)
            printf("rax = %lld (0x%llx)\n", regs.rax, regs.rax);
        else if(strcmp("rbx", reg_name) == 0)
            printf("rbx = %lld (0x%llx)\n", regs.rbx, regs.rbx);
        else if(strcmp("rcx", reg_name) == 0)
            printf("rcx = %lld (0x%llx)\n", regs.rcx, regs.rcx);
        else if(strcmp("rdx", reg_name) == 0)
            printf("rdx = %lld (0x%llx)\n", regs.rdx, regs.rdx);
        else if(strcmp("r8", reg_name) == 0)
            printf("r8 = %lld (0x%llx)\n", regs.r8, regs.r8);
        else if(strcmp("r9", reg_name) == 0)
            printf("r9 = %lld (0x%llx)\n", regs.r9, regs.r9);
        else if(strcmp("r10", reg_name) == 0)
            printf("r10 = %lld (0x%llx)\n", regs.r10, regs.r10);
        else if(strcmp("r11", reg_name) == 0)
            printf("r11 = %lld (0x%llx)\n", regs.r11, regs.r11);
        else if(strcmp("r12", reg_name) == 0)
            printf("r12 = %lld (0x%llx)\n", regs.r12, regs.r12);
        else if(strcmp("r13", reg_name) == 0)
            printf("r13 = %lld (0x%llx)\n", regs.r13, regs.r13);
        else if(strcmp("r14", reg_name) == 0)
            printf("r14 = %lld (0x%llx)\n", regs.r14, regs.r14);
        else if(strcmp("r15", reg_name) == 0)
            printf("r15 = %lld (0x%llx)\n", regs.r15, regs.r15);
        else if(strcmp("rdi", reg_name) == 0)
            printf("rdi = %lld (0x%llx)\n", regs.rdi, regs.rdi);
        else if(strcmp("rsi", reg_name) == 0)
            printf("rsi = %lld (0x%llx)\n", regs.rsi, regs.rsi);
        else if(strcmp("rbp", reg_name) == 0)
            printf("rbp = %lld (0x%llx)\n", regs.rbp, regs.rbp);
        else if(strcmp("rsp", reg_name) == 0)
            printf("rsp = %lld (0x%llx)\n", regs.rsp, regs.rsp);
        else if(strcmp("rip", reg_name) == 0)
            printf("rip = %lld (0x%llx)\n", regs.rip, regs.rip);
        else if (strcmp("flags", reg_name) == 0)
            printf("flags = %lld (0x%llx)\n", regs.eflags , regs.eflags);
        else
            printf("** register not found\n");
    }

    return 0;
}

int set_reg(int argc, char argv[][100]) {
    if(state != STATE_RUN) {
        printf("** state must be RUNNING\n");
        return 0;
    }

    if(argc < 2) {
        printf("** Not enough input arguments\n");
        return 0;
    }

    struct user_regs_struct regs;
    char* reg_name = argv[0];
    char* reg_val = argv[1];

    if(ptrace(PTRACE_GETREGS, program.pid, 0, &regs ) == 0) {

        if(strcmp("rip", reg_name) == 0 && regs.rip != (unsigned long long)strtol(reg_val, NULL, 16)){
            program.hit_bp_id = -1;
        }

        if(strcmp("rax", reg_name) == 0)
            regs.rax = strtol(reg_val, NULL, 16);    
        else if(strcmp("rbx", reg_name) == 0)
            regs.rbx = strtol(reg_val, NULL, 16);
        else if(strcmp("rcx", reg_name) == 0)
            regs.rcx = strtol(reg_val, NULL, 16);
        else if(strcmp("rdx", reg_name) == 0)
            regs.rdx = strtol(reg_val, NULL, 16);
        else if(strcmp("r8", reg_name) == 0)
            regs.r8 = strtol(reg_val, NULL, 16);
        else if(strcmp("r9", reg_name) == 0)
            regs.r9 = strtol(reg_val, NULL, 16);
        else if(strcmp("r10", reg_name) == 0)
            regs.r10 = strtol(reg_val, NULL, 16);
        else if(strcmp("r11", reg_name) == 0)
            regs.r11 = strtol(reg_val, NULL, 16);
        else if(strcmp("r12", reg_name) == 0)
            regs.r12 = strtol(reg_val, NULL, 16);
        else if(strcmp("r13", reg_name) == 0)
            regs.r13 = strtol(reg_val, NULL, 16);
        else if(strcmp("r14", reg_name) == 0)
            regs.r14 = strtol(reg_val, NULL, 16);
        else if(strcmp("r15", reg_name) == 0)
            regs.r15 = strtol(reg_val, NULL, 16);
        else if(strcmp("rdi", reg_name) == 0)
            regs.rdi = strtol(reg_val, NULL, 16);
        else if(strcmp("rsi", reg_name) == 0)
            regs.rsi = strtol(reg_val, NULL, 16);
        else if(strcmp("rbp", reg_name) == 0)
            regs.rbp = strtol(reg_val, NULL, 16);
        else if(strcmp("rsp", reg_name) == 0)
            regs.rsp = strtol(reg_val, NULL, 16);
        else if(strcmp("rip", reg_name) == 0)
            regs.rip = strtol(reg_val, NULL, 16);
        else if (strcmp("flags", reg_name) == 0)
            regs.eflags = strtol(reg_val, NULL, 16);
        else
            printf("** register not found\n");

        ptrace(PTRACE_SETREGS, program.pid, 0, &regs);
    }

    return 0;
}