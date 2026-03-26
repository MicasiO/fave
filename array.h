#ifndef ARRAY_H
#define ARRAY_H

typedef struct {
    char** items;
    int capacity;
    int size;
} StrArr;

// string array
int init_str_arr(StrArr* arr);
int push_str_arr(StrArr* arr, const char* str);
int pop_str_arr(StrArr* arr, const char* str);
void split_str(StrArr* arr, const char* str, const char* delim);
void free_str_arr(StrArr* arr);

// init_comm_arr()

#endif
