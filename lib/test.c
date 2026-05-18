#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "AVL.h"
//#include "BST.h"


int compare(void *a, void *b);
void print_tree(node_t *node);
void test_bst();
void test_avl();

int main() {
	//test_bst();
	test_avl();
	return 0;
}

int compare(void *a, void *b) {
	return (*(int*)a - *(int*)b);
}

void print_tree(node_t *node) {
	if (node != NULL) {
		print_tree(node->left);
		printf("%d ", *(int*)node->v1);
		print_tree(node->right);
	}
}

void print_sideway(node_t *node, int level) {
	if (node != NULL) {
		print_sideway(node->right, level + 1);
		for (int i = 0; i < level; i++) {
			printf("    ");
		}
		if (level > 0) {
			printf("-->");
		}
		printf("%d\n", *(int*)node->v1);
		print_sideway(node->left, level + 1);
	}
}

// void test_bst() {
// 	tree_t *tree = create_tree(compare, sizeof(int));
// 	int a = 5;
// 	int b = 3;
// 	int c = 7;
// 	insert_node(tree, &a);
// 	insert_node(tree, &b);
// 	insert_node(tree, &c);
// 	print_sideway(tree->root, 0);

// 	node_t *result = search(tree, &b);
// 	if (result != NULL) {
// 		printf("Found: %d\n", *(int*)result->v1);
// 	} else {
// 		printf("Not found\n");
// 	}

// 	delete_node(tree, &b);
// 	print_sideway(tree->root, 0);
// 	result = search(tree, &b);
// 	if (result != NULL) {
// 		printf("Found: %d\n", *(int*)result->v1);
// 	} else {
// 		printf("Not found\n");
// 	}
// }

void test_avl() {
	tree_t *tree = create_tree(compare, sizeof(int));
	int a = 5;
	int b = 3;
	int c = 7;
	int d = 2;
	int e = 1;
	int f = 6;
	int g = 8;
	int h = 9;
	int i = 10;
	int j = 4;
	insertOnAVL(tree, &a);
	insertOnAVL(tree, &b);
	insertOnAVL(tree, &c);
	insertOnAVL(tree, &d);
	insertOnAVL(tree, &e);
	insertOnAVL(tree, &f);
	insertOnAVL(tree, &g);
	insertOnAVL(tree, &h);
	insertOnAVL(tree, &i);
	insertOnAVL(tree, &j);
	print_sideway(tree->root, 0);

	node_t *result = searchOnAVL(tree, &b);
	if (result != NULL) {
		printf("Found: %d\n", *(int*)result->v1);
	} else {
		printf("Not found\n");
	}

	deleteOnAVL(tree, &b);
	print_sideway(tree->root, 0);
	result = searchOnAVL(tree, &b);
	if (result != NULL) {
		printf("Found: %d\n", *(int*)result->v1);
	} else {
		printf("Not found\n");
	}

	deleteOnAVL(tree, &j);
	insertOnAVL(tree, &(int){11});
	print_sideway(tree->root, 0);
	printf("\n");
	deleteOnAVL(tree, &(int){1});
	print_sideway(tree->root, 0);
	printf("\n");
}