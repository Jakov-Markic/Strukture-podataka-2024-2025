/*Napisati program koji omogucava rad s binarnim stablom pretrazivanja. Treba
omoguciti unosenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
level order), brisanje i pronalazenje nekog elementa.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define ERROR_ALLOCATING_MEMORY -1

typedef struct _Tree{
	int value;
	struct _Tree* left;
	struct _Tree* right;
}Tree;

void print_inorder(Tree *);
void print_preorder(Tree *); // something suspicios
void print_postorder(Tree *);
void print_levelorder(Tree *);
void deleteTree(Tree *);
Tree* deleteEl(Tree *, int);
void findEl(Tree*, int);
void createBranch(Tree*, int);
Tree* createEl(int);
Tree* findMin(Tree*);
void printChoise();

int main() {

	int choise;
	int value;
	int isValid;
	Tree * root = NULL;
	printf("Radimo stabla:");
	while (1) {
		printChoise();
		isValid = scanf(" %d", &choise);
		if (!isValid) {
			printf("Izlaz iz programa\nAdio\n");
			deleteTree(root);
			return 0;
		}

		switch (choise)
		{
		case 1:
			printf("Vrijednost\t");
			isValid = scanf(" %d", &value);
			if(!isValid){
				printf("Not valid value\n");
				break;
			}
			if(root == NULL)
				root = createEl(value);
			else
				createBranch(root, value);

			break;
		case 2:
			printf("Vrijednost\t");
			isValid = scanf(" %d", &value);
			if (!isValid) {
				printf("Not valid value\n");
				break;
			}
			deleteEl(root, value);

			break;
		case 3:
			print_inorder(root);
			printf("\n");
			break;
		case 4:
			print_preorder(root);
			printf("\n");
			break;
		case 5:
			print_postorder(root);
			printf("\n");
			break;
		case 6:
			print_levelorder(root);
			printf("\n");
			break;
		case 7:
			printf("Vrijednost \t");
			isValid = scanf(" %d", &value);
			if (!isValid) {
				printf("Not valid value\n");
				break;
			}
			findEl(root, value);
			break;
		
		default:
			printf("Izlaz iz programa\nAdio\n");
			deleteTree(root);
			return 0;
			break;
		}
	}


	return 0;
}

void createBranch(Tree* root, int value) {
	if (value > root->value) {
		if (root->right == NULL) {
			root->right = createEl(value);
			return;
		}
		createBranch(root->right, value);
	}
	else if (value < root->value) {
		if (root->left == NULL) {
			root->left = createEl(value);
			return;
		}
		createBranch(root->left, value);
	}
	else{
		printf("Value already exists in tree\n");
		return;
	}
	return;
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

void deleteTree(Tree*root) {
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
	}else
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
void print_postorder(Tree* root){
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
void printChoise() {
	printf("\n--------------------------\n");
	printf("Imas sljedece opcije:\n");
	printf("1 - dodavanje elementa\n");
	printf("2 - brisanje elementa\n");
	printf("3 - inorder ispis\n");
	printf("4 - preorder ispis\n");
	printf("5 - postorder ispis\n");
	printf("6 - levelorder ispis\n");
	printf("7 - pronalazenje elementa\n");

	printf("X - izlaz iz programa\n");
	printf("--------------------------\n");
	printf("Vas odabir je:\t");
}
