/*Napisati program koji cita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini racun. Na pocetku svake datoteke je zapisan datum u kojem vremenu je
racun izdat u formatu YYYY-MM-DD. Svaki sljedeci red u datoteci predstavlja artikl u formatu
naziv, kolicina, cijena. Potrebno je formirati vezanu listu racuna sortiranu po datumu. Svaki cvor
vezane liste sadrzava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omoguciti upit kojim ce korisnik saznati koliko je novaca sveukupno potroseno na specificni
artikl u odredenom vremenskom razdoblju i u kojoj je kolicini isti kupljen.*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define DATE_SIZE 11

#define ERROR_OPENNING_FILE -1
#define ERROR_ALLOCATING_MEMORY -2
#define NOT_VALID_RECEIPT -10
#define ERROR_FINDING_ARTICLE -20
#define ERROR_INVALID_DATE -30

struct _Article;
typedef struct _Article* ArticleNext;
typedef struct _Article {
	char name[BUFFER_SIZE];
	int amount;
	float price;

	ArticleNext next;
}Article; //gud

struct _Receipt;
typedef struct _Receipt* nextReceipt;
typedef struct _Receipt {

	char date[DATE_SIZE];
	Article ArticleHead;
	nextReceipt next;
}Receipt; //gud

int readFile(char *, nextReceipt);
nextReceipt readReceipt(char*);

void sortInputReceipt(nextReceipt, nextReceipt);
void sortInputArticle(ArticleNext, ArticleNext); 

nextReceipt combineReceipt(nextReceipt, nextReceipt);

ArticleNext createArticle(char *, int, float); 

int findArticle(char *, char *, char *, nextReceipt);
void searchReceipt(char*, int*, float*, nextReceipt);
int checkDate(int, int, int);

void printError(int); 
void printAllRecipts(nextReceipt);

void deleteAllReceipt(nextReceipt);
void deleteRecipt(nextReceipt);

int main() {

	Receipt ReceiptHead;
	ReceiptHead.next = NULL;
	ReceiptHead.ArticleHead.next = NULL;
	strcpy(ReceiptHead.date, "0");
	readFile("racuni.txt", &ReceiptHead);


	if (ReceiptHead.next == NULL) {
		printf("No valid receipts found");
		return -1;
	}

	int gudInput = 0, wantToRun = 0;
	char date1[DATE_SIZE];
	char date2[DATE_SIZE];
	char item[BUFFER_SIZE];

	printf("Dobar dan, dobro dosli u mali ducan.\n");
	printf("Imate sljedece opcije:\n");
	printf("1 - ispisi sve racune koji postoje\n");
	printf("2 - trazi artikal u udredjenom datumu\n");
	printf("X (bilo sta drugo) - gasimo program\n");
	printf("Vasa opcija je: ");
	scanf("%d", &wantToRun);

	while (wantToRun == 1 || wantToRun == 2) {

		if (wantToRun == 1) {
			printAllRecipts(ReceiptHead.next);
		}
		else {
			do {
				printf("Upisi datume YYYY-MM-DD: \n");
				printf("Od: ");
				scanf(" %s", date1);
				printf("Do: ");
				scanf(" %s", date2);
				printf("Element za nac: ");
				scanf(" %s", item);
				gudInput = findArticle(date1, date2, item, &ReceiptHead);

			} while (gudInput != 0);
		}

		printf("Vasa opcija je: ");
		scanf("%d", &wantToRun);
	}
	printf("\nHvala na dolaski u mali ducan\nAdio\n");

	deleteAllReceipt(&ReceiptHead);
	return 0;
}


int readFile(char* filename, nextReceipt ReceiptHead) {

	FILE* fp;
	fp = fopen("racuni.txt", "r");
	if (fp == NULL) {
		printError(ERROR_OPENNING_FILE);
		return ERROR_OPENNING_FILE;
	}

	char buffer[BUFFER_SIZE];
	char receiptName[BUFFER_SIZE];

	while (!feof(fp)) {
		fgets(buffer, BUFFER_SIZE, fp);
		(void)sscanf(buffer, "%[^\n]", receiptName);
		nextReceipt R = NULL;
		R = readReceipt(receiptName);
		if (R == NULL) {
			return ERROR_OPENNING_FILE;
		}
		(void)sortInputReceipt(ReceiptHead, R);
	}
	fclose(fp);
	return 0;
}
nextReceipt readReceipt(char* filename) {
	FILE* fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printError(ERROR_OPENNING_FILE);
		return NULL;
	}

	nextReceipt R = NULL;
	R = (nextReceipt)malloc(sizeof(Receipt));
	if (R == NULL) {
		printError(ERROR_ALLOCATING_MEMORY);
		return NULL;
	}
	R->next = NULL;
	R->ArticleHead.next = NULL;

	ArticleNext Artc = NULL;
	char buffer[BUFFER_SIZE];
	int amount, year, month, day;
	int checkInput = 0;
	float price;
	char name[BUFFER_SIZE];

	fgets(buffer, BUFFER_SIZE, fp);
	checkInput = sscanf(buffer, "%d-%d-%d", &year, &month, &day);
	if (checkDate(year, month, day) == ERROR_INVALID_DATE || checkInput != 3) {
		printError(ERROR_INVALID_DATE);
		return NULL;
	}
	strcpy(R->date, buffer);
	R->date[DATE_SIZE - 1] = '\0';

	while (!feof(fp)) {
		fgets(buffer, BUFFER_SIZE, fp);
		(void)sscanf(buffer, "%s %d %f", name, &amount, &price);
		Artc = createArticle(name, amount, price);
		if (Artc == NULL) {
			return NULL;
		}
		sortInputArticle(&(R->ArticleHead), Artc);
	}

	fclose(fp);
	return R;
}

void sortInputReceipt(nextReceipt Head, nextReceipt R) {
	nextReceipt Result = NULL;
	nextReceipt temp = NULL;
	while (Head->next != NULL && strcmp(R->date, Head->next->date) > 0)
		Head = Head->next;
	if (Head->next != NULL && strcmp(Head->next->date, R->date) == 0) {
		Result = combineReceipt(Head->next, R);
		if (Result == NULL) {
			printError(ERROR_ALLOCATING_MEMORY);
			return;
		}
		Result->next = Head->next;
		Head->next = Result;
		temp = Result->next;
		Result->next = Result->next->next;
		deleteRecipt(R);
		deleteRecipt(temp);
	}
	else {
		R->next = Head->next;
		Head->next = R;
	}
}
void sortInputArticle(ArticleNext Head, ArticleNext Art) {
	while (Head->next != NULL && strcmp(Art->name, Head->next->name) > 0)
		Head = Head->next;
	if (Head->next != NULL && strcmp(Art->name, Head->next->name) == 0) {
		Head->next->amount += Art->amount;
	}
	else {
		Art->next = Head->next;
		Head->next = Art;
	}
}


nextReceipt combineReceipt(nextReceipt FirstHead, nextReceipt SecondHead) {

	ArticleNext FirstArticle = FirstHead->ArticleHead.next;
	ArticleNext SecondArticle = SecondHead->ArticleHead.next;
	nextReceipt Result = NULL;
	ArticleNext ResultArticle = NULL;
	Result = (nextReceipt)malloc(sizeof(Receipt));
	if (Result == NULL) {
		return NULL;
	}
	Result->next = NULL;
	strcpy(Result->date, FirstHead->date);
	Result->ArticleHead.next = NULL;
	ResultArticle = &Result->ArticleHead;
	ArticleNext articleCopy = NULL;


	while (FirstArticle != NULL && SecondArticle != NULL) {
		if (strcmp(FirstArticle->name, SecondArticle->name) < 0) {
			articleCopy = createArticle(FirstArticle->name, FirstArticle->amount, FirstArticle->price);
			FirstArticle = FirstArticle->next;
		}
		else if (strcmp(FirstArticle->name, SecondArticle->name) > 0) {
			articleCopy = createArticle(SecondArticle->name, SecondArticle->amount, SecondArticle->price);
			SecondArticle = SecondArticle->next;
		}
		else {
			articleCopy = createArticle(FirstArticle->name, FirstArticle->amount + SecondArticle->amount, FirstArticle->price);
			FirstArticle = FirstArticle->next;
			SecondArticle = SecondArticle->next;
		}
		if (articleCopy == NULL) {
			return NULL;
		}

		ResultArticle->next = articleCopy;
		ResultArticle = ResultArticle->next;
		if (FirstArticle == NULL) {
			while (SecondArticle != NULL) {
				articleCopy = createArticle(SecondArticle->name, SecondArticle->amount, SecondArticle->price);
				ResultArticle->next = articleCopy;
				ResultArticle = ResultArticle->next;
				SecondArticle = SecondArticle->next;
			}
		}

		if (SecondArticle == NULL) {
			while (FirstArticle != NULL) {
				articleCopy = createArticle(FirstArticle->name, FirstArticle->amount, FirstArticle->price);
				ResultArticle->next = articleCopy;
				ResultArticle = ResultArticle->next;
				FirstArticle = FirstArticle->next;
			}
		}

	}
	return Result;
}
ArticleNext createArticle(char* name, int amount, float price) {
	ArticleNext Q = NULL;
	Q = (ArticleNext)malloc(sizeof(Article));
	if (Q == NULL) {
		printError(ERROR_ALLOCATING_MEMORY);
		return NULL;
	}
	strcpy(Q->name, name);
	Q->amount = amount;
	Q->price = price;
	Q->next = NULL;

	return Q;
}

int findArticle(char* date1, char* date2, char* item, nextReceipt ReceiptHead) {

	nextReceipt lower = ReceiptHead->next;
	nextReceipt high = NULL;

	int ammount = 0;
	float price = 0;

	if (strcmp(date2, date1) < 0) {
		printError(ERROR_INVALID_DATE);
		return ERROR_INVALID_DATE;
	}

	while (ReceiptHead->next != NULL) {
		if (strcmp(date1, ReceiptHead->next->date) >= 0) {
			lower = ReceiptHead->next;
			if (strcmp(date1, lower->date) > 0 && strcmp(date1, lower->next->date) < 0) {
				lower = lower->next;
			}
		}
		if (strcmp(date2, ReceiptHead->next->date) >= 0) {
			high = ReceiptHead->next;
		}
		ReceiptHead = ReceiptHead->next;
	}

	if (lower == ReceiptHead) {
		printError(ERROR_FINDING_ARTICLE);
		return ERROR_FINDING_ARTICLE;
	}

	if (high == NULL) {
		printError(ERROR_FINDING_ARTICLE);
		return ERROR_FINDING_ARTICLE;
	}

	while (lower != high->next) {

		searchReceipt(item, &ammount, &price, lower);

		lower = lower->next;
	}

	if (ammount == 0) {
		printf("Nismo nasli trazeni element :(\n\n");
		return 0;
	}

	printf("\nTrazeni element %s :\n", item);
	printf("Kolicina: %d\n", ammount);
	printf("Cijena: %f\n", price);
	printf("Ukupno: %f\n\n", ammount * price);


	return 0;
}
void searchReceipt(char* articleName, int* ammount, float* price, nextReceipt fReceipt) {

	ArticleNext findMe = fReceipt->ArticleHead.next;

	while (findMe != NULL) {

		if (strcmp(findMe->name, articleName) == 0) {
			*ammount += findMe->amount;
			*price = findMe->price;
			break;
		}

		findMe = findMe->next;
	}

}
int checkDate(int year, int month, int day) {
	if (year < 0 || year >= 10000) {
		printError(ERROR_INVALID_DATE);
		return ERROR_INVALID_DATE;
	}
	if (month < 0 || month > 12) {
		printError(ERROR_INVALID_DATE);
		return ERROR_INVALID_DATE;
	}
	if (day < 0 || day > 31) {
		printError(ERROR_INVALID_DATE);
		return ERROR_INVALID_DATE;
	}
	if (month == 2 && day > 29) {
		printError(ERROR_INVALID_DATE);
		return ERROR_INVALID_DATE;
	}
	if (month == 2 && day == 29 && (year % 4 != 0)) {
		printError(ERROR_INVALID_DATE);
		return ERROR_INVALID_DATE;
	}
	if (month == 2 && day == 29 && (year % 100 == 0) && (year % 400 != 0)) { //mozak
		printError(ERROR_INVALID_DATE);
		return ERROR_INVALID_DATE;
	}

	return 0;
}

void printError(int a) {
	switch (a)
	{
	case ERROR_ALLOCATING_MEMORY:
		printf("ERROR alocating memory\n");
		break;
	case ERROR_OPENNING_FILE:
		printf("ERROR openning file\n");
		break;

	case NOT_VALID_RECEIPT:
		printf("NOT valid receipt\n");
		break;
	case ERROR_FINDING_ARTICLE:
		printf("ERROR finding receipt\n");
		break;
	case ERROR_INVALID_DATE:
		printf("ERROR invalid dates\n");
		break;

	default:
		printf("");
		break;
	}
}
void printAllRecipts(nextReceipt ReceiptBeg) {
	ArticleNext temp = NULL;
	while (ReceiptBeg != NULL) {

		printf("\nDate: %s\n", ReceiptBeg->date);
		temp = ReceiptBeg->ArticleHead.next;
		while (temp != NULL) {
			printf("%-20s %d %f\n", temp->name, temp->amount, temp->price);
			temp = temp->next;
		}
		printf("----------------------\n");

		ReceiptBeg = ReceiptBeg->next;
	}

}

void deleteAllReceipt(nextReceipt ReceiptHead) {
	nextReceipt temp = ReceiptHead->next;
	while (temp != NULL) {
		ReceiptHead->next = temp->next;
		deleteRecipt(temp);
		temp = ReceiptHead->next;
	}
}
void deleteRecipt(nextReceipt deleteMe) {
	ArticleNext temp1 = &deleteMe->ArticleHead;
	ArticleNext temp2 = deleteMe->ArticleHead.next;
	while (temp2 != NULL) {
		temp1->next = temp2->next;
		free(temp2);
		temp2 = temp1->next;
	}
	free(deleteMe);
}