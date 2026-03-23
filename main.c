#include "dir.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    dir* a = NULL;
    dir* b = NULL;

    if (argc != 4) {
        fprintf(stderr,
                "Error: Insufficient amount of options. Expected 3 arguments, got %d.\n",
                argc - 1);
        return 1;
    }

    char* command = argv[1];
    char* option = argv[2];
    char* value = argv[3];

    if (strcmp(command, "add") != 0 && strcmp(command, "show") != 0) {
        fprintf(stderr, "Error: unknown option '%s'. Available options are: add, show\n",
                command);
    }

    if (strcmp(option, "-c") != 0 && strcmp(option, "-d") != 0) {
        fprintf(stderr, "Error: unknown option '%s'. Available options are: -c, -d\n",
                option);
    }

    dir_arr data;
    init_dir_arr(&data);
    deserialize(&data);

    for (int i = 0; i < data.size; i++) {
        printf("%s\n", data.dirs[i]->title);
        for (int j = 0; j < data.dirs[i]->commands.size; j++) {

            printf("%s\n", data.dirs[i]->commands.items[j]);
        }
    }
    // push_dir_arr(&data, "/home/mike/Documents/Code/");
    // push_dir_arr(&data, "/home/mike/Documents/Code/cpp/calendar-tui/");
    //
    // a = get_dir(&data, "/home/mike/Documents/Code/");
    // if (a != NULL) {
    //     push_command_dir(a, "echo something");
    //     push_command_dir(a, "echo else");
    // }
    //
    // b = get_dir(&data, "/home/mike/Documents/Code/cpp/calendar-tui/");
    // if (b != NULL) {
    //     push_command_dir(b, "echo something");
    //     push_command_dir(b, "echo else");
    // }
    //
    // serialize(&data);

    free_dir_arr(&data);

    return 0;
}
