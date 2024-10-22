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

typedef struct Osoba
{
    char * ime;
    char * prezime;
    int godina_rodjenja;

    struct Osoba *next;
}Osoba;

Osoba * create_Person(char * ime, char * prezime, int godina);
int newPerson_Beginning(char * ime, char * prezime, int godina, Osoba * Person);
int newPerson_End(char * ime, char * prezime, int godina, Osoba * Person);
Osoba * findPerson_bySurname(char * prezime, Osoba * Person);
Osoba * findPrevious(char * prezime, Osoba * Preson);
void deletePerson(char * prezime, Osoba * Person);

void printPerson(Osoba * Person);

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
    checkOsoba = findPerson_bySurname("Antic", Head.next);
    
    checkOsoba = findPerson_bySurname("Antonovic", Head.next);

    deletePerson("Ivic", &Head);
    deletePerson("Antonovic", &Head);

    printPerson(Head.next);

    
    
    return 0;
}

Osoba * create_Person(char * ime, char * prezime, int godina){
    Osoba * q;
    q = (Osoba *) malloc (sizeof(Osoba));
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
    if(q->ime == NULL){
        printf("\nNuspjesna upis prezimena.\n");
        return NULL;
    }
    q->prezime = prezime;

    q->godina_rodjenja = godina;
    if(q->ime == NULL){
        printf("\nNuspjesna upis godine.\n");
        return NULL;
    }
    return q;
}

int newPerson_Beginning(char * ime, char * prezime, int godina, Osoba * Person){

    Osoba * q = create_Person(ime, prezime, godina);
    if(q == NULL){
        return ERROR_CREATING_PERSON;
    }
    

    q->next = Person->next;
    Person->next = q;

    return 0;
}
int newPerson_End(char * ime, char * prezime, int godina, Osoba * Person){
    Osoba * q = create_Person(ime, prezime, godina);
    if(q == NULL){
        return ERROR_CREATING_PERSON;
    }
    Osoba * temp = Person; // fix by removing temp
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    q->next = temp->next;
    temp->next = q;
    
}

Osoba * findPerson_bySurname (char * prezime, Osoba * Person){
    Osoba * p = Person;
    while (p != NULL && strcmp(p->prezime, prezime) != 0)
        p = p->next;
    if (p != NULL)
        printf("Osoba s %s prezimenom postoji\n", prezime);
    else
        printf("Osoba s %s prezimenom ne postoji  :( \n", prezime);
    return p;
    
    
}
void deletePerson(char * prezime, Osoba * Person){
    Osoba * p = findPerson_bySurname(prezime, Person);
    if(p == NULL){
        printf("Osoba s %s prezimenom nema za izbrisati\n", prezime);
        return;
    }
    Osoba * q = Person;
    while (q->next != p)
    {
        q = q->next;
    }

    q->next = p->next;
    /* free(p->ime);
    free(p->prezime);
    free(p->next); */
    free(p);

    
    


    return;
}

void printPerson(Osoba * Person){

    Osoba * p = Person;
    while(p != NULL){
        printf("Ime: %s\n", p->ime);
        printf("Prezime: %s\n", p->prezime);
        printf("Godine rodjenja: %d\n\n", p->godina_rodjenja);

        p = p->next;
    }
}
