#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#define _CRT_SECURE_NO_WARNINGS 
#pragma warning(disable : 4996)


typedef struct Node{
int data;
struct Node* left;
struct Node* right;
}Node;

Node* read_tree(Node* root, int data) {
	if (root == NULL){
		root = (Node*)malloc(sizeof(Node));
		root->left = NULL;
		root->right = NULL;
		root->data = data;
	}
	else {
		if (data < root->data){
			root->left = read_tree(root->left, data);
		}
		else {
			root->right = read_tree(root->right, data);
		}
	}
	return root;
}

void print_inner_descending(Node* root) {
	if (root != NULL) {
		if (root->left == NULL && root->right == NULL) {
			return;
		}
		print_inner_descending(root->left); 
		printf("%d ", root->data);
		print_inner_descending(root->right); 
	}
}

void purge(Node* root) {
	if (root) {
		if (root->left != NULL) {
			purge(root->left);
		}
		if (root->right != NULL) {
			purge(root->right);
		}
		free(root);
	}
}


int main() {

	FILE* input = fopen("input.txt", "r");
	if (!input) {
		exit(0);
	}

	int data;

	Node* root = NULL;

	while (fscanf(input, "%d ", &data)){
		root = read_tree(root, data);
	}

	print_inner_descending(root);

	fclose(input);
	purge(root);
	return 0;
}