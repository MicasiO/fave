#ifndef ACTION_H
#define ACTION_H

typedef enum { ADD, REMOVE, SHOW } Action;
typedef enum { DIRECTORY, COMMAND } Object;

void handle_show_dirs();
void handle_show_comms();

void handle_add_dir(char* note);
void handle_add_comm(const char* comm, char* note);

void handle_rm_dir();
void handle_rm_comm();

#endif
