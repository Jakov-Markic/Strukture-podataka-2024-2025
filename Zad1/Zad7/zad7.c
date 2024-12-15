/*
Napisati program koji pomocu vezanih listi (stabala) predstavlja strukturu direktorija.
Omoguciti unos novih direktorija i pod-direktorija, ispis sadrzaja direktorija i
povratak u prethodni direktorij. Tocnije program treba preko menija simulirati
koristenje DOS naredbi: 1- "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIRECTORY_NAME 256
#define ERROR_ALLOCATING_MEMORY -1
#define DEFAULT_CHILDREN_NUM 4

typedef struct _Directory* dirPath;

typedef struct _Directory {
	char name [MAX_DIRECTORY_NAME];
	int childAmmount;
	int size;
	dirPath* children;
}Directory;

dirPath createEmptyDirectory(Directory*, char *);
dirPath createDirectory(Directory*, char*);
void removeDirectory(Directory*, char*);
dirPath moveToDirectory(Directory*, char*);
void removeChild(Directory*);
void printDirectory(Directory*);

void choosenCommand(char*, char *, Directory*);
void exitProgram(Directory*);

void printError(int);
void printPath(Directory*);

int main() {

	Directory root;
	dirPath currentDir;
	char commandLine[MAX_DIRECTORY_NAME];
	char dirName[MAX_DIRECTORY_NAME];
	char command[5];
	int isExit = 0;
	int checkInput = 0;


	strcpy(root.name, "C:");
	root.children = (dirPath*)malloc(DEFAULT_CHILDREN_NUM * sizeof(dirPath));
	root.children[0] = &root;
	root.children[1] = &root;
	root.children[2] = NULL;
	root.children[3] = NULL;
	root.childAmmount = 2;
	root.size = 4;
	currentDir = &root;

	while (!isExit) {

		fgets(commandLine, MAX_DIRECTORY_NAME, stdin);
		checkInput = sscanf(commandLine, "%s %s\n", command, dirName);
		tolower(command);
		if (checkInput == 0) {
			printf("Command not recognised\n");
		}
		else if (checkInput == 1) {
			if (!strcmp(command, "exit"))
				isExit = 1;
			if (!strcmp(command, "help")) {
				printf("cd - move to directory\n");
				printf("md - create directory\n");
				printf("dir - print directory\n");
				printf("rm - remove directory\n");
			}
			if (!strcmp(command, "dir"))
				printDirectory(currentDir);
		}
		else {
			if (!strcmp(command, "md")) {
				createDirectory(currentDir, dirName);
			}
			if (!strcmp(command, "cd"))
				currentDir = moveToDirectory(currentDir, dirName);
			if (!strcmp(command, "rm"))
				removeDirectory(currentDir, dirName);
		}
	}


	return 0;
}

int choosenCommand(char* command, char dirName, Directory * currentDir) {
	if (!strcmp(command, "exit"))
		return 1;
	if (!strcmp(command, "help")) {
		printf("cd - move to directory\n");
		printf("md - create directory\n");
		printf("dir - print directory\n");
		printf("rm - remove directory\n");
	}
	if (!strcmp(command, "dir"))
		printDirectory(currentDir);
	if (!strcmp(command, "md")) {
		createDirectory(currentDir, dirName);
	}
	if (!strcmp(command, "cd"))
		currentDir = moveToDirectory(currentDir, dirName);
	if (!strcmp(command, "rm"))
		removeDirectory(currentDir, dirName);
}

dirPath createDirectory(Directory*dir, char*name) {
	dirPath temp;
	dirPath* tempChildren = dir->children;
	int i,j;
	if (dir->childAmmount == dir->size) {
		dir->children = (dirPath*)realloc(tempChildren, 2 * dir->size * sizeof(dirPath));
		if (dir->children == NULL) {
			printf("Error allocating memory\n");
			return NULL;
		}
		dir->size *= 2;
		for (i = dir->childAmmount; i < dir->size; i++) { //ako ne postavlja realociranu memoriju na null
			dir->children[i] = NULL;
		}
	}
	for (i = 2; dir->children[i] != NULL; i++) {
		if (!strcmp(name, dir->children[i]->name)) {
			printf("Directory already exists, try another name.\n");
			return NULL;
		}
		if (strcmp(name, dir->children[i]->name) < 0) {
			for (j = dir->size; j >= 2; j--)
				if (dir->children[j] != NULL)
					dir->children[j + 1] = dir->children[j];
			break;
		}
	}
	dir->children[i] = createEmptyDirectory(dir, name); // provjerit warning
	if(dir->children[i] == NULL) {
		return NULL;
	}
	dir->childAmmount++;

}
void removeDirectory(Directory*dir, char*name) {

	dirPath temp = NULL;
	int i;
	for (i = 2; i < dir->childAmmount; i++) {
		if (!strcmp(name, dir->children[i]->name)) {
			temp = dir->children[i];
			break;
		}
	}
	if (temp == NULL) {
		printf("Directory not found to be removed\n");
		return;
	}
	removeChild(temp);
	for (i; i < dir->childAmmount; i++) {
		dir->children[i] = dir->children[i + 1];
	}
	dir->children[dir->childAmmount - 1] = NULL;
	dir->childAmmount--;

}
void removeChild(Directory* dir) {
	if (dir->childAmmount != 2) {
		int i;
		for (i = 2; dir->children[i] != NULL; i++) {
			removeChild(dir->children[i]);
			dir->childAmmount--;
		}
	}
	free(dir->children);
	free(dir);
}


dirPath moveToDirectory(Directory*dir, char*name) {
	int i;
	if(!strcmp(name, ".")) {
		return dir;
	}
	if (!strcmp(name, "..")) {
		return dir->children[1];
	}

	for (i = 2; i < dir->childAmmount; i++) {
		if (!strcmp(name, dir->children[i]->name)) {
			return dir->children[i];
		}
	}
	printf("Directory not found.\n");
	return dir;
}
void printDirectory(Directory* dir) {
	printf(".\n");
	printf("..\n");
	for (int i = 2; i < dir->childAmmount; i++) {
		printf("%s\n", dir->children[i]->name);
	}
	return;
}
dirPath createEmptyDirectory(Directory* parent, char* name) {
	dirPath dir = (dirPath)malloc(sizeof(Directory));
	if (dir == NULL) {
		printf("Error allocating memory\n");
		return NULL;
	}
	dir->children = (dirPath*)malloc(DEFAULT_CHILDREN_NUM * sizeof(dirPath));
	if (dir->children == NULL) {
		printf("Error allocating memory\n");
		return NULL;
	}
	strcpy(dir->name, name);
	dir->children[0] = dir;
	dir->children[1] = parent;
	dir->children[2] = NULL;
	dir->children[3] = NULL;

	dir->childAmmount = 2;
	dir->size = 4;

	return dir;
}
void chooseCommand(char* command) {

}

