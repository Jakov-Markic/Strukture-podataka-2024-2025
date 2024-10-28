/*
Definirati strukturu osoba (ime, prezime, godina rođenja) i napisati program koji:
A. dinamički dodaje novi element na početak liste,
B. ispisuje listu,
C. dinamički dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_CREATING_PERSON 10
#define NAME_LENGHT 30
#define SURNAME_LENGHT 50

struct _Osoba;
typedef struct _Osoba * Position;

typedef struct _Osoba
{
    char * ime;
    char * prezime;
    int godina_rodjenja;

    Position next;
}Osoba;

Position create_Person(char * ime, char * prezime, int godina);
int newPerson_Beginning(char * ime, char * prezime, int godina, Position Person);
int newPerson_End(char * ime, char * prezime, int godina, Position Person);
Position findPerson_bySurname(char * prezime, Position Person);
Position findPrevious(char * prezime, Position Preson);

void deletePerson_BySurname(char * prezime, Position Person);

void printPerson(Position Person);

int main(){

    Osoba Head;
    Head.next = NULL;
    int checkError = 0;
    Osoba * checkOsoba = NULL;
    
    checkError = newPerson_Beginning("Mate", "Matic", 2000, &Head);
    if(checkError == ERROR_CREATING_PERSON){
        printf("Problem sa stvaranjem osobe\n");
    }

    checkError = newPerson_Beginning("Ante", "Antic", 2002, &Head);
    if(checkError == ERROR_CREATING_PERSON){
        printf("Problem sa stvaranjem osobe\n");
    }

    newPerson_End("Ivo", "Ivic", 1999, &Head);

    printPerson(Head.next);

    //checkOsoba ako zelimo nesto raditi s tim podacima
    checkOsoba = findPerson_bySurname("Antic", Head.next);
    
    checkOsoba = findPerson_bySurname("Antonovic", Head.next);

    deletePerson_BySurname("Ivic", &Head);
    deletePerson_BySurname("Antonovic", &Head);

    printPerson(Head.next);

    
    
    return 0;
}

Position create_Person(char * ime, char * prezime, int godina){
    Position q = (Position) malloc (sizeof(Osoba));
    if(q == NULL){
        printf("\nNeuspjesna kreacija osobe.\n");
        return NULL;
    }
    q->ime = (char *)malloc(NAME_LENGHT * sizeof(char));
    if(q->ime == NULL){
        printf("\nNuspjesna upis imena.\n");
        return NULL;
    }
    q->ime = ime;
    
    q->prezime = (char *)malloc(SURNAME_LENGHT * sizeof(char));
    if(q->prezime == NULL){
        printf("\nNuspjesna upis prezimena.\n");
        return NULL;
    }
    q->prezime = prezime;

    q->godina_rodjenja = godina;

    return q;
}

int newPerson_Beginning(char * ime, char * prezime, int godina, Position Person){

    Position q = create_Person(ime, prezime, godina);
    if(q == NULL){
        return ERROR_CREATING_PERSON;
    }

    q->next = Person->next;
    Person->next = q;

    return 0;
}
int newPerson_End(char * ime, char * prezime, int godina, Position Person){
    Position q = create_Person(ime, prezime, godina);
    if(q == NULL){
        return ERROR_CREATING_PERSON;
    }

    while (Person->next != NULL)
    {
        Person = Person->next;
    }

    q->next = Person->next;
    Person->next = q;
    
}

Position findPerson_bySurname (char * prezime, Position Person){
    while (Person != NULL && strcmp(Person->prezime, prezime) != 0)
        Person = Person->next;
    if (Person != NULL)
        printf("\nOsoba s %s prezimenom postoji\n", prezime);
    else
        printf("\nOsoba s %s prezimenom ne postoji  :( \n", prezime);
    return Person;
    
    
}
void deletePerson_BySurname(char * prezime, Position Person){
    Position p = findPerson_bySurname(prezime, Person);
    if(p == NULL){
        printf("\nOsoba s %s prezimenom nema za izbrisati\n", prezime);
        return;
    }
    while (Person->next != p)
    {
        Person = Person->next;
    }

    Person->next = p->next;
    free(p);

    return;
}

void printPerson(Position Person){

    while(Person != NULL){
        printf("\nIme: %s\n", Person->ime);
        printf("Prezime: %s\n", Person->prezime);
        printf("Godine rodjenja: %d\n", Person->godina_rodjenja);

        Person = Person->next;
    }
}
