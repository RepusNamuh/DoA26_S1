/*
 * Starter file for Project 1 Part 2 (Link https://edstem.org/au/courses/31090/lessons/104103/slides/714974)
 *
 * Created for COMP20007 Design of Algorithms 2026
 * Template written by Thomas Minuzzo <thomas.minuzzo@unimelb.edu.au>
 * Implementation by NEIL LE.
 * Compile with gcc -Wall -g -o toposort toposort.c -lm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CODE_LENGTH 9
#define DEBUG 0
#define PRINTED -1

typedef struct{
    char code[CODE_LENGTH+1];//+1 for null byte
    char** pre_req;//Malloc each to CODE_LENGTH + !
    int num_prereqs;
}subject_t;

typedef struct node node_t;
struct  node {
    void *data;
    node_t *next;
};

typedef struct {
    node_t *head;
    node_t *end;
    int count;
}list_t;

/* Reads a list of subjects and their prerequisites into a dynamically allocated adjacency list*/
subject_t* read_major(int* num_subjects);
/* Prints the associated memory of a major, for debugging*/
void print_major(subject_t* major, int num_subjects);
/* Implements Kahn's algorithm to topologically sort a major's subjects */
void kahns_toposort(subject_t* major, int num_subjects);
/* Frees a dynamically allocated adjacency list*/
void free_major(subject_t* major, int num_subjects);
// Add a node_t to a list_t at the end;
void addNode(list_t* list, void *data);
// Pop the element at the top of the linked list
void* popTop(list_t* list);
// Frees a linked list
void freeList(list_t* list);

void printTopoSort(char** sorted, int newN, int oldN);

int main(int argc, char** argv){

    int num_subjects;
    subject_t* major = read_major(&num_subjects);

    if(DEBUG) print_major(major,num_subjects);

    kahns_toposort(major,num_subjects);

    free_major(major,num_subjects);

    return 0;
}

subject_t* read_major(int* num_subjects){

    scanf("Number of subjects: %d\n", num_subjects);

    subject_t* major = (subject_t*)malloc(sizeof(subject_t)*(*num_subjects));

    for(int i = 0 ; i < (*num_subjects) ; i++){
        for(int c = 0 ; c < CODE_LENGTH ; c++){
            scanf("%c", &major[i].code[c]);
        }
        major[i].code[CODE_LENGTH]='\0';

        if(DEBUG){printf("%s\n",major[i].code);}
        
        scanf("(%d)",&major[i].num_prereqs);

        major[i].pre_req = (char**)malloc(sizeof(char*)*major[i].num_prereqs);
        for(int j = 0 ; j < major[i].num_prereqs ; j++){
             major[i].pre_req[j] = (char*)malloc(sizeof(char)*(CODE_LENGTH+1));
            for(int c = 0 ; c < CODE_LENGTH ; c++){
                scanf("%c", &major[i].pre_req[j][c]);
            } 
            major[i].pre_req[j][CODE_LENGTH]='\0';
            if(DEBUG){printf("%s\n",major[i].pre_req[j]);}

            scanf(",");
                   
        }
        scanf("\n");
        

    }
    return major;

}

void print_major(subject_t* major, int num_subjects){
    for(int i = 0 ; i < num_subjects ; i++){
        printf("%s - ", major[i].code);
        for(int j = 0 ; j < major[i].num_prereqs ; j++){
            printf("%s", major[i].pre_req[j]);
            if(j!=major[i].num_prereqs-1){ //Not on last pre-requisite, needs a comma
                printf(", ");
            }
        }
        printf("\n");

    }

}
void addNode(list_t* list, void *data) {
    node_t *temp = (node_t*)malloc(sizeof(node_t));
    temp->data = data;
    temp->next = NULL;
	// The list is empty, set head and end to the same node_t
    if (!list->head || !list->end) {
        list->head = list->end = temp;
    }
    else {
        list->end->next = temp;
        list->end = temp;
    }
    list->count++;
}

void* popTop(list_t* list) {
    if (!list->head || !list->end) {
        return NULL;
    }
    node_t *top = list->head;
	void *data = top->data;
    list->head = top->next;
	// Empty linked list
	if (!list->head) {
		list->end = NULL;
	}

    list->count--;
	free(top);

    return data;
}

void freeList(list_t *list) {
    while (list->head) {
        node_t *temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
    free(list);
}

void printTopoSort(char** sorted, int newN, int oldN) {
    // If number element in sorted list is not same as original list
    if (newN < oldN) {
        printf("Pre-requisites impossible - check for cycles or missing pre-requisites\n");
    }
    else {
        printf("Sorted Order:\n");
        for (int i = 0; i < newN; i++) {
            printf("%s\n", sorted[i]);
        }
    }

}

void kahns_toposort(subject_t* major, int num_subjects){
    int n = 0;
    char* topoOrder[num_subjects];
    list_t* noPreReq = (list_t*)malloc(sizeof(list_t));
    noPreReq->head = noPreReq->end = NULL;
    noPreReq->count = 0;

   //Find the all the subject with no initial prereq.
    for (int i = 0; i < num_subjects; i++) {
        if (major[i].num_prereqs == 0) {
            addNode(noPreReq, &major[i].code);
        } 
    }

    while (noPreReq->count > 0) {
        char* curr_subject  = (char*)popTop(noPreReq);
        topoOrder[n++] = curr_subject;

        for (int i = 0; i < num_subjects; i++) {

            int* size = &major[i].num_prereqs;
            // Only proceed if the subject have pre-req
            if(*size) {
                
                for (int j = 0; j < *size; j++) {
					// Check if curr_subject is a requisite for major[i]
                    if(!strcmp(major[i].pre_req[j], curr_subject)) {
                        // Elimination by freeing major[i].pre_req[j]
						// Then point the empty slot to the end of the list
						free(major[i].pre_req[j]);
                        major[i].pre_req[j] = major[i].pre_req[--(*size)];
                        break;
                    }
                }
                if (*size <= 0) {
                    addNode(noPreReq, &major[i].code);
                }
            }
        }
    }
    printTopoSort(topoOrder, n, num_subjects);
    freeList(noPreReq);
    noPreReq = NULL;
}

void free_major(subject_t* major, int num_subjects){
    for(int i = 0; i < num_subjects ; i++){
        //Note: this set, j, will be empty after Kahn's algorithm
        for(int j = 0; j < major[i].num_prereqs ; j++){
            free(major[i].pre_req[j]);
        }
        free(major[i].pre_req);
    }
    free(major);
}


/* =====================================================================
   This skeleton/program is compiled by the comp20007 teaching team,
   the university of Melbourne.
   Last Updated: 16/02/2026
   ================================================================== */