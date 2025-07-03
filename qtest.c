#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console.h"
#include "skiplist.h"

struct sl_list *skiplist;
int arr[SL_MAXLEVEL] = {0};

static int do_new(int argc, char** argv) {
    if (skiplist) 
        sl_free(skiplist);
    skiplist = sl_list_alloc();
    printf("Skiplist created\n");
    return 0;
}

static int do_insert(int argc, char** argv) {
    if (!skiplist) 
        return fprintf(stderr, "[error] Skiplist not initialized\n");
    if (argc > 2) 
        return fprintf(stderr, "[error] Too more arguments -> (<insert> <key>)\n");
    int key = atoi(argv[1]);
    if (sl_search(skiplist, key))
        return fprintf(stderr, "Key %d already in skiplist\n", key);
    sl_insert(skiplist, key, &arr[key]);
    printf("Inserted key = %d\n", key);
    return 0;
}

static int do_insert_max(int argc, char** argv) {
    if (!skiplist) 
        return fprintf(stderr, "[error] Skiplist not initialized\n");
    if (argc > 2) 
        return fprintf(stderr, "[error] Too more arguments -> (<maxinsert> <key>)\n");
    int key = atoi(argv[1]);
    for (int i = 1; i <= key; i++) {
        if (sl_search(skiplist, i))
            continue;
        sl_insert(skiplist, i, &arr[i]);
    }
    printf("Inserted 1 ~ %d keys in skiplist\n", key);
    return 0;
}

static int do_search(int argc, char** argv) {
    if (!skiplist) 
        return fprintf(stderr, "[error] Skiplist not initialized\n");
    if (argc > 2) 
        return fprintf(stderr, "[error] Too more arguments -> (<search> <key>)\n");
    int key = atoi(argv[1]);
    if (!sl_search(skiplist, key)) 
        return fprintf(stderr, "[error] %d is not in list\n", key);
    else 
        printf("search %d\n", key);
    return 0;
}

static int do_erase(int argc, char** argv) {
    if (!skiplist) 
        return fprintf(stderr, "[error] Skiplist not initialized\n");
    if (argc > 2) 
        return fprintf(stderr, "[error] Too more arguments -> (<erase> <key>)\n");
    int key = atoi(argv[1]);
    if (!sl_search(skiplist, key)) 
        return fprintf(stderr, "[error] %d is not in list\n", key);
    else {
        sl_erase(skiplist, key);
        printf("Erase %d in skiplist\n", key);
    }
    return 0;
}

static int do_print(int argc, char** argv) {
    if (!skiplist) 
        return fprintf(stderr, "[error] Skiplist not initialized\n");
    sl_printList(skiplist);
    return 0;
}

static int do_free(int argc, char** argv) {
    if (!skiplist) 
        return fprintf(stderr, "[error] Skiplist not existed\n");
    sl_free(skiplist);
    skiplist = NULL;
    printf("Skiplist already free\n");
    return 0;
}

static int do_help(int argc, char **argv) {
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
    return 0;
}

static int do_quit(int argc, char **argv) {
    printf("Quit test\n");
    exit(0);
    return 0;
}

void skiplist_console_init() {
    add_command("help", do_help, "Show list of available commands: <help>");
    add_command("new", do_new, "Create a new skiplist: <new>");
    add_command("insert", do_insert, "Insert key: <insert> <key>");
    add_command("maxinsert", do_insert_max, "Insert key from 1 to key value: <maxinsert> <key>");
    add_command("search", do_search, "Search key: <search> <key>");
    add_command("erase", do_erase, "Erase key: <erase> <key>");
    add_command("print", do_print, "Print whole skiplist: <print>");
    add_command("free", do_free, "Free whole skiplist: <free>");
    add_command("quit", do_quit, "Quit: <quit>");
}