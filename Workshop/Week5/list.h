#include <stddef.h>

#ifndef LIST_H
#define LIST_H

typedef struct node node_t;

struct node {
    void *data;
    node_t *next;
    node_t *prev;
};

typedef struct {
    node_t *head;
    node_t *tail;
    int nmemb;
    size_t size;
} list_t;

list_t *createList(size_t data_size);

void insert(list_t *list, void *data, int pos);
void delHead(list_t *list);
void delTail(list_t *list);
void freeList(list_t *list);

#endif