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
#include <ctype.h>

#define ERROR_CREATING_PERSON 10
#define ERROR_FIND_PERSON 15
#define ERROR_OPENING_FILE -1

#define NAME_LENGHT 30
#define SURNAME_LENGHT 50

#define BUFFER_SIZE 1024

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
int newPerson_BeforeP(char* findIme, char* ime, char* prezime, int godina, Position Person);
int newPerson_AfterP(char* findIme, char* ime, char* prezime, int godina, Position Person);

Position findPerson_bySurname(char* prezime, Position Person);
Position findPreviousPerson_bySurname(char* prezime, Position Person);
void deletePerson_BySurname(char* prezime, Position Person);

void sortPerson_bySurname(Position Person);

Position readFile_Person(char * filename);
int checkifLineEmpty(char* line);
int writeFile_Person(Position Person);

void printPerson(Position Person);
void freePerson(Position Person);

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
    printf("\n------------------------\n");

    //checkOsoba ako zelimo nesto raditi s tim podacima
    checkOsoba = findPerson_bySurname("Antic", Head.next);

    checkOsoba = findPerson_bySurname("Antonovic", Head.next);

    deletePerson_BySurname("Ivic", Head.next);
    deletePerson_BySurname("Antonovic", Head.next);

    printPerson(Head.next);
    printf("\n------------------------\n");

    newPerson_AfterP("Antic", "Antun", "Antunovic", 1995, &Head);
    printPerson(Head.next);
    printf("\n------------------------\n");

    newPerson_BeforeP("Antic", "Stipe", "Stipic", 1988, &Head);
    newPerson_BeforeP("Antic", "Luka", "Lukic", 1977, &Head);
    printPerson(Head.next);

    printf("\n------------------------\n");

    printf("\nSortiranje liste\n");
    sortPerson_bySurname(&Head);
    printPerson(Head.next);

    freePerson(&Head);

    writeFile_Person(Head.next);
    Head.next = readFile_Person("studenti.txt");
    printf("\n------------------------\n");
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
    q->next = NULL;

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

int newPerson_BeforeP(char* findIme, char* ime, char* prezime, int godina, Position Person) {
    Person = findPreviousPerson_bySurname(findIme, Person);

    if (Person == NULL) {
        return ERROR_FIND_PERSON;
    }

    Position q = create_Person(ime, prezime, godina);
    if (q == NULL) {
        return ERROR_CREATING_PERSON;
    }
    q->next = Person->next;
    Person->next = q;
}

int newPerson_AfterP(char* findIme, char* ime, char* prezime, int godina, Position Person) {
    Person = findPerson_bySurname(findIme, Person->next);
    if (Person == NULL) {
        return ERROR_FIND_PERSON;
    }
    Position q = create_Person(ime, prezime, godina);
    if (q == NULL) {
        return ERROR_CREATING_PERSON;
    }
    q->next = Person->next;
    Person->next = q;

}

Position findPreviousPerson_bySurname(char* prezime, Position Person) {
    while (Person != NULL && strcmp(Person->next->prezime, prezime) != 0)
        Person = Person->next;
    if (Person == NULL)
        printf("\nOsoba s %s prezimenom ne postoji  :( \n", prezime);

    return Person;
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
void sortPerson_bySurname(Position Person) {

    //nije lipo a ni optimizirano ali radi

    Position temp1 = Person;
    Position temp2 = Person->next;
    Position temp3 = Person->next->next;
    Position end = NULL;
    Position temp = NULL;
    while (end != Person->next->next) {
        temp1 = Person;
        temp2 = Person->next;
        temp3 = Person->next->next;
        temp = NULL;
        while (temp2->next != end) {
            if (strcmp(temp2->prezime, temp3->prezime) > 0) {
                temp = temp2;
                temp2->next = temp3->next;
                temp3->next = temp;
                temp1->next = temp3;

                temp1 = temp1->next;
                temp3 = temp2->next;
            }
            else {
                temp1 = temp1->next;
                temp2 = temp2->next;
                temp3 = temp3->next;
            }


        }
        end = temp2;
    }

}
void freePerson(Position Person) {
    Position temp = Person->next;
    Position deleteMe = NULL;
    while (temp != NULL) {
        deleteMe = temp;
        temp = temp->next;
        free(deleteMe->ime);
        free(deleteMe->prezime);
        free(deleteMe);
    }
        Person->next = NULL;
}
Position readFile_Person(char * filename) {
    FILE* fp = NULL;

    Position q = NULL;
    Position temp = NULL;
    Position tempHead = NULL;
    Position tempPrevious = NULL;

    char buffer[BUFFER_SIZE];
    char ime[NAME_LENGHT];
    char prezime[SURNAME_LENGHT];
    int godina;

    int checkInput = 0;
    int isFirstCreated = 1;
    int isBefore = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("ERROR opening a file\n");
        return NULL;
    }

    while (!feof(fp)) {
        fgets(buffer, BUFFER_SIZE, fp);
        if (!checkifLineEmpty(buffer)) {
            checkInput = sscanf(buffer, "%s %[^0-9] %d", ime, prezime, &godina);

            if (checkInput != 3) {
                printf("ERROR reading file");
                return NULL;
            }
            if (isFirstCreated) {
                q = create_Person(ime, prezime, godina);
                tempHead = q;
                isFirstCreated = 0;
                tempPrevious = q;
            }
            else {
                temp = tempHead;
                isBefore = 0;
                while (!isBefore && temp != NULL) {
                    if (strcmp(prezime, temp->prezime) < 0) {
                        q = create_Person(ime, prezime, godina);
                        q->next = temp;
                        if (temp == tempHead)
                            tempHead = q;
                        isBefore = 1;
                    }
                    temp = temp->next;
                    
                }
                if (!isBefore) {
                    newPerson_End(ime, prezime, godina, tempHead);
                }
            }
        }
    }
    return tempHead;
}
int checkifLineEmpty(char * line)
{
    char* ch = line;
    int is_blank = -1;

    for (ch; *ch != '\0' && is_blank != 0; ch++)
    {
        if (!isspace(*ch))
            is_blank = 0;   
    }

    return is_blank;
}

int writeFile_Person(Position Person) {
    FILE* fp = NULL;
    
    fp = fopen("Osobe.txt", "w");
    if (fp == NULL) {
        printf("ERROR opening a file\n");
        return ERROR_OPENING_FILE;
    }
    while (Person != NULL) {
        fprintf_s(fp, "%-10s\t%-20s\t%d\n", Person->ime, Person->prezime, Person->godina_rodjenja);
        Person = Person->next;
    }
}