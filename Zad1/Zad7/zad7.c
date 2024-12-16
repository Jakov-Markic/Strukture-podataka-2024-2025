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
#include <ctype.h>

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
dirPath moveToDirectory(Directory*, char*);
void removeDirectory(Directory*, char*);
void removeChild(Directory*);
void printDirectory(Directory*);

int main() {

	dirPath currentDir = NULL;
	dirPath temp = NULL;
	dirPath Root = NULL;
	char commandLine[MAX_DIRECTORY_NAME] = "";
	char dirName[MAX_DIRECTORY_NAME] = "";
	char currentPath[MAX_DIRECTORY_NAME] = "";
	char tempPath[MAX_DIRECTORY_NAME] = "";
	char command[10] = "";
	int isExit = 0, pathLength = 0, checkInput = 0;

	Root = (dirPath) malloc(sizeof(Directory));
	if (Root == NULL) {
		printf("Error starting program\n");
		return -1;
	}
	temp = Root;
	Root = createEmptyDirectory(Root, "C:");
	if (Root == NULL) {
		printf("Error starting program\nShuting down\n");
		return -1;
	}
	Root->children[1] = Root;
	free(temp);

	currentDir = Root;
	strcat(currentPath, Root->name);
	pathLength += (int)strlen(Root->name);

	while (isExit == 0) {
		printf("\n%s>", currentPath);
		fgets(commandLine, MAX_DIRECTORY_NAME, stdin);
		checkInput = sscanf(commandLine, "%9s %s\n", command, dirName);

		if (checkInput == 0) {
			printf("Something is wrong with input\n");
			continue;
		}
		for (int i = 0; i < (int)strlen(command); i++) 
			command[i] = tolower(command[i]);

		if (strcmp(command, "dir") == 0) {
			printDirectory(currentDir);
		}
		else if (strcmp(command, "exit") == 0) {
			isExit = 1;
		}
		else if (strcmp(command, "help") == 0) {
			printf("cd - move to directory\n");
			printf("md - create directory\n");
			printf("dir - print directory\n");
			printf("rm - remove directory\n");
			printf("exit - exit program\n");
		}
		else if (strncmp(command, "cd", 2) == 0) {
			if (command[2] == '.') {
				if (command[3] == '.')
					strcpy(dirName, "..");
				else
					strcpy(dirName, ".");
			}
			temp = currentDir;
			currentDir = moveToDirectory(currentDir, dirName);
			if (temp != currentDir) {
				if (strcmp(dirName, "..") == 0) {
					pathLength = pathLength - ((int)strlen(temp->name) + 1);
					strcpy(tempPath, currentPath);
					strncpy(currentPath, tempPath, pathLength);
					currentPath[pathLength] = '\0';
				}
				else{
					strcat(currentPath, "\\");
					strcat(currentPath, dirName);
					pathLength += (int)strlen(dirName) + 1;
				}
			}
		}
		else if (strcmp(command, "md") == 0) {
			temp = createDirectory(currentDir, dirName);
			if (temp == NULL)
				printf("Error creating directory\n");
		}
		else if (strcmp(command, "rm") == 0) {
			removeDirectory(currentDir, dirName);
		}
		else
			printf("Command not recognised\n");
	}
	removeChild(Root);

	return 0;
}

dirPath createEmptyDirectory(Directory* parent, char* name) {
	dirPath dir = (dirPath)malloc(sizeof(Directory));
	if (dir == NULL) {
		printf("Error allocating memory for directory\n");
		return NULL;
	}
	dir->children = (dirPath*)malloc(DEFAULT_CHILDREN_NUM * sizeof(dirPath));
	if (dir->children == NULL) {
		printf("Error allocating memory for children\n");
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
dirPath createDirectory(Directory*dir, char*name) {
	dirPath* tempChildren = dir->children;
	dirPath temp = NULL;
	int i = 0,j = 0;
	if (dir->childAmmount == dir->size) {
		dir->children = (dirPath*)realloc(tempChildren, 2 * dir->size * sizeof(dirPath));
		if (dir->children == NULL) {
			printf("Error allocating memory\n");
			return NULL;
		}
		dir->size *= 2;
		for (i = dir->childAmmount; i < dir->size; i++)
			dir->children[i] = NULL;
	}
	for (i = 2; i < dir->childAmmount; i++) {
		if (strcmp(name, dir->children[i]->name) == 0) {
			printf("Directory already exists, try another name.\n");
			return NULL;
		}
		if (strcmp(name, dir->children[i]->name) < 0) {
			for (j = dir->childAmmount; j > i; j--) {
				dir->children[j] = dir->children[j - 1];
			}
			break;
		}
	}
	dir->children[i] = createEmptyDirectory(dir, name);
	if (dir->children[i] == NULL) { //zatvoriti prazninu koja je napravljena
		for (i; i < dir->childAmmount; i++) 
			dir->children[i] = dir->children[i + 1];
		dir->children[i] = NULL;
		
		return NULL;
	}
	dir->childAmmount++;
	return dir->children[i];

}
dirPath moveToDirectory(Directory* dir, char* name) {
	int i;
	if (strcmp(name, ".") == 0)
		return dir;
	if (strcmp(name, "..") == 0)
		return dir->children[1];

	for (i = 2; i < dir->childAmmount; i++)
		if (strcmp(name, dir->children[i]->name) == 0)
			return dir->children[i];

	printf("Directory not found.\n");
	return dir;
}
void removeDirectory(Directory*dir, char*name) {

	dirPath temp = NULL;
	int i;

	if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
		printf("Directory cannot be removed\n");
		return;
	}

	for (i = 2; i < dir->childAmmount; i++) {
		if (strcmp(name, dir->children[i]->name) == 0) {
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
	dir->childAmmount -= 1;

}
void removeChild(Directory* dir) {
	if (dir->childAmmount != 2) {
		for (int i = 2; i < dir->childAmmount; i++) {
			removeChild(dir->children[i]);
		}
	}
	free(dir->children);
	free(dir);
}
void printDirectory(Directory* dir) {
	printf(".\n");
	printf("..\n");
	for (int i = 2; i < dir->childAmmount; i++)
		printf("%s\n", dir->children[i]->name);
	return;
}
