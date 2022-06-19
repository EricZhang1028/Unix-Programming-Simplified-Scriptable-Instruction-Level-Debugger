#include "dump.h"

int dump(int argc, char argv[][100]) {
    if(state != STATE_RUN) {
        printf("** state must be RUNNING\n");
        return 0;
    }

    if(argc == 0) {
        printf("** no addr is given\n");
        return 0;
    }

    unsigned long long address = strtol(argv[0], NULL, 16);
    unsigned char data[80];
    unsigned char* data_ptr = data;
    for(int i = 0; i < 5; i++) {
        printf("      %llx: ", address);
        for(int k = 0; k < 16; k++) {
            *data_ptr = ptrace(PTRACE_PEEKDATA, program.pid, address, 0);
            address++;
            data_ptr++;
            printf("%02x ", data[16 * i + k]);
        }
        printf(" |");

        for(int k = 0; k < 16; k++) {
            char c = data[16 * i + k];
            if(isprint(c) != 0)
                printf("%c", c);
            else
                printf(".");
        }
        
        printf("|\n");
    }

    return 0;
}