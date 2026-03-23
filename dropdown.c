#include "dropdown.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

Dropdown* create_dropdown(char** options, int num_options) {
    Dropdown* dropdown = (Dropdown*)malloc(sizeof(Dropdown));
    if (!dropdown) {
        return NULL;
    }

    dropdown->options = options;
    dropdown->num_options = num_options;
    dropdown->selected = 0;
    return dropdown;
}

void display_dropdown(Dropdown* dropdown, int first_draw) {
    if (!first_draw) {
        int lines_to_move = dropdown->num_options;
        printf("\033[%dA", lines_to_move);
    }

    printf("\r" CLEAR_LINE);
    for (int i = 0; i < dropdown->num_options; i++) {
        printf("\r" CLEAR_LINE);
        if (i == dropdown->selected) {
            printf("%s> %s%s\n", BOLD, dropdown->options[i], RESET);
        } else {
            printf("  %s\n", dropdown->options[i]);
        }
    }
}

int read_key() {
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
        return c;
    }
    return 0;
}

void handle_movement(Dropdown* dropdown, int key) {
    switch (key) {
    case 'k':
    case KEY_UP:
        if (dropdown->selected > 0) {
            dropdown->selected--;
        } else {
            dropdown->selected = dropdown->num_options - 1;
        }
        break;

    case 'j':
    case KEY_DOWN:
        if (dropdown->selected < dropdown->num_options - 1) {
            dropdown->selected++;
        } else {
            dropdown->selected = 0;
        }
        break;
    }

    display_dropdown(dropdown, 0);
}

int run_dropdown(Dropdown* dropdown) {
    init_terminal();

    int is_running = 1;
    int first_draw = 1;
    while (is_running) {
        display_dropdown(dropdown, first_draw);
        first_draw = 0;
        int key = read_key();

        switch (key) {
        case KEY_ESC:
            if (read_key() == '[') {
                handle_movement(dropdown, read_key());
            } else {
                dropdown->selected = -1;
                is_running = 0;
            }
            break;

        case 'j':
        case 'k':
            handle_movement(dropdown, key);
            break;

        case ' ':
        case KEY_ENTER:
            is_running = 0;
            break;

        case KEY_Q:
            dropdown->selected = -1;
            is_running = 0;
            break;
        }
    }

    reset_terminal();
    return dropdown->selected;
}

void init_terminal() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    printf(HIDE_CURSOR);
}

void reset_terminal() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    printf(SHOW_CURSOR);
}

void free_dropdown(Dropdown* dropdown) { free(dropdown); }
