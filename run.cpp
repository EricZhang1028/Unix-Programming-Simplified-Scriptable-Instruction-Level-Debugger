#include "run.h"

int run(int argc, char argv[][100]) {
    char t[][100] = {'\0'};
    if(state == STATE_RUN){
        printf("** program %s is already running\n", program.name);
        cont(0, t);
    }
    else if(state == STATE_LOAD) {
        start(0, t);
        for(auto &b: program.bp)
            change_byte(b.addrress, 0xcc);
        cont(0, t);
    }
    else
        printf("** state must be LOADED or RUNNING\n");

    return 0;
}