#include "action.h"
#include "array.h"
#include "dir.h"
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
                fprintf(stdout, "cd '%s'\n", data.dirs[number - 1]->title);
                fprintf(stderr, "\n");
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
        free((char*)path);
        free_dir_arr(&data);
        fprintf(stderr, "This directory is not faved\n");
        return;
    }

    if (dir->commands.size == 0) {
        free((char*)path);
        free_dir_arr(&data);
        fprintf(stderr, "This directory has no faved commands\n");
        return;
    }

    fprintf(stderr, BOLD "Faved commands:\n" RESET);
    for (int i = 0; i < dir->commands.size; i++) {
        fprintf(stderr, "%d) %s\n", i + 1, dir->commands.items[i]);
    }

    fprintf(stderr, BOLD "\nSelect number: " RESET);
    char input[256];
    int number;

    if (fgets(input, sizeof(input), stdin)) {
        if (sscanf(input, "%d", &number) == 1) {
            if (number - 1 >= 0 && number - 1 < dir->commands.size) {
                fprintf(stdout, "%s\n", dir->commands.items[number - 1]);
                free((char*)path);
                free_dir_arr(&data);
                return;
            }
        }
    }

    fprintf(stderr, "Invalid input\n");
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

    fprintf(stderr, BOLD "Faved: " RESET "%s\n", path);

    free((char*)path);
    free_dir_arr(&data);
}

void handle_add_comm(const char* comm) {
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

    push_command_dir(test_dir, comm);

    serialize(&data);

    fprintf(stderr, BOLD "Faved: " RESET "%s\n", comm);

    free((char*)path);
    free_dir_arr(&data);
}

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

    fprintf(stderr, BOLD "Removed: " RESET "%s\n", path);

    free((char*)path);
    free_dir_arr(&data);
}

void handle_rm_comm() {
    const char* path = get_current_path();

    DirArr data;
    init_dir_arr(&data);
    deserialize(&data);

    Dir* dir = get_dir(&data, path);

    if (dir == NULL) {
        free((char*)path);
        free_dir_arr(&data);
        fprintf(stderr, "This directory is not faved\n");
        return;
    }

    if (dir->commands.size == 0) {
        free((char*)path);
        free_dir_arr(&data);
        fprintf(stderr, "This directory has no faved commands\n");
        return;
    }

    fprintf(stderr, BOLD "Remove faved command:\n" RESET);
    for (int i = 0; i < dir->commands.size; i++) {
        fprintf(stderr, "%d) %s\n", i + 1, dir->commands.items[i]);
    }

    fprintf(stderr, BOLD "\nSelect number: " RESET);
    char input[256];
    int number;

    if (fgets(input, sizeof(input), stdin)) {
        if (sscanf(input, "%d", &number) == 1) {
            if (number - 1 >= 0 && number - 1 < dir->commands.size) {
                fprintf(stderr, BOLD "Removed: " RESET "%s",
                        dir->commands.items[number - 1]);
                pop_command_dir(dir, dir->commands.items[number - 1]);
                serialize(&data);

                free((char*)path);
                free_dir_arr(&data);
                return;
            }
        }
    }

    fprintf(stderr, "Invalid input\n");
    free_dir_arr(&data);
    free((char*)path);
}
