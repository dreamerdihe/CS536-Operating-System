//
// Created by dreamer_he's mac on 10/22/19.
//

#include <stdio.h>
#include <stdlib.h>

int zipone(int repeat, char *c) {
    int n_write = fwrite(&repeat, sizeof(int), 1, stdout);
    if(n_write < 1) {
        exit(1);
    }

    // write the character
    n_write = fwrite(c, sizeof(char), 1, stdout);
    if(n_write < 1) {
        exit(1);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if(argc == 1) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    int repeat;
    char repeatChar = '#';
    char *line = NULL;
    size_t linecap;
    ssize_t linelen;

    for(int i = 1; i < argc; i++) { // traverse the files
        FILE *fp = fopen(argv[i], "r");
        if(fp == NULL) {
            printf("wzip: cannot open file\n");
            exit(1);
        }

        while((linelen = getline(&line, &linecap, fp)) > 0) { // traverse the lines in a file
            // traverse the characters in this line
            for(int i = 0; i < linelen; i++) { // traverse the chars in the line
                if(repeatChar == '#') {
                    repeatChar = line[i];
                    repeat = 1;
                } else if(line[i] == repeatChar) { // a repeat char appears;
                    repeat++;
                } else {
                    // write the repeat number
                    int rcode = zipone(repeat, &repeatChar);
                    if(rcode) {
                        exit(1);
                    }
                    // reset the repeat when reach the end of a repeat occurrence
                    repeatChar = line[i];
                    repeat = 1;
                }
            }
        }

        fclose(fp);
    }

    int rcode = zipone(repeat, &repeatChar);
    if(rcode) {
        exit(1);
    }

    return 0;
}

