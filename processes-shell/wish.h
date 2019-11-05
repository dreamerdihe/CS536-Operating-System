//
// Created by dreamer_he's mac on 10/30/19.
//

#ifndef CS536_OPERATING_SYSTEM_WISH_H
#define CS536_OPERATING_SYSTEM_WISH_H

// number of elements in fixed-size array
#define LEN(x) (sizeof(x) / sizeof(x[0]))

void trim(char*);
void get_argv(char*, int*, char**, char*[]);
void update_paths(char***, char**, int);
void get_paths(char***, int);

#endif //CS536_OPERATING_SYSTEM_WISH_H
