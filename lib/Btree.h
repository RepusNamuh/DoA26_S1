#include <stddef.h>
#include <stdlib.h>


#ifndef _2_3_H
#define _2_3_H

typedef struct node_23 node_23_t;
typedef struct tree_23 tree_23_t;

struct node_23 {
	void *v1;
	void *v2;
	void *overflow;
	node_23_t *left, *right, *mid1, *mid2, *parent;
	int count;

};

struct tree_23 {
	node_23_t *root;
	int (*cmp)(void*, void*);
	size_t size;
};


tree_23_t *create_23_tree(int (cmp(void*, void*)), size_t size);
node_23_t *create_23_node(void *v1, void *v2);
void insertOn23(tree_23_t *tree, void *v1, void *v2);
void deleteOn23(tree_23_t *tree, void *v1);
node_23_t* searchOn23(tree_23_t *tree, void *v1);
void print_23_tree(node_23_t *node, int level);


#endif