/*Napisati program za zbrajanje i mnozenje polinoma. Koeficijenti i eksponenti se
citaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nuzno sortirani.
//slazemo eksponente od manjeg prema vecem
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_OPENNING_FILE -1
#define ERROR_READING_FROM_FILE 1
#define ERROR_ALLOCATING_MEMORY -10
#define BUFFER_SIZE 1024

struct _Postfix;
typedef struct _Postfix* Position;
typedef struct _Postfix
{
	float num;

	Position next;
}Postfix_El;

int readPostfixFromFile(char * filename, Position Postfix);

int calcStack(char operation, Position Postfix);
int pushStack(float num, Position Postfix);
int popStack(Position Postfix);


int main() {

	Postfix_El Head;
	Head.next = NULL;

	int catchError = 0;

	catchError = readPostfixFromFile("postfix.txt", &Head);

	return 0;
}

int readPostfixFromFile(char* filename, Position Postfix) {
	FILE* fp;
	fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("ERROR opening file");
		return ERROR_OPENNING_FILE;
	}

	char * buffer = (char *) malloc (BUFFER_SIZE * sizeof(char));
	if(buffer == NULL){
		return ERROR_ALLOCATING_MEMORY;
	}
	int bufferIndex = 0, isOperator = 0, isNumber = 0, byteSize;
	int catchError = 0;
	float number;
	char operation;

	while (!feof(fp)) {
		fgets(buffer, BUFFER_SIZE, fp);
		while (strlen(buffer) > 0) {
			isNumber = sscanf(buffer, "%f %n", &number, &byteSize);
			isOperator = sscanf(buffer, "%c %n", &operation, &byteSize);

			if (!isNumber) {
				//operator
				catchError = calcStack(operation, Postfix);
				if (catchError == ERROR_READING_FROM_FILE) {
					printf("Wrong file format\n");
					return ERROR_READING_FROM_FILE;
				}
			}
			else {
				catchError = pushStack(number, Postfix);
				if (catchError == ERROR_ALLOCATING_MEMORY) {
					printf("ERROR pushing to stack\n");
					return ERROR_ALLOCATING_MEMORY;
				}
			}
			buffer += byteSize;
			bufferIndex += byteSize;
		}
	}

	printf("Result is %f\n", Postfix->next->num);

	return 0;
}

int popStack(Position Postfix) {
	Position temp = Postfix->next;
	Postfix->next = temp->next;
	free(temp);
}

int calcStack(char operation, Position Postfix) {

	float num1, num2;
	float result;

	num2 = Postfix->next->num;
	num1 = Postfix->next->next->num;

	switch (operation)
	{
	case '+':
		result = num1 + num2;
		popStack(Postfix);
		Postfix->next->num = result;
		break;
	case '-':
		result = num1 - num2;
		popStack(Postfix);
		Postfix->next->num = result;
		break;
	case '*':
		result = num1 * num2;
		popStack(Postfix);
		Postfix->next->num = result;
		break;
	case '/':
		if (!num2) {
			printf("You can't divide with 0\n");
			return ERROR_READING_FROM_FILE;
		}
		result = num1 / num2;
		popStack(Postfix);
		Postfix->next->num = result;
		break;

	default:
		return ERROR_READING_FROM_FILE;
		break;
	}

	return 0;
}
int pushStack(float num, Position Postfix) {
	Position Q = (Position) malloc(sizeof(Postfix_El));

	if (Q == NULL) {
		printf("ERROR allocating memory");
		return ERROR_ALLOCATING_MEMORY;
	}
	Q->num = num;

	Q->next = Postfix->next;
	Postfix->next = Q;

	return 0;
}