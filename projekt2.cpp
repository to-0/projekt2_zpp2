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
FILM* pridaj(FILM* zac_f) { //prida na koniec zoznamu
	FILM *akt_f = zac_f;
	FILM* vloz_f = (FILM*)malloc(sizeof(FILM));
	char buffer[500];
	FILE* f = stdin;
	if(fgets(buffer, 500, f) != NULL) { //precitam cely riadok do bufferu
		strncpy(vloz_f->nazov, buffer, 100); 
		vloz_f->nazov[strlen(vloz_f->nazov) - 1] = '\0';
		fscanf(f, "%d", &vloz_f->rok_vyroby);
		fscanf(f, "%s %s ", vloz_f->reziser.krstne,vloz_f->reziser.priezvisko); //nacitam meno a priezvisko rezisera
		while (akt_f->dalsi != NULL) { //musim prejst az na koniec
			akt_f = akt_f->dalsi;
		}
		vloz_f->herci = NULL;
		vloz_f->dalsi = NULL;
		akt_f->dalsi = vloz_f; //nastavim ukazovatel na novy film
		akt_f = akt_f->dalsi; //posuniem sa na aktualne posledny film
		//Idem riesit pridavanie hereckeho obsadenia
		HEREC* akt_h;
		akt_h = vloz_f->herci;//na zaciatku to bude NULL
		while (fgets(buffer, 500, f) != NULL && buffer[0]!='*') { //citam hercov po riadkoch az pokial nenajdem *
			HEREC* vloz_h = (HEREC*)malloc(sizeof(HEREC));
			sscanf(buffer, " %s %s %d ", vloz_h->meno.krstne, vloz_h->meno.priezvisko, &vloz_h->rok);
			vloz_h->dalsi=NULL;
			if (akt_f->herci == NULL) { //ak citam prveho herca
				akt_f->herci = vloz_h; //nastacim ukazovatel vo filme na prveho herca
				akt_h = vloz_h; //aktualny herec je ten co som teraz vlozil
			}
			else { //ak uz tam je nejaky herec
				akt_h->dalsi = vloz_h;  //
				akt_h = akt_h->dalsi;
			}
			//printf("I am while loop with buffer: %s", buffer);
		}
		
	}
	return zac_f;
}
void vymaz(FILM** zac,char nazov[]) {
	if (*zac == NULL) {
		printf("Nie je co mazat\n");
		return;
	}
	FILM* akt,*p;
	akt = *zac;
	if (!(strcmp((*zac)->nazov, nazov))) { //chcem vymazat prvy film, zmeni sa zaciatok
		if (akt->dalsi != NULL) { //pokial to neni iba 1 prvkove
			akt = akt->dalsi;
			free(*zac);
			*zac = akt;
		}
		else {
			free(*zac);
			*zac = NULL;
		}
	}
	while (akt != NULL && (strcmp(akt->nazov, nazov))) { //prechadzam pokial nenajdem ten aktualny
		p = akt; //pred aktualnym
		akt = akt->dalsi;
	}
	p->dalsi = akt->dalsi;
	free(akt);

}
void ukonci(FILM** zac_f) {
	FILM *akt_f = *zac_f;
	HEREC *akt_h;
	HEREC* p_h;
	FILM* p;
	if (*zac_f == NULL) {
		return;
	}
	while (akt_f != NULL) {
		p = akt_f; //pomocna premenna tuto pomocou nej budeme uvolnovat pamat
		akt_h = akt_f->herci;
		while (akt_h != NULL) {
			p_h= akt_h;
			akt_h = akt_h->dalsi;
			free(p_h); //postupne mazem vzdy toho predosleho herca az kym dojdem na koniec
		}
		//teraz mozem vymazat aj film a posunut sa na dalsi
		akt_f = akt_f->dalsi;
		free(p);
	}
	
}
int main()
{
	FILE* f, *k=stdin;
	FILM* zaciatok_filmov=NULL;

	f = fopen("filmy.txt", "r");
	if (f == NULL) {
		printf("Subor sa nepodarilo otvorit\n");
		return -1;
	}
	char prikaz[100];
	while (fgets(prikaz, 100, k) != NULL) { //CITAM Z KONZOLY CIZE K
		prikaz[strlen(prikaz) - 1] = '\0';//musim odstranit \n z konca...
		if (!strcmp(prikaz, "nacitaj"))
			zaciatok_filmov = nacitaj(zaciatok_filmov, f);
		if (!strcmp(prikaz, "vypis"))
			vypis(zaciatok_filmov);
		if (!strcmp(prikaz, "pridaj"))
			zaciatok_filmov = pridaj(zaciatok_filmov);
		if (!strcmp(prikaz, "vymaz")) {
			char nazov[100];
			fgets(nazov, 100, k);
			vymaz(&zaciatok_filmov,nazov);
		}
		if (!strcmp(prikaz, "koniec")) {
			ukonci(&zaciatok_filmov);
			return 1;
		}
			
			
		
	}
	zaciatok_filmov = nacitaj(zaciatok_filmov, f);
	vypis(zaciatok_filmov);
	zaciatok_filmov = pridaj(zaciatok_filmov);
	vypis(zaciatok_filmov);

}
