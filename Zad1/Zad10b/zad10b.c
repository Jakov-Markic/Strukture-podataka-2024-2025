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

//zad b)

struct _City;
typedef struct _City* CityNext;
typedef struct _City {
	char name[BUFFER_SIZE];
	int population;

	CityNext next;
}City;

typedef struct _TreeCountry {
	char name[BUFFER_SIZE];
	City* cities;
	struct _TreeCity* left;
	struct _TreeCity* right;
}TreeCountry;


TreeCountry* readFile(char*);
City* readFileCity(char*); //gud

TreeCountry* createCountry(char*, char *);
TreeCountry* createBranch(TreeCountry*, char*, char*);
City* createCity(char*, int); //gud

void printDataBase(TreeCountry*);
void printCityList(City*);

void findsCites(TreeCountry*, char*, int);
void printCities(City*, int);

int main() {

	TreeCountry* Head = NULL;
	char buffer[BUFFER_SIZE];
	char name[BUFFER_SIZE];
	int population = 0;
	int isFormat;
	Head = readFile("drzave.txt");
	if (Head == NULL) {
		return -1;
	}

	printDataBase(Head);

	while (1) {
		printf("\nUpisi ime drzave pa broj stanovnika ");
		printf("\n(izlaz iz programa je negativna populacija)");
		printf("\n(Format: imeDrzave BrojStanovnika) :");
		fgets(buffer, BUFFER_SIZE, stdin);
		isFormat = sscanf(buffer, "%s %d\n", name, &population);
		if (isFormat != 2) {
			printf("Wrong input\n");
		}
		if (population <= 0) {
			return 0;
		}
		findsCites(Head, name, population);
	}
	return 0;
}

TreeCountry* readFile(char* filename) {
	TreeCountry * Head;
	TreeCountry* Node = NULL;
	TreeCountry* temp = NULL;
	FILE* fp = NULL;
	char buffer[BUFFER_SIZE];
	char countryName[BUFFER_SIZE];
	char countryFileName[BUFFER_SIZE];
	int isFormat = 0;

	Head = NULL;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error openning file %d\n", ERROR_OPENNING_FILE);
		return NULL;
	}

	while (!feof(fp)) {
		fgets(buffer, BUFFER_SIZE, fp);
		isFormat = sscanf(buffer, "%s %s\n", countryName, countryFileName);

		if (isFormat != 2) {
			printf("Wrong file format\n");
			return NULL;
		}


		Head = createBranch(Head, countryName, countryFileName);

	}
	fclose(fp);
	return Head;
}
City* readFileCity(char* filename) {
	City Head;
	City* temp = NULL;
	City* Node = NULL;
	FILE* fp = NULL;
	char buffer[BUFFER_SIZE];
	char cityName[BUFFER_SIZE];
	int population = 0;
	int isFormat = 0;
	Head.next = NULL;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error opening file\n");
		return NULL;
	}
	while (!feof(fp)) {
		temp = &Head;
		fgets(buffer, BUFFER_SIZE, fp);
		isFormat = sscanf(buffer, "%[^0-9] %d", cityName, &population);
		if (isFormat != 2) {
			printf("Wrong file format\n");
			return NULL;
		}
		Node = createCity(cityName, population);
		if (Node == NULL) {
			fclose(fp);
			return NULL;
		}

		while (temp->next != NULL && temp->next->population > population)
			temp = temp->next;
		Node->next = temp->next;
		temp->next = Node;

	}

	return Head.next;
}
TreeCountry* createCountry(char* name, char * filename) {

	TreeCountry* Node = (TreeCountry*)malloc(sizeof(TreeCountry));
	if (Node == NULL) {
		printf("Error alocating memory\n");
		return NULL;
	}
	Node->left = NULL;
	Node->right = NULL;
	strcpy(Node->name, name);
	Node->cities = readFileCity(filename);
	if (Node->cities == NULL) {
		printf("Error creating cities\n");
		free(Node);
		return NULL;
	}

	return Node;
}
TreeCountry* createBranch(TreeCountry* root, char * countryName, char * filename) {
	if (root == NULL) {
		root = createCountry(countryName, filename);
		return root;
	}
	else if (strcmp(countryName, root->name) < 0) {
		if (root->left == NULL) {
			root->left = createCountry(countryName, filename);
			return root;
		}
		createBranch(root->left, countryName, filename);
	}
	else if (strcmp(countryName, root->name) > 0) {
		if (root->right == NULL) {
			root->right = createCountry(countryName, filename);
			return root;
		}
		createBranch(root->right, countryName, filename);
	}
	return root;
}
City* createCity(char* cityName, int population) {
	City* T = (City*)malloc(sizeof(City));
	if (T == NULL) {
		printf("Error allocating memory\n");
		return NULL;
	}
	strcpy(T->name, cityName);
	T->population = population;
	T->next = NULL;

	return T;
}

void printDataBase(TreeCountry* root) {
	if (root == NULL) {
		return root;
	}
	printf("\n------------------------\n");
	printf("%-30s\n", root->name);
	printCityList(root->cities);
	printDataBase(root->left);
	printDataBase(root->right);
}
void printCityList(City* Head) {
	while (Head != NULL) {
		printf("%-30s %d\n", Head->name, Head->population);
		Head = Head->next;
	}
	return;
}


void findsCites(TreeCountry* root, char* name, int population) {
	
	if (strcmp(name, root->name) < 0) {
		findsCites(root->left, name, population);
	}
	else if(strcmp(name, root->name) > 0){
		findsCites(root->right, name, population);
	}
	else if(strcmp(name, root->name) == 0){
		printf("\nYour choise was : %s\n", name);
		printCities(root->cities, population);
	}
	else {
		printf("That country doesn't exits here\n");
		return;
	}

	return;
}
void printCities(City* Head, int population) {
	
	while (Head != NULL && Head->population > population) {
		printf("%-30s %d\n", Head->name, Head->population);
		Head = Head->next;
	}
}
