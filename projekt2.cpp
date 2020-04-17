// projekt2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string.h>
typedef struct {
	char krstne[100];
	char priezvisko[100];
}MENO;
typedef struct herec{
	MENO meno;
	int rok;
	herec* dalsi;
}HEREC;
typedef struct film {
	char nazov[100];
	int rok_vyroby;
	MENO reziser;
	HEREC* herci;
	film* dalsi;
}FILM;

FILM* nacitaj(FILM* zaciatok_f, FILE* f) { //funkcia z textoveho suboru nacita filmy a vrati ich zaciatok ako navratovu hodnotu
	char buffer[500],p=0;
	FILM* akt_f = zaciatok_f,*vloz_f=NULL;
	HEREC* vloz_h,*akt_h=NULL;
	HEREC* zac_h = NULL;

	while (fgets(buffer, 500, f) != NULL) {
		if (buffer[0] == '\n') continue; //blblo to...
		if (buffer[0] != '*') { //mam novy film cize idem ho vytvarat
			vloz_f = (FILM*)malloc(sizeof(FILM));
			strncpy(vloz_f->nazov, buffer, 100);
			vloz_f->nazov[strlen(vloz_f->nazov) - 1] = '\0';
			fscanf(f,"%d", &vloz_f->rok_vyroby);
			fscanf(f,"%s %s", vloz_f->reziser.krstne, vloz_f->reziser.priezvisko);
			vloz_f->herci = NULL;
			vloz_f->dalsi = NULL;
			if (zaciatok_f == NULL) {
				zaciatok_f = vloz_f;
				akt_f = vloz_f;
			}
			else {
				akt_f->dalsi = vloz_f;
				akt_f = akt_f->dalsi;
			}
		}
		else { //idem pridavat herca
			vloz_h = (HEREC*)malloc(sizeof(HEREC)); //pomocna premenna, toto je ukazovatel na herca ktoreho budem vkladat
			char trash[3];
			sscanf(buffer, "%s %s %s %d ", trash, vloz_h->meno.krstne, vloz_h->meno.priezvisko, &vloz_h->rok); //nacitam vsetky udaje
			vloz_h->dalsi = NULL;
			if (akt_f->herci == NULL) { //idem vkladat prveho herca
				akt_f->herci = vloz_h; //akt_f->herci ide ukazovat na prveho herca
				akt_h = vloz_h; //aktualny herec je tento prvy
			}
			else {//ak uz tam je nejaky herec
				akt_h->dalsi = vloz_h; //nastavim ukazovatela na dalsieho herca aktualneho herca na noveho herca
				akt_h = akt_h->dalsi; //posuniem aktualneho herca na tohto posledneho
			}
		}
	}
	return zaciatok_f;
}
void vypis(FILM* zaciatok_f) {
	FILM* akt = zaciatok_f;
	
	if (zaciatok_f == NULL) {
		printf("Prazdny zoznam\n");
		return;
	}
	HEREC* akt_h=zaciatok_f->herci;
	while (akt != NULL) {
		printf("%s (%d) %s %s\n Hraju:", akt->nazov, akt->rok_vyroby, akt->reziser.krstne, akt->reziser.priezvisko);
		akt_h = akt->herci;
		while (akt_h != NULL) {
			printf(" %s %s (%d)", akt_h->meno.krstne, akt_h->meno.priezvisko, akt_h->rok);
			akt_h = akt_h->dalsi; //posuniem aktualneho na dalsieho
			if (akt_h != NULL) putchar(','); //iba aby za poslednym nebola ciarka
		}
		putchar('\n');
		akt = akt->dalsi;
	}
}
int main()
{
	FILE* f;
	FILM* zaciatok_filmov=NULL;
	f = fopen("filmy.txt", "r");
	if (f == NULL) {
		printf("Subor sa nepodarilo otvorit\n");
		return -1;
	}
	zaciatok_filmov = nacitaj(zaciatok_filmov, f);
	vypis(zaciatok_filmov);

}
