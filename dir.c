#include "dir.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// dir
int init_dir(dir* dir, const char* title) {
    if (dir == NULL || title == NULL) {
        return 1;
    }

    DIR* valid = opendir(title);
    if (valid == NULL) {
        perror("Failed to open directory");
        return 1;
    }

    size_t len = strlen(title) + 1;

    dir->title = malloc(len);
    strcpy(dir->title, title);

    init_str_arr(&(dir->commands));
    closedir(valid);
    return 0;
}

int push_command_dir(dir* dir, const char* comm) {
    return push_str_arr(&(dir->commands), comm);
}
int pop_command_dir(dir* dir, const char* comm) {
    return pop_str_arr(&(dir->commands), comm);
}

dir* get_dir(dir_arr* data, const char* title) {
    if (data == NULL || title == NULL) {
        return NULL;
    }

    for (int i = 0; i < data->size; i++) {
        if (strcmp(data->dirs[i]->title, title) == 0) {
            return data->dirs[i];
        }
    }

    return NULL;
}

void free_dir(dir* dir) {
    if (dir == NULL) {
        return;
    }

    free_str_arr(&(dir->commands));
    free(dir->title);
    free(dir);
}

// dir array
int init_dir_arr(dir_arr* data) {
    data->size = 0;
    data->capacity = 4;
    data->dirs = calloc(data->capacity, sizeof(dir*));

    if (data->dirs == NULL) {
        perror("Failed to allocate memory for string array");
        return 1;
    }

    return 0;
}

int push_dir_arr(dir_arr* data, const char* title) {
    if (data->size == data->capacity) {
        int new_capacity = data->capacity * 2;

        dir** temp = realloc(data->dirs, sizeof(dir*) * new_capacity);

        if (temp == NULL) {
            perror("Failed to resize array");
            return 1;
        }

        data->dirs = temp;
        data->capacity = new_capacity;
    }

    data->dirs[data->size] = malloc(sizeof(dir));

    if (data->dirs[data->size] == NULL) {
        perror("Failed to allocated dir struct");
        return 1;
    }

    if (init_dir(data->dirs[data->size], title) != 0) {
        free(data->dirs[data->size]);
        return 1;
    }

    if (data->dirs[data->size] == NULL) {
        return 1;
    }

    data->size++;
    return 0;
}

void free_dir_arr(dir_arr* data) {
    if (data == NULL) {
        return;
    }

    for (int i = 0; i < data->size; i++) {
        if (data->dirs[i] != NULL) {
            free_dir(data->dirs[i]);
        }
    }

    free(data->dirs);

    data->dirs = NULL;
}

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

int deserialize(dir_arr* data) {
    FILE* file;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    dir* current_dir = NULL;

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

int serialize(dir_arr* data) {
    FILE* file;

    const char* path = get_data_path();

    file = fopen(path, "w");
    if (file == NULL) {
        perror("Failed to open data file");
        free((char*)path);
        return 1;
    }

    for (int d = 0; d < data->size; d++) {
        dir* dir = data->dirs[d];

        fprintf(file, "D:%s\n", dir->title);

        for (int c = 0; c < dir->commands.size; c++) {
            fprintf(file, "C:%s\n", dir->commands.items[c]);
        }
    }

    free((char*)path);
    fclose(file);
    return 0;
}
