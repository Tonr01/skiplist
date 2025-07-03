#define test(ops, index, size) \
    printf("[" #ops            \
           "]: "               \
           "return %d, "       \
           "index %d, "        \
           "size %d\n",        \
           (int) ops, index, size)

#define SL_MAXLEVEL 32

#define compiler_barrier() asm volatile("" : : : "memory")

#define list_for_each_from(pos, head) for (; pos != head; pos = pos->next)

#define list_for_each_safe_from(pos, n, head) \
    for (n = pos->next; pos != head; pos = n, n = pos->next)

#define list_entry(ptr, i) container_of(ptr, struct sl_node, link[i])

#define container_of(ptr, type, member)                         \
    __extension__({                                             \
        const __typeof__(((type *) 0)->member) *__mptr = (ptr); \
        (type *) ((char *) __mptr - offsetof(type, member));    \
    })

struct sl_link {
    struct sl_link *prev, *next;
};

struct sl_list {
    int size;
    int level;
    struct sl_link head[SL_MAXLEVEL];
};

struct sl_node {
    int key;
    void *val;
    struct sl_link link[0];
};

struct sl_list *sl_list_alloc(void);
void sl_delete(struct sl_list *list);
void *sl_search(struct sl_list *list, int key);
int sl_insert(struct sl_list *list, int key, void *val);
int sl_erase(struct sl_list *list, int key);
void sl_printList(struct sl_list *list);
void sl_free(struct sl_list *list);

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