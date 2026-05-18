#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

int main(int argc, char* argv[]) {
    list_t *testList = createList(sizeof(int));
    int data = 10;
    int nextData = 20;
    int topData = 5;
    insert(testList, &data, 0);
    printf("Head: %d\n", *(int*)testList->head->data);
    printf("Tail: %d\n", *(int*)testList->tail->data);
    printf("Head Pointer: %p\n", (void*)testList->head);
    printf("Tail Pointer: %p\n", (void*)testList->tail);

    insert(testList, &nextData, testList->nmemb);
    printf("Tail Next Pointer %p\n:", (void*)testList->tail->next);
    printf("Head Prev Pointer %p\n:", (void*)testList->head->prev);
    insert(testList, &topData, 0);
    printf("Tail Next Pointer %p\n:", (void*)testList->tail->next);
    printf("Head Prev Pointer %p\n:", (void*)testList->head->prev);

    node_t *currNode = testList->head;

    for (int i = 0; i < testList->nmemb; i++) {
        printf("Node %d: %d\n", i, *(int*)currNode->data);
        currNode = currNode->next;
    }

    freeList(testList);
    testList = NULL;
    printf("testList ptr: %p\n", (void*)testList);
    assert(!testList);

    
    delHead(testList);
    printf("Head: %d\n", *(int*)testList->head->data);
    printf("Tail: %d\n", *(int*)testList->tail->data);
    printf("Tail Next Pointer: %p\n", (void*)testList->tail->next);
    printf("Head Prev Pointer: %p\n", (void*)testList->head->prev);

    delTail(testList);
    printf("Head: %d\n", *(int*)testList->head->data);
    printf("Tail: %d\n", *(int*)testList->tail->data);
    assert(testList->head);
    assert(testList->tail);

    delHead(testList);
    printf("Number of Element: %d\n", testList->nmemb);
    printf("Head Pointer: %p\n", (void*)testList->head);
    printf("Tail Pointer: %p\n", (void*)testList->tail);



}