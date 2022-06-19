#include "help.h"

int help(int argc, char argv[][100]) {
    
    
    for(int i = 0; i < len_commands; i++) 
        printf("%s\n", commands[i].help_msg);

    return 0;
}