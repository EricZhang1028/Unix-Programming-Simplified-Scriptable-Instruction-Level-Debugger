#include "run_cmd.h"

struct Command commands[] = {
    { (char*)"break", (char*)"b", (char*)"- break {instruction-address}: add a break point", brk },
    { (char*)"cont", (char*)"c", (char*)"- cont: continue execution",  cont},
    { (char*)"delete", NULL, (char*)"- delete {break-point-id}: remove a break point",  del},
    { (char*)"disasm", (char*)"d", (char*)"- disasm addr: disassemble instructions in a file or a memory region", disasm },
    { (char*)"dump", (char*)"x", (char*)"- dump addr: dump memory content", dump },
    { (char*)"exit", (char*)"q", (char*)"- exit: terminate the debugger",  quit},
    { (char*)"get", (char*)"g", (char*)"- get reg: get a single value from a register",  get_reg},
    { (char*)"getregs", NULL, (char*)"- getregs: show registers",  get_regs},
    { (char*)"help", (char*)"h", (char*)"- help: show this message", help },
    { (char*)"list", (char*)"l", (char*)"- list: list break points", list },
    { (char*)"load", NULL, (char*)"- load {path/to/a/program}: load a program", load_program },
    { (char*)"run", (char*)"r", (char*)"- run: run the program", run },
    { (char*)"vmmap", (char*)"m", (char*)"- vmmap: show memory layout", vmmap },
    { (char*)"set", (char*)"s", (char*)"- set reg val: get a single value to a register", set_reg },
    { (char*)"si", NULL, (char*)"- si: step into instruction", si },
    { (char*)"start", NULL, (char*)"- start: start the program and stop at the first instruction", start },
};

int len_commands = sizeof(commands)/sizeof(struct Command);

char* trim (char *s)
{
    int i;

    while (isspace (*s)) s++;   // skip left side white spaces
    for (i = strlen (s) - 1; (isspace (s[i])); i--) ;   // skip right side white spaces
    s[i + 1] = '\0';
    return s;
}

int parse_command(char* command, char* cmd, char argv[][100], int* argc){
    command = trim(command);
    
    int flag = 0; // for first command
    int s_flag = 0;
    int index = 0;

    for(size_t i = 0; i < strlen(command); i++) {

        if(command[i] == '\n' || command[i] == '\0') continue;

        if(command[i] == ' ' && s_flag == 1) continue;
        
        if(command[i] == ' ' && s_flag == 0){
            if(flag == 0){
                flag = 1;
                cmd[index] = '\0';
            }
            else {
                argv[(*argc)++][index] = '\0';
            }
            index = 0;
            s_flag = 1;
            continue;
        }

        if(flag == 0) {
            cmd[index++] = command[i];
        }
        else{
            argv[*argc][index++] = command[i];
        }
        s_flag = 0;
    }

    if(flag == 0)
        cmd[index] = '\0';
    else
        argv[(*argc)++][index] = '\0';

    return 0;
}

fun find_function(char* cmd) {
    for(size_t i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        if(strcmp(cmd, commands[i].name) == 0){
            return commands[i].cmd_fun;
        }
        if(commands[i].shortcut != NULL && strcmp(cmd, commands[i].shortcut) == 0)
            return commands[i].cmd_fun;
    }

    return NULL;
}

int run_command(char* command) {
    char cmd[100];
    char argv[100][100];
    int argc = 0;

    parse_command(command, cmd, argv, &argc);
    fun cmd_fun = find_function(cmd);
    if(cmd_fun == NULL) {
        printf("** command not found \n");
        return 0;
    }
    else {
        return cmd_fun(argc, argv);
    }
    



    return 0;
}