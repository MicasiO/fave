#include "utils.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// utils
const char* get_data_path() {
    char* home = getenv("HOME");
    if (home == NULL) {
        perror("Failed to open data directory");
        return NULL;
    }

    const char* sub_path = "/.local/share/fave.txt";

    size_t len = strlen(home) + strlen(sub_path) + 1;

    char* path = malloc(len);
    if (path == NULL) {
        return NULL;
    }

    snprintf(path, len, "%s%s", home, sub_path);
    return path;
}

int deserialize(DirArr* data) {
    FILE* file;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    Dir* current_dir = NULL;

    const char* path = get_data_path();

    file = fopen(path, "r");
    if (file == NULL) {
        free((char*)path);
        return 1;
    }

    while ((read = getline(&line, &len, file)) != -1) {
        if (read > 0 && line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }
        if (strncmp(line, "D:", 2) == 0) {
            push_dir_arr(data, line + 2);
            current_dir = get_dir(data, line + 2);
        }
        if (strncmp(line, "C:", 2) == 0) {
            if (current_dir != NULL) {
                push_command_dir(current_dir, line + 2);
            }
        }
    }

    free(line);
    free((char*)path);
    fclose(file);
    return 0;
}

int serialize(DirArr* data) {
    FILE* file;

    const char* path = get_data_path();

    file = fopen(path, "w");
    if (file == NULL) {
        perror("Failed to open data file");
        free((char*)path);
        return 1;
    }

    for (int d = 0; d < data->size; d++) {
        Dir* dir = data->dirs[d];

        fprintf(file, "D:%s\n", dir->title);

        for (int c = 0; c < dir->commands.size; c++) {
            fprintf(file, "C:%s\n", dir->commands.items[c]);
        }
    }

    free((char*)path);
    fclose(file);
    return 0;
}
