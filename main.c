#include "action.h"
#include "utils.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    signal(SIGINT, handle_sigint);

    if (argc != 2 && argc != 3) {
        fprintf(
            stderr,
            "Error: Insufficient amount of options. Expected 1-2 arguments, got %d.\n",
            argc - 1);
        return 1;
    }

    char* command = argv[1];
    char* addition_arg = NULL;

    if (strlen(command) > 2) {
        fprintf(stderr, "Error: Invalid command.\n");
        return 1;
    }

    Object object = -1;
    Action action = -1;

    for (int i = 0; command[i] != '\0'; i++) {
        switch (command[i]) {
        case 's':
            action = SHOW;
            break;
        case 'a':
            action = ADD;
            break;
        case 'r':
            action = REMOVE;
            break;
        case 'd':
            object = DIRECTORY;
            break;
        case 'c':
            object = COMMAND;
            break;
        default:
            fprintf(stderr, "Error: Unknown flag '%c'\n", command[i]);
            return 1;
        }
    }

    if (action == -1 || object == -1) {
        fprintf(stderr,
                "Error: Must specify both an action (s,a,r) and an object (d,c).\n");
        return 1;
    }

    switch (action) {
    case SHOW:
        if (object == DIRECTORY)
            handle_show_dirs();
        else
            handle_show_comms();
        break;

    case ADD:
        if (object == DIRECTORY)
            handle_add_dir();
        else
            handle_add_comm();
        break;

    case REMOVE:
        if (object == DIRECTORY)
            handle_rm_dir();
        else
            handle_rm_comm();
        break;

    default:
        break;
    }

    // DirArr data;
    // init_dir_arr(&data);
    // deserialize(&data);
    //
    // for (int i = 0; i < data.size; i++) {
    //     printf("%s\n", data.dirs[i]->title);
    //     for (int j = 0; j < data.dirs[i]->commands.size; j++) {
    //
    //         printf("%s\n", data.dirs[i]->commands.items[j]);
    //     }
    // }
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
    //
    // free_dir_arr(&data);

    return 0;
}
