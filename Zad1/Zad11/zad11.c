/*
Prepraviti zadatak 10 na nacin da se formira hash tablica drzava. Tablica ima 11 mjesta, a
funkcija za preslikavanje kljuc racuna da se zbraja ASCII vrijednost prvih pet slova drzave zatim
racuna ostatak cjelobrojnog dijeljenja te vrijednosti s velicinom tablice. Drzave s istim kljucem se
pohranjuju u vezanu listu sortiranu po nazivu drzave. Svaki cvor vezane liste sadrzi stablo
gradova sortirano po broju stanovnika, zatim po nazivu grada.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define HASH_SIZE 11

#define ERROR_ALLOCATING_MEMORY -1
#define ERROR_OPENNING_FILE -2
#define ERROR_READING_FILE -3
#define ERROR_CREATING_ELEMENT -4

typedef struct _TreeCity {
	char name[BUFFER_SIZE];
	int population;
	struct _TreeCity* left;
	struct _TreeCity* right;
}TreeCity;

typedef struct _Country {
	char name[BUFFER_SIZE];
	TreeCity* cityTree;

	struct _Country* next;
}Country;

Country** readFile(char*);
TreeCity* readFileCity(char*);
int calcHashPos(char*);

Country* createCountry(char*);
TreeCity* createBranch(TreeCity*, char*, int);
TreeCity* createCity(char*, int);

void printDataBase(Country**);
void printTree_inorder(TreeCity*);

void findsCites(Country**, char*, int);
void printCities(TreeCity*, int);

void deleteTree(TreeCity*);
void deleteList(Country*);
void deleteHash(Country**);

int main() {

	Country** hashTable = NULL;
	char buffer[BUFFER_SIZE];
	char name[BUFFER_SIZE];
	int population = 0;
	int isFormat, i;

	hashTable = (Country**)malloc(sizeof(Country*) * HASH_SIZE);
	if (hashTable == NULL) {
		return ERROR_ALLOCATING_MEMORY;
	}
	for (i = 0; i < HASH_SIZE; i++) {
		hashTable[i] = NULL;
	}

	hashTable = readFile("drzave.txt");
	if (hashTable == NULL) {
		return ERROR_ALLOCATING_MEMORY;
	}

	printDataBase(hashTable);

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
			findsCites(hashTable, name, population);
		}
	}
	deleteHash(hashTable);
	return 0;
}

Country** readFile(char* filename) {
	Country** hashTable;
	Country* Node = NULL;
	Country* temp = NULL;
	FILE* fp = NULL;
	char buffer[BUFFER_SIZE];
	char countryName[BUFFER_SIZE];
	char countryFileName[BUFFER_SIZE];
	int isFormat = 0;
	int hashPosition = 0;
	int i;

	hashTable = (Country**)malloc(sizeof(Country*) * HASH_SIZE);
	if (hashTable == NULL) {
		return NULL;
	}
	for (i = 0; i < HASH_SIZE; i++) {
		hashTable[i] = NULL;
	}

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error openning file %d\n", ERROR_OPENNING_FILE);
		return NULL;
	}

	while (!feof(fp)) {
		fgets(buffer, BUFFER_SIZE, fp);
		isFormat = sscanf(buffer, "%s %s\n", countryName, countryFileName);

		if (isFormat != 2) {
			printf("Wrong file format %d\n", ERROR_READING_FILE);
			return NULL;
		}

		Node = createCountry(countryName);
		if (Node == NULL) {
			fclose(fp);
			return NULL;
		}

		Node->cityTree = readFileCity(countryFileName);
		if (Node->cityTree == NULL) {
			printf("Error creating tree %d\n", ERROR_CREATING_ELEMENT);
			fclose(fp);
			return NULL;
		}

		//hash table
		hashPosition = calcHashPos(countryName);
		temp = hashTable[hashPosition];

		//adding to hash
		if (temp == NULL) hashTable[hashPosition] = Node;
		else {
			while (temp->next != NULL && strcmp(Node->name, temp->next->name) > 0) 
				temp = temp->next;
			Node->next = temp->next;
			temp->next = Node;
		}


	}
	fclose(fp);
	return hashTable;
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
		printf("Error opening file %d\n", ERROR_OPENNING_FILE);
		return NULL;
	}
	while (!feof(fp)) {
		fgets(buffer, BUFFER_SIZE, fp);
		isFormat = sscanf(buffer, "%[^0-9] %d", cityName, &population);
		if (isFormat != 2) {
			printf("Wrong file format %d\n", ERROR_READING_FILE);
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


int calcHashPos(char* countryName) {
	int hashValue = 0, i;
	for (i = 0; i < 5; i++) {
		if (countryName[i] == '\0') { //if country name has less that 5 letters
			for (i; i < 5; i++) {
				hashValue += 65;
			}
		}
		else
			hashValue += (int)countryName[i];
	}
	return hashValue % HASH_SIZE;
}

Country* createCountry(char* name) {

	Country* Node = (Country*)malloc(sizeof(Country));
	if (Node == NULL) {
		printf("Error alocating memory %d\n", ERROR_ALLOCATING_MEMORY);
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
		}
		else
			createBranch(root->right, cityName, population);
	}

	return root;
}
TreeCity* createCity(char* cityName, int population) {
	TreeCity* T = (TreeCity*)malloc(sizeof(TreeCity));
	if (T == NULL) {
		printf("Error allocating memory %d\n", ERROR_ALLOCATING_MEMORY);
		return NULL;
	}
	strcpy(T->name, cityName);
	T->population = population;
	T->left = NULL;
	T->right = NULL;

	return T;
}

void printDataBase(Country** hashTable) {
	int i;
	for (i = 0; i < 11; i++) {
		if (hashTable[i] == NULL) {
			continue;
		}
		printf("\n--------------------------------\n");
		printf("%s :\n", hashTable[i]->name);
		printTree_inorder(hashTable[i]->cityTree);
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

void findsCites(Country** hashTable, char* name, int population) {
	int hashPosition = 0, i;
	Country* temp = NULL;
	hashPosition = calcHashPos(name);
	temp = hashTable[hashPosition];

	while (temp != NULL && strcmp(temp->name, name) != 0) {
		temp = temp->next;
	}

	if (temp == NULL) {
		printf("That country doesn't exist here\n");
		return;
	}

	printf("\nYour choise was: %s\n", name);
	printCities(temp->cityTree, population);
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

void deleteHash(Country** hashTable) {
	int i;
	for (i = 0; i < HASH_SIZE; i++) {
		if (hashTable[i] != NULL) {
			deleteList(hashTable[i]);
		}
	}
	free(hashTable);
}