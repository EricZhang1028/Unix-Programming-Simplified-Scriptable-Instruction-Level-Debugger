#include "start.h"

int start(int argc, char argv[][100]) {
    if(state == STATE_RUN) {
        printf("** program is already running\n");
        return 0;
    }

    if(state != STATE_LOAD){
        printf("** state must be LOADED\n");
        return 0;
    }

    if((program.pid = fork()) < 0){
        printf("** start fork error\n");
        return 0;
    }
    else if(program.pid == 0) {
        if(ptrace(PTRACE_TRACEME, 0, 0, 0) < 0)
            printf("** start ptrace@traceme error\n");

        char* no_args[] = {program.name, NULL};
        if(execvp(program.name, no_args) < 0){
            printf("** start execvp error\n");
            return 0;
        }
    }
    else {
        int wait_status = 0;
        waitpid(program.pid, &wait_status, 0);
        if(WIFSTOPPED(wait_status)) {
            ptrace(PTRACE_SETOPTIONS, program.pid, 0, PTRACE_O_EXITKILL);
            state = STATE_RUN;
            program.base_address = 0;

            program.hit_bp_id = -1;
            for(auto &x : program.bp) {
                unsigned long long tmp = x.addrress;
                unsigned char cmd = change_byte(tmp, 0xcc);
                x.orig_cmd = cmd;
            }

            printf("** pid %d\n", program.pid);
        }
        else
            printf("** start WIFSTOPPED error\n");
    }

    return 0;

}

int quit(int argc, char argv[][100]) {
    if(program.pid != 0) {
        kill(program.pid, SIGTERM);
    }
    return 2;
}