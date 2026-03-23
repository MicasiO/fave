#ifndef UTILS_H
#define UTILS_H

#include "dir.h"
#include "dropdown.h"

// utils
const char* get_data_path();
int deserialize(DirArr* data);
int serialize(DirArr* data);
void handle_sigint(int sig);

#endif
