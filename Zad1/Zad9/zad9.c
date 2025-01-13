/*Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u cvorove binarnog stabla.
* 
a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraca
pokazivac na korijen stabla.

b) Napisati funkciju replace koja ce svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat ce se u stablo na
slici Slika 2.

c) Prepraviti program na nacin da umjesto predefiniranog cjelobrojnog polja koristenjem
funkcije rand() generira slucajne brojeve u rasponu <10, 90>. Takoder, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon koristenja funkcije iz a), zatim b)
dijela zadatka.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ERROR_ALLOCATING_MEMORY -1

typedef struct _Tree {
	int value; 
	struct _Tree* left;
	struct _Tree* right;
}Tree;

void print_inorder(Tree*);
void print_preorder(Tree*);
void print_postorder(Tree*);
void print_levelorder(Tree*);
void print_inorderFile(Tree*, FILE*);

void deleteTree(Tree*);
Tree* deleteEl(Tree*, int);

void findEl(Tree*, int);
Tree* findMin(Tree*);

Tree* createBranch(Tree*, int);
Tree* createEl(int);
Tree* insert(int [], int);
int replace(Tree*);

int main() {

	srand(time(NULL));

	Tree* root = NULL;
	Tree* rootB = NULL;
	int array [] = {2, 5, 7, 8, 11, 1, 4, 2, 3, 7};
	int len = sizeof(array) / sizeof(int);
	int i, value;
	root = insert(array, len);
	printf("Level order a) & b):\n");
	print_levelorder(root);
	replace(root);
	printf("\n----------------\n");
	print_levelorder(root);
	printf("\n----------------\n");

	for (i = 0; i < 4; i++) {
		value = (rand() % (90 - 10 + 1)) + 10;
		rootB = createBranch(rootB, value);
	}

	FILE* fp = NULL;
	fp = fopen("randomTree.txt", "w");
	if (fp == NULL) {
		printf("Error opening file\n");
		return -1;
	}
	printf("Level order random tree in file:");

	print_inorderFile(rootB, fp);
	printf("\n----------------\n");
	print_levelorder(rootB);

	replace(rootB);

	fprintf(fp, "\n\n");
	printf("\n----------------\n");
	print_levelorder(rootB);
	print_inorderFile(rootB, fp);

	fclose(fp);
	deleteTree(root);
	deleteTree(rootB);

	return 0;
}
void print_inorderFile(Tree* root, FILE* file) {
	if (root == NULL) {
		return;
	}
	print_inorderFile(root->left, file);
	fprintf(file, "%d ", root->value);
	print_inorderFile(root->right, file);
}
int replace(Tree* root) {
	if (root == NULL) return 0;
	int currentvalue = root->value;
	root->value = replace(root->left) + replace(root->right);
	return currentvalue + root->value;

}
Tree * insert(int array[], int len) {
	int i;
	Tree* root = NULL;
	for (i = 0; i < len; i++) {
		root = createBranch(root, array[i]);
	}
	return root;
};

Tree* createBranch(Tree* root, int value) {
	if (root == NULL) {
		root = createEl(value);
	}
	else if (value < root->value) {
		if (root->right == NULL) {
			root->right = createEl(value);
		}else
		createBranch(root->right, value);
	}
	else if (value >= root->value) {
		if (root->left == NULL) {
			root->left = createEl(value);
		}else
		createBranch(root->left, value);
	}
	return root;
}
Tree* createEl(int value) {
	Tree* T = (Tree*)malloc(sizeof(Tree));
	if (T == NULL) {
		printf("Error allocating memory");
		return NULL;
	}
	T->value = value;
	T->left = NULL;
	T->right = NULL;

	return T;
}

void deleteTree(Tree* root) {
	if (root == NULL) {
		return;
	}
	if (root->left != NULL) {
		deleteTree(root->left);
	}
	if (root->right != NULL) {
		deleteTree(root->right);
	}
	free(root);
	return;
}
Tree* findMin(Tree* root) {
	if (root == NULL) {
		return NULL;
	}
	if (root->left == NULL) {
		return root;
	}
	else
		findMin(root->left);
}
Tree* deleteEl(Tree* root, int value) {
	if (root == NULL) {
		printf("Element does't exist\n");
	}
	else if (value < root->value) {
		root->left = deleteEl(root->left, value);
	}
	else if (value > root->value) {
		root->right = deleteEl(root->right, value);
	}
	else if (root->left != NULL && root->right != NULL) {
		Tree* temp = findMin(root->right);
		root->value = temp->value;
		root->right = deleteEl(root->right, root->value);
	}
	else {
		Tree* temp = root;
		root = (root->left == NULL) ? root->right : root->left;
		free(temp);
	}
	return root;
}

void print_levelorder(Tree* root) {
	//left is element to print, right is next in row
	Tree rowHead;
	Tree* temp = NULL;
	Tree* temp2 = NULL;
	Tree* last = NULL;
	rowHead.left = NULL;
	rowHead.right = NULL;

	Tree* rowEl = createEl(0);
	if (rowEl == NULL) {
		printf("Error allocating memory\n");
		return;
	}
	rowHead.right = rowEl;
	temp2 = rowHead.right;
	rowEl->left = root;
	while (rowHead.right != NULL) {

		printf("%d ", temp2->left->value);
		temp = temp2;

		if (temp->left->left != NULL) {
			rowEl = createEl(0);
			if (rowEl == NULL) {
				printf("Error allocating memory\n");
				return;
			}
			rowEl->left = temp->left->left;
			//add to begining
			rowEl->right = rowHead.right;
			rowHead.right = rowEl;


		}
		if (temp->left->right != NULL) {
			rowEl = createEl(0);
			if (rowEl == NULL) {
				printf("Error allocating memory\n");
				return;
			}

			rowEl->left = temp->left->right;

			//add to begining
			rowEl->right = rowHead.right;
			rowHead.right = rowEl;
		}

		temp = rowHead.right;
		temp2 = &rowHead;
		while (temp->right != NULL) {

			temp = temp->right;
			temp2 = temp2->right;
		}
		free(temp);
		temp2->right = NULL;

	}


}
void print_preorder(Tree* root) {
	if (root == NULL) {
		return;
	}
	printf("%d ", root->value);
	print_inorder(root->left);
	print_inorder(root->right);
}
void print_postorder(Tree* root) {
	if (root == NULL) {
		return;
	}
	print_inorder(root->left);
	print_inorder(root->right);
	printf("%d ", root->value);
}
void print_inorder(Tree* root) {
	if (root == NULL) {
		return;
	}
	print_inorder(root->left);
	printf("%d ", root->value);
	print_inorder(root->right);
}
void findEl(Tree* root, int value) {
	if (root == NULL) {
		printf("Value not found\n");
		return;
	}
	if (value < root->value) {
		findEl(root->left, value);
	}
	else if (value > root->value) {
		findEl(root->right, value);
	}
	else {
		printf("Element is in the tree\n");
		return;
	}
}

