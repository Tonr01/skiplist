/* total number of node is 2^32
 * the level here is log2(n), which is log2(2^32) = 32.
 */
#define SL_MAXLEVEL 32

struct sl_link {
    struct sl_link *prev, *next;
};

struct sl_list {
    int size;
    int level;
    struct sl_link head[SL_MAXLEVEL];
};

struct sl_list *sl_list_alloc(void);
void sl_delete(struct sl_list *list);
void *sl_search(struct sl_list *list, int key);
int sl_insert(struct sl_list *list, int key, void *val);
int sl_erase(struct sl_list *list, int key);

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define compiler_barrier() asm volatile("" : : : "memory")

struct sl_node {
    int key;
    void *val;
    struct sl_link link[0];
};

static inline void list_init(struct sl_link *node)
{
    node->next = node;
    compiler_barrier();
    node->prev = node;
}

static inline void __list_add(struct sl_link *new,
                              struct sl_link *prev,
                              struct sl_link *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    compiler_barrier();
    prev->next = new;
}

static inline void list_add(struct sl_link *new, struct sl_link *prev)
{
    __list_add(new, prev, prev->next);
}

static inline void __list_del(struct sl_link *prev, struct sl_link *next)
{
    next->prev = prev;
    compiler_barrier();
    prev->next = next;
}

static inline void list_del(struct sl_link *node)
{
    __list_del(node->prev, node->next);
    list_init(node);
}

#define list_for_each_from(pos, head) for (; pos != head; pos = pos->next)

#define list_for_each_safe_from(pos, n, head) \
    for (n = pos->next; pos != head; pos = n, n = pos->next)

#define container_of(ptr, type, member)                         \
    __extension__({                                             \
        const __typeof__(((type *) 0)->member) *__mptr = (ptr); \
        (type *) ((char *) __mptr - offsetof(type, member));    \
    })

#define list_entry(ptr, i) container_of(ptr, struct sl_node, link[i])

/* the probability set at 1/2 */

static inline void set_random(void)
{
    time_t current_time;
    srandom(time(&current_time));
}

static inline int random_level(void)
{
    uint32_t random_seed = (uint32_t) random();
    int level = __builtin_ffs(random_seed ^ -1) - 1;
    
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

#include <assert.h>
#include <stdio.h>

#define test(ops, index, size) \
    printf("[" #ops            \
           "]: "               \
           "return %d, "       \
           "index %d, "        \
           "size %d\n",        \
           (int) ops, index, size)

#define times 10000

int main(int argc, char *argv[])
{
    struct sl_list *list = sl_list_alloc();
    int i, arr[10] = {0};
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
    return 0;
}
