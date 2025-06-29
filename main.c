#include "skiplist.h"

int main(int argc, char *argv[])
{
    struct sl_list *list = sl_list_alloc();
    int i, arr[10] = {0};
    for (i = 0; i < times; i++)
        sl_insert(list, i, &arr[i]);

    sl_printList(list);
    sl_free(list);

    /*
    for (i = 0; i < times; i++)
        assert(sl_insert(list, i, &arr[i]) == 0);
    test(sl_insert(list, i, NULL), i, list->size);

    for (i = 0; i < 10; i++) {
        arr[i] = i;
        printf("%d search %d\n", i, *(int *) sl_search(list, i));
    }

    for (i = 0; i < times; i++)
        assert(sl_erase(list, i) == 0);
    test(sl_erase(list, i), i, list->size);

    sl_delete(list);
    */
    return 0;
}