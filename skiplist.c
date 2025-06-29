/* total number of node is 2^32
 * the level here is log2(n), which is log2(2^32) = 32.
 */

#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "skiplist.h"

/* the probability set at 1/2 */

static inline void set_random(void)
{
    time_t current_time;
    srandom(time(&current_time));
}

static inline int random_level(void)
{
    int level = 0;
    uint32_t random_seed = (uint32_t) random();

    while (random_seed && (random_seed & 1)) {
        random_seed >>= 1;
        level++;
    }

    return level >= SL_MAXLEVEL ? SL_MAXLEVEL - 1 : level;
}

static struct sl_node *sl_node_alloc(int key, void *val, int level)
{
    struct sl_node *node =
        malloc(sizeof(struct sl_node) + (level + 1) * sizeof(struct sl_link));
    if (!node)
        return NULL;

    node->key = key, node->val = val;

    return node;
}

struct sl_list *sl_list_alloc(void)
{
    struct sl_list *list = malloc(sizeof(struct sl_list));
    if (!list)
        return NULL;

    list->level = 0;
    list->size = 0;
    set_random();
    for (int i = 0; i < SL_MAXLEVEL; i++)
        list_init(&list->head[i]);

    return list;
}

void sl_delete(struct sl_list *list)
{
    struct sl_link *n, *pos = list->head[0].next;

    list_for_each_safe_from (pos, n, &list->head[0]) {
        struct sl_node *node = list_entry(pos, 0);
        free(node);
    }
    free(list);
}

void *sl_search(struct sl_list *list, int key)
{
    int i = list->level;
    struct sl_link *pos = &list->head[i];
    struct sl_link *head = &list->head[i];

    for (; i >= 0; i--) {
        pos = pos->next;
        list_for_each_from (pos, head) {
            struct sl_node *node = list_entry(pos, i);
            if (node->key > key)
                break;
            else if (node->key == key)
                return node->val;
        }
        pos = pos->prev;
        pos--;
        head--;
    }

    return NULL;
}

int sl_insert(struct sl_list *list, int key, void *val)
{
    int i, level = random_level();
    struct sl_link *pos = &list->head[level];
    struct sl_link *head = &list->head[level];
    struct sl_node *new = sl_node_alloc(key, val, level);

    if (!new)
        return -ENOMEM;
    if (level > list->level)
        list->level = level;

    for (i = level; i >= 0; i--) {
        pos = pos->next;
        list_for_each_from (pos, head) {
            struct sl_node *tmp = list_entry(pos, i);
            if (tmp->key > key) {
                break;
            } else if (tmp->key == key)
                goto failed;
        }
        pos = pos->prev;
        list_add(&new->link[i], pos); // IIII
        pos--;
        head--;
    }

    list->size++;

    return 0;
failed:
    free(new);
    return -EEXIST;
}

int sl_erase(struct sl_list *list, int key)
{
    int i = list->level;
    struct sl_link *pos = &list->head[i];
    struct sl_link *n, *head = &list->head[i];

    for (; i >= 0; i--) {
        pos = pos->next;
        list_for_each_safe_from(pos, n, head) { // JJJJ
            struct sl_node *tmp = list_entry(pos, i);
            if (tmp->key == key) {
                for (; i >= 0; i--) {
                    list_del(pos--);
                    if (list->head[i].next == &list->head[i])
                        list->level--;
                }
                free(tmp);
                list->size--;
                return 0;
            } else if (tmp->key > key)
                break;
        }
        pos = pos->prev;
        pos--;
        head--;
    }

    return -EINVAL;
}

void sl_printList(struct sl_list *list) {
    for (int i = list->level; i >= 0; i--) {
        int num = 0;
        struct sl_link *pos = &list->head[i];
        struct sl_link *head = &list->head[i];
        pos = pos->next;
        printf("Level %d = ", i);
        list_for_each_from (pos, head) {
            struct sl_node *node = list_entry(pos, i);
            printf("%d -> ", node->key);
            num++;
        }
        printf("NULL, Num of node = %d\n", num);
    }
}