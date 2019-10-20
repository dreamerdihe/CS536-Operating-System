//
// Created by dreamer_he's mac on 10/22/19.
//

#include <stdio.h>
#include <stdlib.h>

int wunzip(char *fileName) {
    FILE *fp = fopen(fileName, "r");
    if(fp == NULL) {
        printf("wzip: cannot open file\n");
        exit(1);
    }

    void *buffer = malloc(sizeof(int) + sizeof(char));
    while(fread(buffer, sizeof(int) + sizeof(char), 1, fp) == 1) {
        int *repeat = (int*)buffer;
        char *repeatChar = buffer + sizeof(int);

        for(int i = 0; i < *repeat; i++) {
            printf("%c", repeatChar[0]);
        }
    }

    free(buffer);
    fclose(fp);
    return 0;
}

int main(int argc, char *argv[]) {
    if(argc == 1) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }

    for(int i = 1; i < argc; i++) {
        int rcode = wunzip(argv[i]);
        if(rcode) {
            exit(1);
        }
    }

    return 0;
}





