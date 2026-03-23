#ifndef DIR_H
#define DIR_H

#include "array.h"

typedef struct {
    char* title;
    StrArr commands;
} Dir;

typedef struct {
    Dir** dirs;
    int capacity;
    int size;

} DirArr;

// dir
int init_dir(Dir* dir, const char* title);
int push_command_dir(Dir* dir, const char* comm);
int pop_command_dir(Dir* dir, const char* comm);
void free_dir(Dir* dir);

// dir array
int init_dir_arr(DirArr* data);
int push_dir_arr(DirArr* data, const char* title);
Dir* get_dir(DirArr* data, const char* title);
void free_dir_arr(DirArr* data);

#endif
