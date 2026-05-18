#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

list_t *createList(size_t data_size) {
    list_t *temp = (list_t*)malloc(sizeof(list_t));
    assert(temp);
    temp->size = data_size;
    temp->nmemb = 0;
    temp->head = NULL;
    temp->tail = NULL;
    
    return temp;
}

void insert(list_t *list, void *data, int pos) {
    node_t *temp = (node_t*)malloc(sizeof(node_t));
    temp->prev = NULL;
    temp->next = NULL;
    temp->data = data;
    if (list->head == NULL) {
        list->tail = temp;
        list->head = temp;
    } else if (pos == 0) {
        temp->next = list->head;
        list->head->prev = temp;
        list->head = temp;
    } else if (pos == list->nmemb) {
        list->tail->next = temp;
        temp->prev = list->tail;
        list->tail = temp;
    }
    else {
        return;
    }
    list->nmemb++;
}

void delHead(list_t *list) {
    if (list->head == NULL && list->tail == NULL) {
        return;
    }
    node_t *temp = list->head;
    list->head = temp->next;

    if (list->nmemb == 1) {
        list->tail = list->head;
    }
    else {
        list->head->prev = NULL;
    }
    
    free(temp);
    list->nmemb--;
}

void delTail(list_t *list) {
    if(list->nmemb < 1) {
        return;
    }
    node_t *temp = list->tail;
    list->tail = temp->prev;

    if (list->nmemb < 2) {
        list->head = list->tail;
    }
    else {
        list->tail->next = NULL;
    }

    free(temp);
    list->nmemb--;
}

void freeList(list_t *list) {
    for (int i = 0; i < list->nmemb; i++) {
        delHead(list);
    }
    free(list);
}

