//
// Created by dreamer_he's mac on 10/20/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int grep(char *pattern, FILE *stream) {
    if(stream == NULL) {
        printf("wgrep: cannot open file\n");
        exit(1);
    }

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while((linelen = getline(&line, &linecap, stream)) > 0) {
        char *ptr = strstr(line, pattern);
        if(ptr != NULL) {
            printf("%s", line);
        }
    }
    
    free(line);
    fclose(stream);
    return 0;
}

int main(int argc, char *argv[]) {
    if(argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }

    char* pattern = argv[1];
    if(argc == 2) {
        // TODO
        grep(pattern, stdin);
        return 0;
    }

    for(int i = 2; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        int rcode = grep(pattern, fp);
        if(rcode) {
            exit(1);
        }
    }

    return 0;
}

