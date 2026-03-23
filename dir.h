#include "array.h"

typedef struct {
    char* title;
    str_arr commands;
} dir;

typedef struct {
    dir** dirs;
    int capacity;
    int size;

} dir_arr;

// dir
int init_dir(dir* dir, const char* title);
int push_command_dir(dir* dir, const char* comm);
int pop_command_dir(dir* dir, const char* comm);
void free_dir(dir* dir);

// dir array
int init_dir_arr(dir_arr* data);
int push_dir_arr(dir_arr* data, const char* title);
dir* get_dir(dir_arr* data, const char* title);
void free_dir_arr(dir_arr* data);

// utils
const char* get_data_path();
int deserialize(dir_arr* data);
int serialize(dir_arr* data);
