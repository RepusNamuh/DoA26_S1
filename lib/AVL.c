#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "AVL.h"


node_t* avlSearch_healper(node_t *root, void *v1, int(cmp)(void*, void*));
int balance_factor(node_t *node);
node_t* balance_tree(node_t *node);
node_t *right_rotate(node_t *node);
node_t *left_rotate(node_t *node);
node_t *avlInsert_helper(node_t *root, void *v1, int(cmp)(void*, void*));
node_t *avldelete_helper(node_t *root, void *v1, int(cmp)(void*, void*));

void insertOnAVL(tree_t *tree, void *v1) {
	tree->root = avlInsert_helper(tree->root, v1, tree->cmp);
}

void deleteOnAVL(tree_t *tree, void *v1) {
	tree->root = avldelete_helper(tree->root, v1, tree->cmp);
}

node_t* searchOnAVL(tree_t *tree, void *v1) {
	return avlSearch_healper(tree->root, v1, tree->cmp);
}

node_t *avlInsert_helper(node_t *root, void *v1, int(cmp)(void*, void*)) {
	if (root == NULL) {
		return create_node(v1, NULL);
	}
	else if (cmp(v1, root->v1) < 0) {
		root->left = avlInsert_helper(root->left, v1, cmp);
	}
	else if (cmp(v1, root->v1) > 0) {
		root->right = avlInsert_helper(root->right, v1, cmp);
	}
	else {
		return root;
	}
	// Balancing the tree after insertion
	update_height(root);
	return balance_tree(root);
}

node_t* avldelete_helper(node_t *root, void *v1, int(cmp)(void*, void*)) {
	if (root == NULL) {
		return root;
	}
	else if (cmp(v1, root->v1) < 0) {
		root->left = avldelete_helper(root->left, v1, cmp);
	}
	else if (cmp(v1, root->v1) > 0) {
		root->right = avldelete_helper(root->right, v1, cmp);	
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
			root->right = avldelete_helper(root->right, root->v1, cmp);
		}
	}
	update_height(root);
	return balance_tree(root);
}

node_t* avlSearch_healper(node_t *root, void *v1, int(cmp)(void*, void*)) {
	if (root == NULL) {
		return NULL;
	}
	else if (root->v1 == v1) {
		return root;
	}
	else if (cmp(v1, root->v1) < 0) {
		return avlSearch_healper(root->left, v1, cmp);
	}
	else {
		return avlSearch_healper(root->right, v1, cmp);
	}
}

int balance_factor(node_t *node) {
	if (node == NULL) {
		return 0;
	}
	return get_depth(node->left) - get_depth(node->right);
}

node_t *right_rotate(node_t *node) {
	node_t *new_root = node->left;
	node_t *temp = new_root->right;

	new_root->right = node;
	node->left = temp;

	update_height(node);
	update_height(new_root);

	return new_root;
}

node_t *left_rotate(node_t *node) {
	node_t *new_node = node->right;
	node_t *temp = new_node->left;

	new_node->left = node;
	node->right = temp;

	update_height(node);
	update_height(new_node);

	return new_node;
}

node_t* balance_tree(node_t *node) {
	int balance = balance_factor(node);
	if (balance > 1) {
		if (balance_factor(node->left) >= 0) {
			// Left left Case
			return right_rotate(node);
		}
		else {
			// Left Right Case
			node->left = left_rotate(node->left);
			return right_rotate(node);
		}
	}
	else if (balance < -1) {
		if (balance_factor(node->right) <= 0) {
			// Right Right Case
			return left_rotate(node);
		}
		else {
			// Right Left Case
			node->right = right_rotate(node->right);
			return left_rotate(node);
		}
	}
	return node;
}