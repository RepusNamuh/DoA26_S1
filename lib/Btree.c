#include <stdio.h>
#include <stdlib.h>
#include "Btree.h"

void* fill_node(node_23_t *node, void *v, int(cmp)(void*, void*));
node_23_t *insert23_helper(node_23_t *root, void *v1, int(cmp)(void*, void*), int depth);
node_23_t *search23_helper(node_23_t *root, void *v1, int(cmp)(void*, void*));
int promote(void *val, int(cmp)(void*, void*));
void* checkPromotion(node_23_t *node, void *val);
node_23_t* extend_23_tree(node_23_t *root, void *overflow);
void left_split(node_23_t *parent, node_23_t *child);
void mid1_split(node_23_t *parent, node_23_t *child);
void right_split(node_23_t *parent, node_23_t *child);
void split(node_23_t *parent, node_23_t *child);


tree_23_t *create_23_tree(int (cmp(void*, void*)), size_t size) {
	tree_23_t *temp = (tree_23_t*)malloc(sizeof(tree_23_t));
	temp->root = NULL;
	temp->cmp = cmp;
	temp->size = size;
	return temp;
}

node_23_t *create_23_node(void *v1, void *v2) {
	node_23_t *temp = (node_23_t*)malloc(sizeof(node_23_t));
	temp->v1 = v1;
	temp->v2 = v2;
	temp->left = temp->right = temp->mid1 = temp->mid2 = temp->parent = NULL;
	temp->count = 0;
	temp->count += v1 != NULL ? 1 : 0;
	temp->count += v2 != NULL ? 1 : 0;
	return temp;
}

void print_23_tree(node_23_t *node, int level){
	if (node != NULL) {
		print_23_tree(node->right, level + 1);
		for (int i = 0; i < level; i++) {
			printf("    ");
		}
		if (level > 0) {
			printf("-->");
		}
		printf("(%d", *(int*)node->v1);
		if (node->v2) {
			printf(", %d", *(int*)node->v2);
		}
		printf(")\n");
		print_23_tree(node->mid1, level + 1);
		print_23_tree(node->left, level + 1);
	}
}

node_23_t *searchOn23(tree_23_t* tree, void *v1) {
	return search23_helper(tree->root, v1, tree->cmp);
}

void insertOn23(tree_23_t *tree, void *v1, void *v2) {
	tree->root = insert23_helper(tree->root, v1, tree->cmp, 0);
}

void* fill_node(node_23_t *node, void *v, int(cmp)(void*, void*)) {
	void *temp = NULL;
	if (v == NULL) {
		return NULL;
	}

	if (node->v1 == NULL) {
		node->v1 = v;
		node->count++;
	}
	else if (node->v2 == NULL) {
		if (cmp(v, node->v1) < 0) {
			node->v2 = node->v1;
			node->v1 = v;
		}
		else {
			node->v2 = v;
		}
		node->count++;
	}
	else {
		if (cmp(v, node->v1) < 0) {
			temp = node->v1;
			node->v1 = v;
		}
		else if (cmp(v, node->v2) > 0) {
			temp = node->v2;
			node->v2 = v;
		}
		else {
			temp = v;
		}
	}
	node->overflow = temp;
	return temp;

}

node_23_t *insert23_helper(node_23_t *root, void *v1, int(cmp)(void*, void*), int depth) {
	if (root == NULL) {
		if (depth == 0) {
			return create_23_node(v1, NULL);
		}
		else {
			return NULL;
		}
	}
	if (cmp(v1, root->v1) < 0) {
		root->left = insert23_helper(root->left, v1, cmp, depth + 1);
	}
	else if (root->v2 == NULL || 
			(root->v2 != NULL &&(cmp(v1, root->v2)) > 0)) {
		root->right = insert23_helper(root->right, v1, cmp, depth + 1);
	}
	else if (root->v2 != NULL && cmp(v1, root->v2) < 0) {
		root->mid1 = insert23_helper(root->mid1, v1, cmp, depth + 1);
	}
	else {
		return root;
	}

	// Fill the current node up regardless of space
	// Then if overflow, prmote median value
	v1 = checkPromotion(root, v1);

	if (!v1) {
		return root;
	}

	void* overflow = fill_node(root, v1, cmp);
	if (overflow != NULL) {
		if (root->parent == NULL) {
			root = extend_23_tree(root, overflow);
		}
		else {
			// Splitting the node
			node_23_t *parent = root->parent;
			if (parent->left == root) {
				left_split(parent, root);
			}
			else if (parent->mid1 == root) {
				mid1_split(parent, root);
			}
			else if (parent->right == root) {
				right_split(parent, root);
			}
		}
	}

	return root;

}


