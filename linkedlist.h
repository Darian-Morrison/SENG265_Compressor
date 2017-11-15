#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

typedef struct charval charval_t;
struct charval {
    unsigned char c;
    int           val;
    charval_t *next;
};

charval_t *new_charval(unsigned char c, int val);
charval_t *add_front(charval_t *, charval_t *);
charval_t *add_end(charval_t *, charval_t *);
charval_t *peek_front(charval_t *);
charval_t *remove_front(charval_t *);

int index_c(charval_t *list, char ele);

charval_t* pop_c(charval_t **list, int index);

charval_t* find_c(charval_t **list, int index);

void free_charval(charval_t *);

void apply(charval_t *, void(*fn)(charval_t *, void *), void *arg);

void print_list(charval_t* head);

void empty_list(charval_t* head);
#endif
