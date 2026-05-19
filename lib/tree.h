#include <stddef.h>

#ifndef TREE_H
#define TREE_H

typedef struct node node_t;
typedef struct tree tree_t;

struct node {
	void *v1;
	void *v2;
	node_t *left;
	node_t *right;
	node_t *middle;
	node_t *parent;
	int height;
	int count;
};

struct tree {
	node_t *root;
	int (*cmp)(void*, void*);
	size_t size;
};

tree_t *create_tree(int (cmp)(void*, void*), size_t size);
node_t *create_node(void *v1, void *v2);
int get_depth(node_t *node);
int update_height(node_t *node);
void* inorder_sucessor(node_t *root);

#endif