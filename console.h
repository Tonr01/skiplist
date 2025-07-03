#ifndef CONSOLE_H
#define CONSOLE_H

#define MAX_COMMANDS 100

typedef int (*command_func_t)(int argc, char **argv);

void add_command(const char *name, command_func_t func, const char *desc);
void run_console();

#endif