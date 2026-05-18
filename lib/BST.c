#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

node_t* insert_helper(node_t *root, void *v1, int(cmp)(void*, void*));
node_t* search_helper(node_t *root, void *v1, int(cmp)(void*, void*));
node_t* delete_helper(node_t *root, void *v1, int(cmp)(void*, void*));

void insert_node(tree_t *tree, void *v1) {
	tree->root = insert_helper(tree->root, v1,tree->cmp);
}

node_t* insert_helper(node_t *root, void *v1, int(cmp)(void*, void*)) {
	if (root == NULL) {
		return create_node(v1, NULL);
	}
	else if (cmp(v1, root->v1) < 0) {
		root->left = insert_helper(root->left, v1, cmp);
	}
	else if (cmp(v1, root->v1) > 0) {
		root->right = insert_helper(root->right, v1, cmp);
	}
	return root;
}

node_t* search(tree_t *tree, void *v1) {
	return search_helper(tree->root, v1, tree->cmp);
}

node_t* search_helper(node_t *root, void *v1, int(cmp)(void*, void*)) {
	if (root == NULL) {
		return NULL;
	}
	else if (root->v1 == v1) {
		return root;
	}
	else if (cmp(v1, root->v1) < 0) {
		return search_helper(root->left, v1, cmp);
	}
	else {
		return search_helper(root->right, v1, cmp);
	}
}

void delete_node(tree_t *tree, void *v1) {
	tree->root = delete_helper(tree->root, v1, tree->cmp);
}

node_t* delete_helper(node_t *root, void *v1, int(cmp)(void*, void*)) {
	if (root == NULL) {
		return root;
	}
	else if (cmp(v1, root->v1) < 0) {
		root->left = delete_helper(root->left, v1, cmp);
	}
	else if (cmp(v1, root->v1) > 0) {
		root->right = delete_helper(root->right, v1, cmp);	
	}
	else {
		if (root->left == NULL) {
			node_t *temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL) {
			node_t *temp = root->left;
			free(root);
			return temp;
		}
		else {
			// Go Right, then all the way left (inorder sucessor)
			root->v1 = inorder_sucessor(root);
			root->right = delete_helper(root->right, root->v1, cmp);
		}
	}
	update_height(root);
	return root;
}