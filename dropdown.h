#ifndef DROPDOWN_H
#define DROPDOWN_H

// ANSI codes
#define CLEAR_SCREEN "\033[K"
#define CURSOR_UP "\033[A"
#define CURSOR_DOWN "\033[B"
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"
#define CLEAR_LINE "\033[K"
#define BOLD "\033[1m"
#define RESET "\033[0m"

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_ENTER 10
#define KEY_ESC 27
#define KEY_Q 113

typedef struct {
    char** options;
    int num_options;
    int selected;
} Dropdown;

Dropdown* create_dropdown(char** options, int num_options);
void display_dropdown(Dropdown* dropdown, int first_draw);
int read_key();
void handle_movement(Dropdown* dropdown, int key);
int run_dropdown(Dropdown* dropdown);
void init_terminal();
void reset_terminal();
void free_dropdown(Dropdown* dropdown);

#endif
