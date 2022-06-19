#include "vmmap.h"

int vmmap(int argc, char[][100]) {
    if(state != STATE_RUN) {
        printf("** state must be RUNNING\n");
        return 0;
    }

    char path[100];
    snprintf(path, sizeof(path), "/proc/%d/maps", program.pid);
    FILE* f = fopen(path, "r");
    unsigned long long addr, endaddr, offset, inode;
    char permission[100], device[100], filename[100];
    
    while(true) {
        int ret = fscanf(f, "%llx-%llx %s %llx %s %llx", &addr, &endaddr, permission ,&offset, device, &inode);
        if(ret == 0 || ret == EOF) break;

        // if(inode != 0){
        //     ret += fscanf(f, "%s\n", filename);
        //     if(ret == 0 || ret == EOF) break;
        // }
        // else {

        char buf[255];
        filename[0] = '\0';
        fgets(buf, sizeof(buf), f);
        sscanf(buf, "%s\n", filename);

        printf(VMMAP_FORMAT, addr, endaddr, permission[0], permission[1], permission[2], inode, filename);
    }
    fclose(f);
    
    return 0;

}