#include "action.h"
#include "utils.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// NOTE: stderr is swapped with stdin.
//       fprintf(stdin, ...) is read into
//       the command to execute, so for
//       printing to the screen, use
//       fprintf(stderr, ...)

char* get_note_arg(int argc, char* argv[], Object obj);

int main(int argc, char* argv[]) {

    signal(SIGINT, handle_sigint);

    if (argc < 2) {
        fprintf(
            stderr,
            "Error: Insufficient amount of options. Expected 1-2 arguments, got %d.\n",
            argc - 1);
        return 1;
    }

    char* command = argv[1];
    char* note = NULL;

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

    if (action == ADD) {
        if (object == COMMAND) {
            if (argc != 3 && argc != 5) {
                fprintf(
                    stderr,
                    "Error: Insufficient amount of options. Expected 2 or 4 arguments, "
                    "got %d.\n",
                    argc - 1);
                return 1;
            }
        } else if (object == DIRECTORY) {
            if (argc != 2 && argc != 4) {
                fprintf(
                    stderr,
                    "Error: Insufficient amount of options. Expected 1 or 3 arguments, "
                    "got %d.\n",
                    argc - 1);
                return 1;
            }
        }
    } else {
        if (argc != 2) {
            fprintf(stderr,
                    "Error: Insufficient amount of options. Expected 1 argument, "
                    "got %d.\n",
                    argc - 1);
            return 1;
        }
    }

    switch (action) {
    case SHOW:
        if (object == DIRECTORY)
            handle_show_dirs();
        else
            handle_show_comms();
        break;

    case ADD:
        if (argc == 4 || argc == 5) {
            note = get_note_arg(argc, argv, object);
        }
        if (object == DIRECTORY)
            handle_add_dir(note);
        else
            handle_add_comm(argv[2], note);

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

    return 0;
}

char* get_note_arg(int argc, char* argv[], Object obj) {
    int i = -1;
    if (obj == COMMAND) {
        i = 3;
    } else {
        i = 2;
    }
    if (strcmp(argv[i], "-n") != 0) {
        fprintf(stderr,
                "Error: invalid option. If you are trying to add a note, use this "
                "format:\nfave %s -n 'note'\n",
                argv[1]);
        exit(1);
    }

    if (argv[i + 1][0] == '\0') {
        fprintf(stderr, "Error: note cannot be empty.\n");
        exit(1);
    }

    size_t len = strlen(argv[i + 1]) + 1;

    char* note = malloc(len);
    if (note == NULL) {
        return NULL;
    }

    snprintf(note, len, "%s", argv[i + 1]);
    return note;
}
