#ifndef ACTION_H
#define ACTION_H

typedef enum { ADD, REMOVE, SHOW } Action;
typedef enum { DIRECTORY, COMMAND } Object;

// int show(Object obj);
// int add(Object obj);
// int remove(Object obj);
//
void handle_show_dirs();
void handle_show_comms();

void handle_add_dir();
void handle_add_comm();

void handle_rm_dir();
void handle_rm_comm();

#endif