void left_split(node_23_t *parent, node_23_t *child) {
	if (parent->mid1) {
		parent->mid2 = parent->mid1;
	}
	parent->mid1 = create_23_node(child->v2, NULL);
	parent->mid1->parent = parent;
	parent->mid1->left = child->mid2;
	parent->mid1->right = child->right;
	if (parent->mid1->left) {
		parent->mid1->left->parent = parent->mid1;
	}
	if (parent->mid1->right) {
		parent->mid1->right->parent = parent->mid1;
	}
	child->right = child->mid1;
	child->mid1 = NULL;
	child->mid2 = NULL;
	
	child->v2 = NULL;
	child->count = 1;
}

void mid1_split(node_23_t *parent, node_23_t *child) {
	parent->mid2 = create_23_node(child->v2, NULL);
	parent->mid2->parent = parent;
	parent->mid2->left = child->mid2;
	parent->mid2->right = child->right;
	if (parent->mid2->left) {
		parent->mid2->left->parent = parent->mid2;
	}
	if (parent->mid2->right) {
		parent->mid2->right->parent = parent->mid2;
	}
	child->right = child->mid1;
	child->mid1 = NULL;
	child->mid2 = NULL;

	child->v2 = NULL;
	child->count = 1;
}

void right_split(node_23_t *parent, node_23_t *child) {
	parent->mid2 = create_23_node(child->v1, NULL);
	parent->mid2->parent = parent;
	parent->mid2->left = child->left;
	parent->mid2->right = child->mid1;
	if (parent->mid2->left) {
		parent->mid2->left->parent = parent->mid2;
	}
	if (parent->mid2->right) {
		parent->mid2->right->parent = parent->mid2;
	}
	child->left = child->mid2;
	child->mid1 = NULL;
	child->mid2 = NULL;

	child->v1 = child->v2;
	child->v2 = NULL;
	printf("Child value = %d\n", *(int*)child->v1);
	child->count = 1;

	if (parent->mid1 == NULL) {
		parent->mid1 = parent->mid2;
		parent->mid2 = NULL;
	}
}

void* checkPromotion(node_23_t *node, void *val) {
	void *temp = NULL;
	if (node->left == NULL && node->right == NULL) {
		temp = val;
	}
	else if (node->left && node->left->overflow) {
		temp =  node->left->overflow;
		node->left->overflow = NULL;
	}
	else if (node->mid1 && node->mid1->overflow) {
		temp = node->mid1->overflow;
		node->mid1->overflow = NULL;
	}
	else if (node->right && node->right->overflow) {
		temp = node->right->overflow;
		node->right->overflow = NULL;
	}
	return temp;
}

node_23_t* extend_23_tree(node_23_t *root, void *overflow) {
	node_23_t *new_root = create_23_node(overflow, NULL);
	new_root->left = root;
	new_root->right = create_23_node(root->v2, NULL);

	new_root->left->parent = new_root;
	new_root->right->parent = new_root;

	new_root->left->left = root->left;
	new_root->right->right = root->right;
	new_root->right->left = root->mid2;
	new_root->left->right = root->mid1;
	
	root->v2 = root->overflow = NULL;
	root->mid1 = root->mid2 = NULL;
	root->count = 1;


	if (new_root->left->left != NULL) {
		new_root->left->left->parent = new_root->left;
	}
	if (new_root->left->right != NULL) {
		new_root->left->right->parent = new_root->left;
	}
	if (new_root->right->left != NULL) {
		new_root->right->left->parent = new_root->right;
	}
	if (new_root->right->right != NULL) {
		new_root->right->right->parent = new_root->right;
	}
	return new_root;
}

node_23_t *search23_helper(node_23_t *root, void *v1, int(cmp)(void*, void*)) {
	if (root == NULL) {
		return NULL;
	}
	else if (root->v1 == v1 || (root->v2 && root->v2 == v1)) {
		return root;
	}
	else if (cmp(v1, root->v1) < 0) {
		return search23_helper(root->left, v1, cmp);
	}
	else if (root->v2 == NULL || (root->v2 != NULL && cmp(v1, root->v2) < 0)) {
		return search23_helper(root->mid1, v1, cmp);
	}
	else {
		return search23_helper(root->right, v1, cmp);
	}
}