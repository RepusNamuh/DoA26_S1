#include <stddef.h>
#include "tree.h"

#ifndef AVL_H
#define AVL_H

void insertOnAVL(tree_t *tree, void *v1);
void deleteOnAVL(tree_t *tree, void *v1);
node_t* searchOnAVL(tree_t *tree, void *v1);

#endif