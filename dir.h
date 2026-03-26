#ifndef DIR_H
#define DIR_H

#include "array.h"

typedef struct {
    char* note;
    char* command;
} Comm;

typedef struct {
    char* path;
    char* note;

    Comm** commands;
    int capacity;
    int size;
} Dir;

typedef struct {
    Dir** dirs;
    int capacity;
    int size;

} DirArr;

// dir
int init_dir(Dir* dir, const char* path, char* note);
int push_command_dir(Dir* dir, const char* comm, char* note);
int pop_command_dir(Dir* dir, const char* comm);
void free_dir(Dir* dir);

// dir array
int init_dir_arr(DirArr* data);
int push_dir_arr(DirArr* data, const char* path, char* note);
int pop_dir_arr(DirArr* data, const char* path);
Dir* get_dir(DirArr* data, const char* path);
void free_dir_arr(DirArr* data);

#endif
