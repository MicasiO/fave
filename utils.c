#include "utils.h"
#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

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

const char* get_current_path() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Failed to get current directory path");
        return NULL;
    }

    size_t len = strlen(cwd) + 2;

    char* path = malloc(len);
    if (path == NULL) {
        return NULL;
    }

    snprintf(path, len, "%s/", cwd);
    return path;
}

int deserialize(DirArr* data) {
    FILE* file = fopen(get_data_path(), "r");
    if (!file) {
        return 1;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    Dir* current_dir = NULL;
    Comm* current_comm = NULL;

    while ((read = getline(&line, &len, file)) != -1) {
        if (read > 0 && line[read - 1] == '\n')
            line[read - 1] = '\0';

        // handle directory
        if (strncmp(line, "D:", 2) == 0) {
            push_dir_arr(data, line + 2, NULL);
            current_dir = data->dirs[data->size - 1];
            current_comm = NULL;
        }

        // handle command
        else if (strncmp(line, "C:", 2) == 0 && current_dir != NULL) {
            push_command_dir(current_dir, line + 2, NULL);
            current_comm = current_dir->commands[current_dir->size - 1];
        }

        // handle note
        else if (strncmp(line, "N:", 2) == 0) {
            if (current_comm != NULL) {
                free(current_comm->note);
                current_comm->note = strdup(line + 2);
            } else if (current_dir != NULL) {
                free(current_dir->note);
                current_dir->note = strdup(line + 2);
            }
        }
    }

    free(line);
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

        fprintf(file, "D:%s\n", dir->path);
        if (dir->note != NULL) {
            fprintf(file, "N:%s\n", dir->note);
        }

        for (int c = 0; c < dir->size; c++) {
            fprintf(file, "C:%s\n", dir->commands[c]->command);
            if (dir->commands[c]->note != NULL) {
                fprintf(file, "N:%s\n", dir->commands[c]->note);
            }
        }
    }

    free((char*)path);
    fclose(file);
    return 0;
}

void handle_sigint(int sig) {
    reset_terminal();
    exit(0);
}

void run_command(const char* cmd) {
    pid_t pid = fork();

    if (pid == 0) {
        char* args[] = {"sh", "-c", (char*)cmd, NULL};
        execvp("sh", args);
        perror("Failed to execute command");
        exit(1);
    } else {
        wait(NULL);
    }
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
