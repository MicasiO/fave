#include "dir.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// dir
int init_dir(Dir* dir, const char* path, char* note) {

    if (dir == NULL || path == NULL) {
        return 1;
    }

    // validate directory
    DIR* valid = opendir(path);
    if (valid == NULL) {
        perror("Failed to open directory");
        return 1;
    }
    closedir(valid);

    // setup commands array
    dir->size = 0;
    dir->capacity = 4;
    dir->commands = (Comm**)calloc(dir->capacity, sizeof(*dir->commands));

    if (dir->commands == NULL) {
        perror("Failed to allocate memory for command array");
        return 1;
    }

    dir->path = strdup(path);
    dir->note = (note != NULL) ? strdup(note) : NULL;

    return 0;
}

int push_command_dir(Dir* dir, const char* comm, char* note) {

    if (dir == NULL) {
        return 1;
    }

    // resize if full
    if (dir->size == dir->capacity) {
        int new_capacity = dir->capacity * 2;
        Comm** temp = realloc(dir->commands, sizeof(Comm*) * new_capacity);

        if (temp == NULL) {
            perror("Failed to resize array");
            return 1;
        }

        dir->commands = temp;
        dir->capacity = new_capacity;
    }

    Comm* new_comm = malloc(sizeof(Comm));
    if (new_comm == NULL)
        return 1;

    new_comm->command = strdup(comm);
    new_comm->note = (note != NULL) ? strdup(note) : NULL;

    dir->commands[dir->size] = new_comm;
    dir->size++;
    return 0;
}

int pop_command_dir(Dir* dir, const char* comm) {
    int found_index = -1;

    // find command index in array
    for (int i = 0; i < dir->size; i++) {
        if (strcmp(dir->commands[i]->command, comm) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        return 1;
    }

    // delete command
    free(dir->commands[found_index]->note);
    free(dir->commands[found_index]->command);
    free(dir->commands[found_index]);

    // shift commands back in array
    for (int i = found_index; i < dir->size - 1; i++) {
        dir->commands[i] = dir->commands[i + 1];
    }

    dir->size--;
    dir->commands[dir->size] = NULL;

    return 0;
}

void free_dir(Dir* dir) {
    if (dir == NULL) {
        return;
    }

    for (int i = 0; i < dir->size; i++) {
        if (dir->commands[i] != NULL) {
            free(dir->commands[i]->note);
            free(dir->commands[i]->command);
            free(dir->commands[i]);
        }
    }

    free(dir->commands);

    dir->commands = NULL;
    dir->capacity = 0;
    dir->size = 0;

    free(dir->note);
    free(dir->path);
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

int push_dir_arr(DirArr* data, const char* path, char* note) {
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
        perror("Failed to allocate dir struct");
        return 1;
    }

    if (init_dir(data->dirs[data->size], path, note) != 0) {
        free(data->dirs[data->size]);
        return 1;
    }

    if (data->dirs[data->size] == NULL) {
        return 1;
    }

    data->size++;
    return 0;
}

int pop_dir_arr(DirArr* data, const char* path) {
    int found_index = -1;

    for (int i = 0; i < data->size; i++) {
        if (strcmp(data->dirs[i]->path, path) == 0) {
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

Dir* get_dir(DirArr* data, const char* path) {
    if (data == NULL || path == NULL) {
        return NULL;
    }

    for (int i = 0; i < data->size; i++) {
        if (strcmp(data->dirs[i]->path, path) == 0) {
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
