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
