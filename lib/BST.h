#include <stddef.h>
#include "tree.h"

#ifndef BST_H
#define BST_H


void insert_node(tree_t *tree, void *v1);
void delete_node(tree_t *tree, void *v1);
node_t* search(tree_t *tree, void *v1);

#endif