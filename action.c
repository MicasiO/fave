#include "action.h"
#include "array.h"
#include "dir.h"
#include "dropdown.h"
#include "utils.h"
#include <stdio.h>

void handle_show_dirs() {
    DirArr data;
    init_dir_arr(&data);
    deserialize(&data);

    if (data.size == 0) {
        printf("No faved directories\n");
        return;
    }

    printf("Open faved directories:\n\n");

    StrArr options;
    init_str_arr(&options);

    for (int i = 0; i < data.size; i++) {
        push_str_arr(&options, data.dirs[i]->title);
    }

    Dropdown* dropdown = create_dropdown(options.items, options.size);

    int selected = run_dropdown(dropdown);

    printf(CLEAR_SCREEN);
    if (selected >= 0) {
        printf("You selected: %s\n", options.items[selected]);
    }
    free_dropdown(dropdown);

    free_str_arr(&options);
    free_dir_arr(&data);
}

void handle_show_comms() {}

void handle_add_dir() {}
void handle_add_comm() {}

void handle_rm_dir() {}
void handle_rm_comm() {}
