#include <stdlib.h>

#include "tree.h"



tree_t *create_tree(int (cmp(void*, void*)), size_t size) {
	tree_t * temp = (tree_t *)malloc(sizeof(tree_t));
	temp->root = NULL;
	temp->cmp = cmp;
	temp->size = size;
	return temp;
}

node_t *create_node(void *v1, void *v2) {
	node_t *temp = (node_t *)malloc(sizeof(node_t));
	temp->v1 = v1;
	temp->v2 = v2;
	temp->left = NULL;
	temp->right = NULL;
	temp->middle = NULL;
	temp->parent = NULL;
	
	temp->count = 0;
	temp->count += v1 != NULL ? 1 : 0;
	temp->count += v2 != NULL ? 1 : 0;
	temp->height = 1;
	return temp;
}

int get_depth(node_t *node) {
	if (node == NULL) {
		return 0;
	}
	return node->height;
}

int update_height(node_t *node) {
	if (node == NULL) {
		return 0;
	}
	int left_height = get_depth(node->left);
	int right_height = get_depth(node->right);
	node->height = 1 + (left_height > right_height ? left_height : right_height);
	return node->height;
}

void* inorder_sucessor(node_t *root) {
	node_t *temp = root->right;
	while (temp->left != NULL) {
		temp = temp->left;
	}
	return temp->v1;
}