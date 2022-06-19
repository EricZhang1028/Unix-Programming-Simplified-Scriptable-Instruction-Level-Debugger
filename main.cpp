#include "libmain.h"
#include "errno.h"

int state = STATE_NONE;
struct program_info program;

void init_program() {
    // program.name = "";
    program.pid = 0;
    program.hit_bp_id = -1;
    program.increase_bp_id = 0;
}

int main(int argc, char* argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    init_program();

    int script_flag = 0;
    char* script_path;
    FILE* input = stdin;

    if(argc > 1) {
        if(argc == 2) {
            char program_path[100][100];
            snprintf(program_path[0], sizeof(program_path), "%s", argv[1]);
            if(load_program(1, program_path) != 0) return 0;
        }
        else if(argc == 3 || argc == 4){
            char c;
            while((c = getopt(argc, argv, "-s:")) != -1) {
                switch(c){
                    case 's':
                        script_path = optarg;
                        script_flag = 1;
                        if((input = fopen(script_path, "r")) == NULL) {
                            perror(script_path);
                            return 0;
                        }
                        break;
                    default:
                        char* tmp = optarg;
                        char program_path[100][100];
                        snprintf(program_path[0], sizeof(program_path), "%s", tmp);
                        if(load_program(1, program_path) != 0) return 0;
                        break;
                }
            }
        }
        else{
            printf("usage: ./hw4 [-s script] [program]\n");
        }
    }

    while(true) {
        if(script_flag == 0)
            printf("sdb> ");

        char* command = NULL;
        size_t command_size = 0;
        
        if(getline(&command, &command_size, input) != -1) {
            if(strcmp(command, "\n") == 0) continue;
            // printf("I got: %s\n",command);
            
            int ret;
            if((ret = run_command(command)) < 0)
                printf("** run command error\n");
            else if(ret == 2) // exit
                return 0;
        }
        else
            return -1;
    }
}