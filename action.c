#include "action.h"
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
        if (data.dirs[i]->note != NULL) {
            fprintf(stderr, "%d) '%s' - %s\n", i + 1, data.dirs[i]->note,
                    data.dirs[i]->path);
        } else {
            fprintf(stderr, "%d) %s\n", i + 1, data.dirs[i]->path);
        }
    }

    fprintf(stderr, BOLD "\nSelect number: " RESET);
    char input[256];
    int number;

    if (fgets(input, sizeof(input), stdin)) {
        if (sscanf(input, "%d", &number) == 1) {
            if (number - 1 >= 0 && number - 1 < data.size) {
                fprintf(stdout, "cd '%s'\n", data.dirs[number - 1]->path);
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

    if (dir->size == 0) {
        free((char*)path);
        free_dir_arr(&data);
        fprintf(stderr, "This directory has no faved commands\n");
        return;
    }

    fprintf(stderr, BOLD "Faved commands:\n" RESET);
    for (int i = 0; i < dir->size; i++) {
        if (dir->commands[i]->note != NULL) {
            fprintf(stderr, "%d) '%s' - %s\n", i + 1, dir->commands[i]->note,
                    dir->commands[i]->command);
        } else {
            fprintf(stderr, "%d) %s\n", i + 1, dir->commands[i]->command);
        }
    }

    fprintf(stderr, BOLD "\nSelect number: " RESET);
    char input[256];
    int number;

    if (fgets(input, sizeof(input), stdin)) {
        if (sscanf(input, "%d", &number) == 1) {
            if (number - 1 >= 0 && number - 1 < dir->size) {
                fprintf(stdout, "%s\n", dir->commands[number - 1]->command);
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

void handle_add_dir(char* note) {

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

    push_dir_arr(&data, path, note);

    serialize(&data);

    fprintf(stderr, BOLD "Faved: " RESET "%s\n", path);

    free((char*)path);
    free_dir_arr(&data);
}

void handle_add_comm(const char* comm, char* note) {

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

    for (int i = 0; i < test_dir->size; i++) {
        if (strcmp(test_dir->commands[i]->command, comm) == 0) {
            fprintf(stderr, "This command is already faved\n");
            free_dir_arr(&data);
            free((char*)path);
            return;
        }
    }

    push_command_dir(test_dir, comm, note);

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

    if (dir->size == 0) {
        free((char*)path);
        free_dir_arr(&data);
        fprintf(stderr, "This directory has no faved commands\n");
        return;
    }

    fprintf(stderr, BOLD "Remove faved command:\n" RESET);
    for (int i = 0; i < dir->size; i++) {
        if (dir->commands[i]->note != NULL) {
            fprintf(stderr, "%d) '%s' - %s\n", i + 1, dir->commands[i]->note,
                    dir->commands[i]->command);
        } else {
            fprintf(stderr, "%d) %s\n", i + 1, dir->commands[i]->command);
        }
    }

    fprintf(stderr, BOLD "\nSelect number: " RESET);
    char input[256];
    int number;

    if (fgets(input, sizeof(input), stdin)) {
        if (sscanf(input, "%d", &number) == 1) {
            if (number - 1 >= 0 && number - 1 < dir->size) {
                fprintf(stderr, BOLD "Removed: " RESET "%s\n",
                        dir->commands[number - 1]->command);
                pop_command_dir(dir, dir->commands[number - 1]->command);
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
