/*Napisati program koji prvo procita koliko redaka ima datoteka, tj. koliko ima sudenata zapisanih u datoteci.
Nakon toga potrebno je dinamicki alocirati prostor za niz struktura studenata (ime, prezime, bodovi) i 
ucitati iz datoteke sve zapise. Na ekran ispisati ime, prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrzi ime i prezime studenata, te broj bodova na kolokviju.
relativan_br_bodova = br_bodova/max_br_bodova*100  */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define ERROR_OPENING_FILE -1
#define BUFFER_SIZE 1024

typedef struct
{
	char ime[30];
	char prezime[30];
	int bodovi;


}Student;

int count_rows(char* filename);

int main() {

	return 0;
}


int count_rows(char* filename) {
	FILE* fp = NULL;
	int count = 0;
	char buffer[BUFFER_SIZE];
	fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("ERROR opening file\n");
		return ERROR_OPENING_FILE;
	}

	while (!feof(fp))
	{
		fgets(buffer, BUFFER_SIZE, fp);
		//sscanf()
		++count;
	}

	fclose(fp);
	return count;
}
