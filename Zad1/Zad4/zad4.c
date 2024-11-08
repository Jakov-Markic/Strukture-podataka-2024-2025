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

struct _Polynomial;
typedef struct _Polynomial* Position;
typedef struct _Polynomial
{
	int coefficient;
	int exponent;

	Position next;	
}Polynom;

int readFile(char *, Position, int);

Position createPolynom(int, int);
int PolynomMultiplication(Position, Position, Position);
int PolynomAddition(Position, Position, Position);

void addPolynomAfter(Position, Position);

void printPolynom(Position);
void checkErrorPrint(int);
void deletePolynom(Position);

int main() {

	Polynom Head1, Head2, HeadResult;
	Head1.next = NULL;
	Head2.next = NULL;
	HeadResult.next = NULL;
	int checker;

	checker = readFile("polinom.txt", &Head1, 0);
	checkErrorPrint(checker);
	printf("Prvi polinom: ");
	printPolynom(Head1.next);

	checker = readFile("polinom.txt", &Head2, 1);
	checkErrorPrint(checker);
	printf("Drugi polinom:");
	printPolynom(Head2.next);
	
	checker = PolynomAddition(Head1.next, Head2.next, &HeadResult);
	checkErrorPrint(checker);
	printf("\nZbrajanje polinoma\n");
	printPolynom(HeadResult.next);

	deletePolynom(&HeadResult);

	checker = PolynomMultiplication(Head1.next, Head2.next, &HeadResult);
	checkErrorPrint(checker);
	printf("\nMnozenje polinoma\n");
	printPolynom(HeadResult.next);

	deletePolynom(&HeadResult);
	deletePolynom(&Head1);
	deletePolynom(&Head2);

	return 0;
}

int readFile(char * filename, Position Polinom, int polinomRow){

	FILE * fp;
	fp = fopen(filename, "r");
	if(fp == NULL){
		return ERROR_OPENNING_FILE;
	}
	Position temp = NULL;
	Position Q = NULL;
	char * buffer = (char *) malloc(BUFFER_SIZE * sizeof(char));
	if (buffer == NULL)
		return ERROR_ALLOCATING_MEMORY;
	int koeficijent = 0;
	int eksponent = 0;
	int byteSize = 0;
	int checker = 0;
	int countRow = 0;

	while (!feof(fp)) {
		do{
			fgets(buffer, BUFFER_SIZE, fp);
			countRow++;
		} while (countRow <= polinomRow);
		while (strlen(buffer) > 0)
		{
			temp = Polinom;
			checker = sscanf(buffer, "%d %d %n", &koeficijent, &eksponent, &byteSize);
			if(checker != 2){
				return ERROR_READING_FROM_FILE;
			}
			Q = createPolynom(koeficijent, eksponent);
			if(Q == NULL)
				return ERROR_ALLOCATING_MEMORY;
			
			while (temp->next != NULL && Q->exponent > temp->next->exponent)
				temp = temp->next;
			
			/* if(temp->next != NULL && Q->exponent < temp->next->exponent){
				addPolynomAfter(Q, temp); 
			} */
			if(temp->next != NULL && Q->exponent == temp->next->exponent){
				temp->next->coefficient += Q->coefficient;
				free(Q);
			}
			else{
				addPolynomAfter(Q, temp); 
			}
			buffer = buffer + byteSize; // sumljivo
		}
		

	}
	fclose(fp);

	return 0;
}

Position createPolynom(int koeficijent, int eksponent){

	Position q = (Position) malloc (sizeof(Polynom));

	if(q == NULL){
		return NULL;
	}

	q->coefficient = koeficijent;
	q->exponent = eksponent;
	q->next = NULL;

	return q;

}
void addPolynomAfter(Position after, Position polinom){
	after->next = polinom->next;
	polinom->next = after;
}
void addPolynomBefore(Position before, Position polinom){

}

int PolynomAddition(Position First, Position Second, Position Result) {
	int koeficijent = 0;

	while (First != NULL && Second != NULL) {
		if (Second->exponent == First->exponent) {
			koeficijent = First->coefficient + Second->coefficient;
			Result->next = createPolynom(koeficijent, Second->exponent);
			if (Result == NULL) {
				return ERROR_ALLOCATING_MEMORY;
			}
			Result = Result->next;
			First = First->next;
			Second = Second->next;
		}
		else if (First->exponent < Second->exponent) {
			Result->next = createPolynom(First->coefficient, First->exponent);
			if (Result == NULL) {
				return ERROR_ALLOCATING_MEMORY;
			}
			Result = Result->next;
			First = First->next;
		}
		else if (Second->exponent < First->exponent) {
			Result->next = createPolynom(Second->coefficient, Second->exponent);
			if (Result == NULL) {
				return ERROR_ALLOCATING_MEMORY;
			}
			Result = Result->next;
			Second = Second->next;
		}
		
		if (First == NULL) {
			while (Second != NULL) {
				Result->next = createPolynom(Second->coefficient, Second->exponent);
				if (Result == NULL) {
					return ERROR_ALLOCATING_MEMORY;
				}
				Result = Result->next;
				Second = Second->next;
			}
		}
		if (Second == NULL) {
			while (First != NULL) {
				Result->next = createPolynom(First->coefficient, First->exponent);
				if (Result == NULL) {
					return ERROR_ALLOCATING_MEMORY;
				}
				Result = Result->next;
				First = First->next;
			}
		}
	}

	return 0;
}
int PolynomMultiplication(Position First, Position Second, Position Result) {
	
	Position temp = NULL;
	Position tempResult = NULL;
	Position Q = NULL;
	int koeficijent = 0;
	int eksponent = 0;
	while (First != NULL) {
		temp = Second;
		while (temp != NULL) {
			koeficijent = First->coefficient * temp->coefficient;
			eksponent = First->exponent + temp->exponent;
			Q = createPolynom(koeficijent, eksponent);
			if (Q == NULL)
				return ERROR_ALLOCATING_MEMORY;

			tempResult = Result;
			while (tempResult->next != NULL && Q->exponent > tempResult->next->exponent)
				tempResult = tempResult->next;
			if (tempResult->next != NULL && Q->exponent == tempResult->next->exponent) {
				tempResult->next->coefficient += Q->coefficient;
				free(Q);
			}
			else {
				addPolynomAfter(Q, tempResult);
			}

			temp = temp->next;
		}
		First = First->next;
	}
	return 0;
}

void printPolynom(Position Polinom){
	while(Polinom != NULL){
		printf("%dx%d ", Polinom->coefficient, Polinom->exponent);
		Polinom = Polinom->next;
	}
	printf("\n");
}

void checkErrorPrint(int check){
	switch (check)
	{
	case ERROR_ALLOCATING_MEMORY:
		printf("ERROR allocating memory for polynom\n");
		break;
	case ERROR_OPENNING_FILE:
		printf("ERROR opening file\n");
		break;
	case ERROR_READING_FROM_FILE:
		printf("ERROR reading from file\n");
		break;
	default:
		return;
		break;
	}
}

void deletePolynom(Position Polinom) {
	Position temp = Polinom->next;
	Position deleteMe = NULL;
	while (temp != NULL) {
		deleteMe = temp;
		temp = temp->next;
		free(deleteMe);
	}
	Polinom->next = NULL;
}