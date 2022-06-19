#include "disasm.h"

int disasemble(unsigned char* pos, unsigned long long *address, size_t max_len) {
    csh cshandle;
    cs_insn* insn;
    size_t count;

    if (cs_open(CS_ARCH_X86, CS_MODE_64, &cshandle) != CS_ERR_OK) {
        printf("** cap error: init error \n");
        return 0 ;
    }

    if ((count = cs_disasm(cshandle, pos, 100, *address, 0, &insn)) > 0) {
        for (size_t i = 0; i < count && i < max_len; i++) {
            printf("      %lx: ", insn[i].address);
            
            for (size_t j = 0; j < 12; j++) {
                if (j < insn[i].size)
                    printf("%02hhx ", insn[i].bytes[j]);
                else
                    printf("   ");
            }
            printf("\t%s\t%s\n", insn[i].mnemonic, insn[i].op_str);

            if(insn[i].address >= program.load.virtual_address + program.load.size - 1)  
                return -1;
        }
    } 
    else
       printf("** disasm error: failed to disassemble code \n"); 

    cs_close(&cshandle);
    return count;
}

int disasm(int argc, char argv[][100]) {
    if(argc == 0) {
        printf("** no addr is given\n");
        return 0;
    }

    if(state != STATE_RUN) {
        printf("** state must be RUNNING\n");
        return 0;
    }

    unsigned long long address = strtol(argv[0], NULL, 16);
    if(!(address >= program.load.virtual_address && address < program.load.virtual_address+program.load.size)){
        printf("** the address is out of the range of the text segment\n");
        return 0;
    }

    struct disasm* dmp;
    dmp = &program.asm_info;
    long long offset = program.asm_info.offset + (address - program.asm_info.cur_virtual_address);
    
    unsigned char* pos = (unsigned char*)dmp->data + offset;
    int count = disasemble(pos, &address, DISASM_LEN);

    if(count < 0)
        printf("** the address is out of the range of the text segment\n");

    return 0;
}