//
// Created by dreamer_he's mac on 10/20/19.
//
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE (512)

int wcat(char *fileName) {
    FILE* fp = fopen(fileName, "r");
    if(fp == NULL) {
        printf("wcat: cannot open file\n");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    while(fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        printf("%s", buffer);
    }

    fclose(fp);
    return 0;
}

int main(int argc, char *argv[]) {
    for(int i = 1; i < argc; i++) {
        char *fileName = argv[i];
        int rcode = wcat(fileName);
        if(rcode) {
            exit(1);
        }
    }

    return 0;
}





