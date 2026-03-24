#include "action.h"
#include "array.h"
#include "dir.h"
#include "dropdown.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle_show_dirs() {
    DirArr data;
    init_dir_arr(&data);
    deserialize(&data);

    if (data.size == 0) {
        fprintf(stderr, "No faved directories\n");
        return;
    }

    fprintf(stderr, BOLD "Faved directories:\n" RESET);

    for (int i = 0; i < data.size; i++) {
        fprintf(stderr, "%d) %s\n", i + 1, data.dirs[i]->title);
    }

    fprintf(stderr, BOLD "\nSelect number: " RESET);
    char input[256];
    int number;

    if (fgets(input, sizeof(input), stdin)) {
        if (sscanf(input, "%d", &number) == 1) {
            if (number - 1 >= 0 && number - 1 < data.size) {
                fprintf(stdout, "cd '%s'", data.dirs[number - 1]->title);
                free_dir_arr(&data);
                return;
            }
        }
    }

    fprintf(stderr, "Invalid input\n");
    free_dir_arr(&data);
}

void handle_show_comms() {
    const char* path = get_current_path();

    DirArr data;
    init_dir_arr(&data);
    deserialize(&data);

    Dir* dir = get_dir(&data, path);

    if (dir == NULL) {
        fprintf(stderr, "This directory is not faved\n");
        return;
    }

    if (dir->commands.size == 0) {
        fprintf(stderr, "This directory has no faved commands\n");
        return;
    }

    printf("Faved commands:\n");

    StrArr options;
    init_str_arr(&options);

    for (int i = 0; i < dir->commands.size; i++) {
        push_str_arr(&options, dir->commands.items[i]);
    }

    Dropdown* dropdown = create_dropdown(options.items, options.size);

    int selected = run_dropdown(dropdown);

    printf(CLEAR_SCREEN);
    if (selected >= 0) {
        run_command(options.items[selected]);
    }

    free_dropdown(dropdown);
    free_str_arr(&options);
    free_dir_arr(&data);
    free((char*)path);
}

void handle_add_dir() {
    DirArr data;
    init_dir_arr(&data);
    deserialize(&data);

    const char* path = get_current_path();

    Dir* test_dir = get_dir(&data, path);
    if (test_dir != NULL) {
        fprintf(stderr, "This directory is already faved\n");
        free_dir_arr(&data);
        free((char*)path);
        return;
    }

    push_dir_arr(&data, path);

    serialize(&data);

    fprintf(stderr, "Faved: %s\n", path);

    free((char*)path);
    free_dir_arr(&data);
}
void handle_add_comm() {}

void handle_rm_dir() {
    DirArr data;
    init_dir_arr(&data);
    deserialize(&data);

    const char* path = get_current_path();

    Dir* test_dir = get_dir(&data, path);
    if (test_dir == NULL) {
        fprintf(stderr, "This directory is not faved\n");
        free_dir_arr(&data);
        free((char*)path);
        return;
    }

    pop_dir_arr(&data, path);

    serialize(&data);

    fprintf(stderr, "Removed: %s\n", path);

    free((char*)path);
    free_dir_arr(&data);
}
void handle_rm_comm() {}
