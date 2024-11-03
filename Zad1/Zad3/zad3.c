/*
Definirati strukturu osoba (ime, prezime, godina rodenja) i napisati program koji:
A. dinamicki dodaje novi element na pocetak liste,
B. ispisuje listu,
C. dinamicki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. brise odredeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.

Prethodnom zadatku dodati funkcije:
A. dinamicki dodaje novi element iza odredenog elementa,
B. dinamicki dodaje novi element ispred odredenog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. cita listu iz datoteke.
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_CREATING_PERSON 10
#define NAME_LENGHT 30
#define SURNAME_LENGHT 50

struct _Osoba;
typedef struct _Osoba* Position;

typedef struct _Osoba
{
    char* ime;
    char* prezime;
    int godina_rodjenja;

    Position next;
}Osoba;

Position create_Person(char* ime, char* prezime, int godina);
int newPerson_Beginning(char* ime, char* prezime, int godina, Position Person);
int newPerson_End(char* ime, char* prezime, int godina, Position Person);
Position findPerson_bySurname(char* prezime, Position Person);
void deletePerson_BySurname(char* prezime, Position Person);

int newPerson_BeforeP(char * osobaIme, char * ime, char * prezime, int godina, Position Person);
int newPerson_AfterP(char * osobaIme, char* ime, char* prezime, int godina, Position Person);
void sortPerson(Position Person);
void readFile_Person(Position Person);
void writeFile_Person(Position Person);

void printPerson(Position Person);

int main() {

    Osoba Head;
    Head.next = NULL;
    int checkError = 0;
    Osoba* checkOsoba = NULL;

    checkError = newPerson_Beginning("Mate", "Matic", 2000, &Head);
    if (checkError == ERROR_CREATING_PERSON) {
        printf("Problem sa stvaranjem osobe\n");
    }

    checkError = newPerson_Beginning("Ante", "Antic", 2002, &Head);
    if (checkError == ERROR_CREATING_PERSON) {
        printf("Problem sa stvaranjem osobe\n");
    }

    checkError = newPerson_End("Ivo", "Ivic", 1999, &Head);
    if (checkError == ERROR_CREATING_PERSON) {
        printf("Problem sa stvaranjem osobe\n");
    }

    printPerson(Head.next);

    //checkOsoba ako zelimo nesto raditi s tim podacima
    checkOsoba = findPerson_bySurname("Antic", Head.next);

    checkOsoba = findPerson_bySurname("Antonovic", Head.next);

    deletePerson_BySurname("Ivic", Head.next);
    deletePerson_BySurname("Antonovic", Head.next);

    printPerson(Head.next);



    return 0;
}

Position create_Person(char* ime, char* prezime, int godina) {
    Position q = (Position)malloc(sizeof(Osoba));
    if (q == NULL) {
        printf("\nNeuspjesna kreacija osobe.\n");
        return NULL;
    }
    q->ime = (char*)malloc(NAME_LENGHT * sizeof(char));
    if (q->ime == NULL) {
        printf("\nNuspjesna upis imena.\n");
        return NULL;
    }
    strcpy(q->ime, ime);

    q->prezime = (char*)malloc(SURNAME_LENGHT * sizeof(char));
    if (q->prezime == NULL) {
        printf("\nNuspjesna upis prezimena.\n");
        return NULL;
    }
    strcpy(q->prezime, prezime);

    q->godina_rodjenja = godina;

    return q;
}

int newPerson_Beginning(char* ime, char* prezime, int godina, Position Person) {

    Position q = create_Person(ime, prezime, godina);
    if (q == NULL) {
        return ERROR_CREATING_PERSON;
    }

    q->next = Person->next;
    Person->next = q;

    return 0;
}
int newPerson_End(char* ime, char* prezime, int godina, Position Person) {
    Position q = create_Person(ime, prezime, godina);
    if (q == NULL) {
        return ERROR_CREATING_PERSON;
    }

    while (Person->next != NULL)
    {
        Person = Person->next;
    }

    q->next = Person->next;
    Person->next = q;

}

int newPerson_BeforeP(char* osobaIme, char* ime, char* prezime, int godina, Position Person) {

}

int newPerson_AfterP(char* osobaIme, char* ime, char* prezime, int godina, Position Person) {

}

Position findPerson_bySurname(char* prezime, Position Person) {
    while (Person != NULL && strcmp(Person->prezime, prezime) != 0)
        Person = Person->next;
    if (Person != NULL)
        printf("\nOsoba s %s prezimenom postoji\n", prezime);
    else
        printf("\nOsoba s %s prezimenom ne postoji  :( \n", prezime);
    return Person;


}
void deletePerson_BySurname(char* prezime, Position Person) {
    Position p = findPerson_bySurname(prezime, Person);
    if (p == NULL) {
        printf("\nOsoba s %s prezimenom nema za izbrisati\n", prezime);
        return;
    }
    while (Person->next != p)
    {
        Person = Person->next;
    }

    Person->next = p->next;
    free(p->ime);
    free(p->prezime);
    free(p);

    return;
}

void printPerson(Position Person) {

    while (Person != NULL) {
        printf("\nIme: %s\n", Person->ime);
        printf("Prezime: %s\n", Person->prezime);
        printf("Godine rodjenja: %d\n", Person->godina_rodjenja);

        Person = Person->next;
    }
}