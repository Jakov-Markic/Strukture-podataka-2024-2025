/*Napisati program koji cita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih drzava. Uz
ime drzave u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
drzave. Svaka datoteka koja predstavlja drzavu sadrzi popis gradova u formatu naziv_grada,
broj_stanovnika.

a) Potrebno je formirati sortiranu vezanu listu drzava po nazivu. Svaki cvor vezane liste
sadrzi stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.

b) Potrebno je formirati stablo drzava sortirano po nazivu. Svaki cvor stabla sadrzi vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.

Nakon formiranja podataka potrebno je ispisati drzave i gradove te omoguciti korisniku putem tipkovnice
pretragu gradova odredene drzave koji imaju broj stanovnika veci od unosa na tipkovnici.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define ERROR_ALLOCATING_MEMORY -1
#define ERROR_OPENNING_FILE -2
#define ERROR_READING_FILE -3

//zad a)

typedef struct _TreeCity {
	char name[BUFFER_SIZE];
	int population;
	struct _TreeCity* left;
	struct _TreeCity* right;
}TreeCity;

typedef struct _Country {
	char name[BUFFER_SIZE];
	TreeCity* cityTree;

	struct _Country * next;
}Country; 

Country* readFile(char *);
TreeCity* readFileCity(char*);

Country* createCountry(char*);
TreeCity* createBranch(TreeCity*, char*, int);
TreeCity* createCity(char*, int);

void printDataBase(Country*);
void printTree_inorder(TreeCity*);

void findsCites(Country*, char*, int);
void printCities(TreeCity*, int);

void deleteTree(TreeCity*);
void deleteList(Country*);

int main() {

	Country Head;
	char buffer[BUFFER_SIZE];
	char name[BUFFER_SIZE];
	int population = 0;
	int isFormat;
	Head.next = readFile("drzave.txt");
	if (Head.next == NULL) {
		return -1;
	}

	printDataBase(Head.next);

	while (1) {
		printf("\nUpisi ime drzave pa broj stanovnika ");
		printf("\n(izlaz iz programa je negativna populacija)");
		printf("\n(Format: imeDrzave BrojStanovnika) :");
		fgets(buffer, BUFFER_SIZE, stdin);
		isFormat = sscanf(buffer, "%s %d\n", name, &population);
		if (isFormat != 2) {
			printf("Wrong input\n");
			continue;
		}
		if (population <= 0) {
			break;
		}
		else {
			findsCites(Head.next, name, population);
		}
	}
	deleteList(&Head);

	return 0;
}

Country* readFile(char* filename) {
	Country Head;
	Country* Node = NULL;
	Country* temp = NULL;
	FILE* fp = NULL;
	char buffer[BUFFER_SIZE];
	char countryName[BUFFER_SIZE];
	char countryFileName[BUFFER_SIZE];
	int isFormat = 0;

	Head.next = NULL;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error openning file %d\n", ERROR_OPENNING_FILE);
		return NULL;
	}

	while (!feof(fp)) {
		temp = &Head;
		fgets(buffer, BUFFER_SIZE, fp);
		isFormat = sscanf(buffer, "%[^0-9] %s\n", countryName, countryFileName);

		if (isFormat != 2) {
			printf("Wrong file format\n");
			return NULL;
		}

		Node = createCountry(countryName);
		if (Node == NULL) {
			fclose(fp);
			return NULL;
		}
		//adding to list
		while (temp->next != NULL && strcmp(Node->name, temp->next->name) > 0)
			temp = temp->next;
		Node->next = temp->next;
		temp->next = Node;

		Node->cityTree = readFileCity(countryFileName);
		if (Node->cityTree == NULL) {
			printf("Error creating tree\n");
			return NULL;
		}

	}
	fclose(fp);
	return Head.next;
}
TreeCity* readFileCity(char* filename) {
	TreeCity* Root = NULL;
	TreeCity* temp = NULL;
	TreeCity* Node = NULL;
	FILE* fp = NULL;
	char buffer[BUFFER_SIZE];
	char cityName[BUFFER_SIZE];
	int population = 0;
	int isFormat = 0;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error opening file\n");
		return NULL;
	}
	while (!feof(fp)) {
		fgets(buffer, BUFFER_SIZE, fp);
		isFormat = sscanf(buffer, "%[^0-9] %d", cityName, &population);
		if (isFormat != 2) {
			printf("Wrong file format\n");
			return NULL;
		}
		Root = createBranch(Root, cityName, population);
		if (Root == NULL) {
			return NULL;
		}
	}
	fclose(fp);
	return Root;
}

Country* createCountry(char* name) {

	Country * Node = (Country*)malloc(sizeof(Country));
	if (Node == NULL) {
		printf("Error alocating memory\n");
		return NULL;
	}
	Node->next = NULL;
	Node->cityTree = NULL;
	strcpy(Node->name, name);
	return Node;
}
TreeCity* createBranch(TreeCity* root, char* cityName, int population) {
	if (root == NULL) {
		root = createCity(cityName, population);
	}
	else if (population <= root->population) {
		if (root->left == NULL) {
			root->left = createCity(cityName, population);
		}
		else if (root->left->population == population) {
			if (strcmp(cityName, root->name) < 0) {
				TreeCity* temp = createCity(cityName, population);
				temp->left = root->left;
				root->left = temp;
			}
			else
				createBranch(root->left, cityName, population);
		}
		else
		createBranch(root->left, cityName, population);
	}
	else if (population > root->population) {
		if (root->right == NULL) {
			root->right = createCity(cityName, population);
		}else
		createBranch(root->right, cityName, population);
	}

	return root;
}
TreeCity* createCity(char* cityName, int population) {
	TreeCity* T = (TreeCity*)malloc(sizeof(TreeCity));
	if (T == NULL) {
		printf("Error allocating memory\n");
		return NULL;
	}
	strcpy(T->name, cityName);
	T->population = population;
	T->left = NULL;
	T->right = NULL;

	return T;
}

void printDataBase(Country* Head) {

	while (Head != NULL) {
		printf("\n--------------------------------\n");
		printf("%s :\n", Head->name);
		printTree_inorder(Head->cityTree);
		Head = Head->next;
	}
}
void printTree_inorder(TreeCity* root) {
	if (root == NULL) {
		return root;
	}
	printTree_inorder(root->left);
	printf("%-30s - %d\n", root->name, root->population);
	printTree_inorder(root->right);
}

void findsCites(Country* Head, char* name, int population) {
	while (Head != NULL && strcmp(Head->name, name) != 0)
		Head = Head->next;

	if (Head == NULL) {
		printf("\nThat country doesn't exit here\n");
		return;
	}

	printf("\nYour choise was: %s\n", name);
	printCities(Head->cityTree, population);
	return;
}
void printCities(TreeCity* root, int population) {
	if (root == NULL)
		return;
	if (root->population >= population) {
		printCities(root->left, population);
		printf("%-30s %d\n", root->name, root->population);
		printCities(root->right, population);
	}
	return;
}

void deleteTree(TreeCity* root) {
	if (root == NULL) return;
	deleteTree(root->left);
	deleteTree(root->right);
	free(root);
	return;
}
void deleteList(Country* Head) {
	Country* temp = Head->next;
	while (temp != NULL) {
		Head->next = temp->next;
		deleteTree(temp->cityTree);
		free(temp);
		temp = Head->next;
	}
}