//
// Created by dreamer_he's mac on 10/30/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include "wish.h"

#ifndef errno 
extern int errno; 
#endif

int main(int argc, char *argv[]) {
    // Some static data
    char **paths = (char *[]){"/bin", };

    // declaration of some neccessary data
    int npath = 1;
    char ***pathsPtr = &paths;
    FILE *input = NULL;
    int is_interactive;
    
    if (argc == 1) {// interactive mode
        // printf("Welcome to Wish interactive shell\n");
        input = stdin;  
        is_interactive = 1;
    } else if(argc == 2) { // batch mode
        // printf("You are using wish shell batch mode\n");
        input = fopen(argv[1], "r");
        if(input == NULL) {
            fprintf(stderr, "wish: %s\n", strerror(errno));
            exit(1);
        }
        is_interactive = 0;
    } else { // error
        printf("wish usage: \n");
        printf("\t wish [filename]\n");
        exit(1);
    }

    char *line = NULL;
    size_t linecap = 0;
    while(1) {
        // print prompt
        if(is_interactive) {
            printf("wish$ ");
        }
        
        // get the user input and trim the input
        char *cmd_argv[20];
        char *cmd_path = NULL;
        int cmd_argc = 0;
        getline(&line, &linecap, input);
        get_argv(line, &cmd_argc, &cmd_path, cmd_argv);

        // built-in function
        if(!strcmp(cmd_path, "exit")) {
            // printf("called built-in function [exit]\n");
            if(cmd_argc == 1) {
                // printf("Bye!\n");
                exit(0);
            } else {
                fprintf(stderr, "exit: should pass no parameters\n");
                continue;
            }
        } else if(!strcmp(cmd_path, "cd")) {
            // printf("called built-in function [cd]\n");
            if(cmd_argc == 2) {
                int rc = chdir(cmd_argv[1]);
                if(rc) {
                    fprintf(stderr, "cd: %s\n", strerror(errno));
                } 
            } else {
                fprintf(stderr, "An error has occurred\n");
            }       
            continue;
        } else if(!strcmp(cmd_path, "path")) {
            // printf("called built-in function [path]\n");
            npath = cmd_argc - 1;
            update_paths(pathsPtr, cmd_argv, npath);
            continue;
        } else if(!strcmp(cmd_path, "$path")) {
            // printf("called built-in function [$path]\n");
            get_paths(pathsPtr, npath);
            continue;
        } else {
            // run the command
            int rc = fork();
            if(rc < 0) { // fork fails
                fprintf(stderr, "fork failed\n");
                exit(1);
            } else if(rc == 0) { // child process
                for(int i = 0; i < npath; i++) {
                    char *path = *pathsPtr[i];
                    char *cmd_full_path = malloc((strlen(*pathsPtr[i]) + strlen(cmd_path) + 10) * sizeof(char));
                    strcpy(cmd_full_path, path);
                    if(cmd_full_path[strlen(cmd_full_path) - 1] != '/') {
                        (void)strcat(cmd_full_path, "/");
                    }

                    (void)strcat(cmd_full_path, cmd_path);
                    // printf("try to run: %s\n", cmd_full_path);
                    if(access(cmd_full_path, X_OK) == 0) { // find the executable
                        execv(cmd_full_path, cmd_argv);
                    }

                    free(cmd_full_path);
                }

                fprintf(stderr, "%s: %s\n", cmd_path, strerror(errno));
                exit(1);
            } else { // parent
                int rc_wait = wait(NULL);
                if(rc_wait == -1) {
                    fprintf(stderr, "%s\n", strerror(errno));
                }
            }
        }
    }
}

// =================== helper functions ======================
void get_argv(char* line, int* cmd_argc, char** cmd_path, char* cmd_argv[]) {
    // delete the last new line symbol
    if(line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
    }

    // start to parse the input command
    char **ap;
    for (ap = cmd_argv; (*ap = strsep(&line, " \t")) != NULL;) {
        if (**ap != '\0') {
            if (++ap >= &cmd_argv[20])
                break;
        }

        if(strlen(cmd_argv[*cmd_argc]) > 0)
            (*cmd_argc)++;
    }

    cmd_argv[*cmd_argc] = NULL;
    *cmd_path = cmd_argv[0];
    return;
}

void update_paths(char ***pathsPtr, char **cmd_argv, int npath) {
    *pathsPtr = malloc(npath * sizeof(char*));
    for(int i = 0; i < npath; i++) {
        (*pathsPtr)[i] = malloc((strlen(cmd_argv[i + 1]) + 10) * sizeof(char));
        strcpy((*pathsPtr)[i], cmd_argv[i + 1]);
    }
}

void get_paths(char ***pathsPtr, int npath) {
    printf("PATH: ");
    for(int i = 0; i < npath - 1; i++) {
        printf("%s, ", (*pathsPtr)[i]);
    }
    printf("%s\n", (*pathsPtr)[npath - 1]);
}