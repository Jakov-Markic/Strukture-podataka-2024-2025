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
#define STUDENTI_FILE "studenti.txt"
#define MAX_BROJ_BODOVA 50
typedef struct
{
	char* ime;
	char* prezime;
	int bodovi;


}Student;

int count_rows(char* filename);
void get_student_data(Student* s, char* filename);
void print_student(Student* s, char* filename, int rows);

int main() {

	int row_count = count_rows(STUDENTI_FILE);
	Student* s = (Student*)malloc(row_count * sizeof(Student));
	get_student_data(s, &STUDENTI_FILE);
	printf("Ispis %d studenata i njegovih podataka:\n\n", row_count);
	print_student(s, STUDENTI_FILE, row_count);

	return 0;
}

void print_student(Student* s, char* filename, int rows) {
	int i = 0;
	for (i; i < rows; i++) {
		printf("%s %s \n", s[i].ime, s[i].prezime);
		printf("\tApsolutan broj bodova: %d\n", s[i].bodovi);
		printf("\tRelativan broj bodova %f %%\n\n", ((float)s[i].bodovi / MAX_BROJ_BODOVA) * 100);
	}
}

void get_student_data(Student* s, char* filename) {
	FILE* fp = NULL;
	char buffer[BUFFER_SIZE];
	int i = 0;
	fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("ERROR opening file\n");
		return ERROR_OPENING_FILE;
	}

	while (!feof(fp))
	{
		fgets(buffer, BUFFER_SIZE, fp);
		if (buffer[0] != ' ' && buffer[0] != '\n') //provjera ako postoji prazan redak
		{
			//Student ima jedno ime, a ostalo je prezime
			s[i].ime = (char*)malloc(50 * sizeof(char));
			s[i].prezime = (char*)malloc(100 * sizeof(char));
			sscanf(buffer, "%s %[^0-9] %d", s[i].ime, s[i].prezime, &s[i].bodovi);
			++i;
		}

	}


	fclose(fp);
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
		if (buffer[0] != ' ' && buffer[0] != '\n') // provjera ako ima prazan red
			++count;
	}

	fclose(fp);
	return count;
}
