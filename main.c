#include "skiplist.h"
#include "console.h"

extern void skiplist_console_init();

int main(int argc, char *argv[]) {
    skiplist_console_init();
    run_console();
    return 0;
}