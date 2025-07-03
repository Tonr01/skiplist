#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "console.h"

#define MAX_COMMANDS 100

int command_count = 0;

struct {
    const char *name;
    command_func_t func;
    const char *desc;
} command[MAX_COMMANDS];

void add_command(const char *name, command_func_t func, const char *desc) {
    command[command_count].name = name;
    command[command_count].func = func;
    command[command_count].desc = desc;
    command_count++;
}

void run_console() {
    char line[128];
    printf("---------------------------------------------------\n");
    printf("Available commands:\n");
    printf("Show list of available commands: <help>\n");
    printf("Create a new skiplist: <new>\n");
    printf("Insert key: <insert> <key>\n");
    printf("Insert key from 1 to key value: <maxinsert> <key>\n");
    printf("Search key: <search> <key>\n");
    printf("Erase key: <erase> <key>\n");
    printf("Print whole skiplist: <print>\n");
    printf("Free whole skiplist: <free>\n");
    printf("Quit: <quit>\n");
    printf("---------------------------------------------------\n");
    while (fgets(line, sizeof(line), stdin)) {
        char *argv[10];
        int argc = 0;
        char *token = strtok(line, " \t\n");
        while (token && argc < 10) {
            argv[argc++] = token;
            token = strtok(NULL, " \t\n");
        }

        if (argc == 0) continue;

        int found = 0;
        for (int i = 0; i < command_count; i++) {
            if (strcmp(argv[0], command[i].name) == 0) {
                command[i].func(argc, argv);
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("Unknown command: %s\n", argv[0]);
        }
    }
}