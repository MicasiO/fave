#include "dir.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// dir
int init_dir(Dir* dir, const char* title) {
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

int push_command_dir(Dir* dir, const char* comm) {
    return push_str_arr(&(dir->commands), comm);
}
int pop_command_dir(Dir* dir, const char* comm) {
    return pop_str_arr(&(dir->commands), comm);
}

void free_dir(Dir* dir) {
    if (dir == NULL) {
        return;
    }

    free_str_arr(&(dir->commands));
    free(dir->title);
    free(dir);
}

// dir array
int init_dir_arr(DirArr* data) {
    data->size = 0;
    data->capacity = 4;
    data->dirs = calloc(data->capacity, sizeof(Dir*));

    if (data->dirs == NULL) {
        perror("Failed to allocate memory for string array");
        return 1;
    }

    return 0;
}

int push_dir_arr(DirArr* data, const char* title) {
    if (data->size == data->capacity) {
        int new_capacity = data->capacity * 2;

        Dir** temp = realloc(data->dirs, sizeof(Dir*) * new_capacity);

        if (temp == NULL) {
            perror("Failed to resize array");
            return 1;
        }

        data->dirs = temp;
        data->capacity = new_capacity;
    }

    data->dirs[data->size] = malloc(sizeof(Dir));

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

int pop_dir_arr(DirArr* data, const char* title) {
    int found_index = -1;

    for (int i = 0; i < data->size; i++) {
        if (strcmp(data->dirs[i]->title, title) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        return 1;
    }

    free_dir(data->dirs[found_index]);

    for (int i = found_index; i < data->size - 1; i++) {
        data->dirs[i] = data->dirs[i + 1];
    }

    data->size--;

    data->dirs[data->size] = NULL;

    return 0;
}

Dir* get_dir(DirArr* data, const char* title) {
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

void free_dir_arr(DirArr* data) {
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
