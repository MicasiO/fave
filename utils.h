#ifndef UTILS_H
#define UTILS_H

#include "array.h"
#include "dir.h"
#include "dropdown.h"

// ANSI codes
#define CLEAR_SCREEN "\033[K"
#define CURSOR_UP "\033[A"
#define CURSOR_DOWN "\033[B"
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"
#define ITALIC "\e[3m"
#define CLEAR_LINE "\033[K"
#define BOLD "\033[1m"
#define RESET "\033[0m"

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_ENTER 10
#define KEY_ESC 27
#define KEY_Q 113

// utils
const char* get_data_path();
const char* get_current_path();
int deserialize(DirArr* data);
int serialize(DirArr* data);
void handle_sigint(int sig);
void run_command(const char* cmd);

#endif
