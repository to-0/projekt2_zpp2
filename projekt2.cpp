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
	if (akt!=NULL && !(strcmp(akt->nazov, nazov))) {
		p->dalsi = akt->dalsi;
		free(akt);
	}
	else {
		printf("Film sa nenasiel\n");
	}
	

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
void filmy(FILM* zaciatok_f, char krstne[], char priezvisko[]) { // vypise vsetky filmy v ktorom hra herec
	FILM* akt = zaciatok_f;
	if (zaciatok_f == NULL) {
		printf("Prazdne filmy\n");
		return;
	}
	while (akt != NULL) { //idem cez vsetky filmy
		HEREC* akt_h = akt->herci;
		while (akt_h != NULL) { //idem cez vsetkych hercov filmu pokial nenajdem zhodu alebo pokial neprejdem na koniec
			if (!strcmp(akt_h->meno.krstne, krstne) && (!strcmp(akt_h->meno.priezvisko, priezvisko))) { //ak som nasiel toho herca idem vypisat film
				printf("%s\n", akt->nazov);
				break;
			}
			akt_h = akt_h->dalsi; //posuvam sa po hercoch
		}
		akt = akt->dalsi;
	}
}
int najdi_dvojicu(HEREC* h1, HEREC* h2) { //pomocna funkcia, skontroluje ci sa herec2 nachadza niekde v zozname hercov1
	while (h1 != NULL) {
		if (!strcmp(h2->meno.krstne, h1->meno.krstne) && !strcmp(h2->meno.priezvisko, h1->meno.priezvisko)) { //ak sa mena rovnaju
			return 1;
		}
		h1 = h1->dalsi;
	}
	return 0; //nenachadza
}

void herci(FILM* zac, char film1[], char film2[]) { //vypise hercov ktori hrali v oboch filmoch, bez opakovania, v poradi v akom su ulozeni
	if (zac == NULL) {
		printf("Prazdny zoznam filmov=n");
		return;
	}
	int nasiel_f1 = 0, nasiel_f2 = 0; //pomocna premenna, zaznacim si ci film1 alebo film2 vobec existuje
	FILM* f1=NULL,*f2=NULL; //f1 ukazuje na prvy film co najdem a f2 na druhy film co najdem
	while (zac != NULL) {
		if (!strcmp(film1, zac->nazov)) {
			f1 = zac;
		}
		if (!strcmp(film2, zac->nazov)) {
			f2 = zac;
		}
		zac = zac->dalsi;
	}
	if (f1 == NULL || f2 == NULL) {
		printf("Dvojica filmov nebola najdena\n");
		return;
	}
	HEREC* h1 = f1->herci, * h2 = f2->herci;
	if (h1 == NULL && h2 == NULL) {
		printf("Oba filmy nemaju hercov\n");
		return;
	}
	int k = 1; //pomocna premenna ci sa rovnaju 2 mena
	HEREC* pom = h1;//ukazujem na zaciatok hercov z druheho filmu
	while (h1 != NULL) {
		printf("%s %s (%d)", h1->meno.krstne, h1->meno.priezvisko, h1->rok);
		h1 = h1->dalsi;
		if (h1 != NULL || h2 != NULL) putchar(','); //ak bude pokracovat aspon jeden herec tak este napisem ciarku
	}
	h1 = pom; //znova nastavim h1 na prveho herca
	while (h2 != NULL) { //idem prechadzat hercov z oboch filmov, ak skoncim na jednom z nich tak uz nema zmysel dalej hladat
		if (!najdi_dvojicu(h1,h2)) { //nenasiel som rovnakeho herca vo filme 1
			printf("%s %s (%d)", h2->meno.krstne, h2->meno.priezvisko, h2->rok);
			if (h2->dalsi != NULL && !najdi_dvojicu(h1, h2->dalsi)) putchar(','); //ak dalsi herec nebude 0 a zaroven sa nebude dalsi herec rovnat nejakemu z prveho filmu mozem napisat,
		}
		h2 = h2->dalsi;
	}
}
//ODTIALTO FUNKCIE IBA PRE MOZNOST ROK
FILM* najdi_film_rok(FILM* zac_f, int rok) { //pomocna funkcia na najdenie filmu v danom roku, tuto funkciu budem postupne volat pricom sa bude jej argument
	while (zac_f != NULL) {
		if (zac_f->rok_vyroby == rok)
			return zac_f;//vratim zaciatok filmu
		zac_f = zac_f->dalsi;
	}
	return NULL;
}
HEREC *pridaj_herca_zoradene(HEREC *zac,HEREC *herec){ //prida herca tak aby bol zoznam stale zoradeny
    HEREC *akt=zac;
    HEREC *pom=akt;
	HEREC* vloz = (HEREC*)malloc(sizeof(HEREC));
	vloz->rok = herec->rok;
	strncpy(vloz->meno.krstne, herec->meno.krstne, 100);
	strncpy(vloz->meno.priezvisko, herec->meno.priezvisko, 100);
	vloz->dalsi = NULL;
    int men=0; //pomocna premenna ci idem menit
	if (zac == NULL) return vloz;
    if(strcmp(zac->meno.krstne,vloz->meno.krstne)>1){ // musime ho vlozit na zaciatok
        men=1;
    }
    //ak sa ich krstne mena rovnaju musime skontrolovat priezvisko
    else if(!strcmp(zac->meno.krstne,vloz->meno.krstne) && strcmp(zac->meno.priezvisko,vloz->meno.priezvisko)>1){
        men=1;
    }
    //ak sa aj priezvisko rovna kontrolujeme rok
    else if(!strcmp(zac->meno.krstne,vloz->meno.krstne) && !strcmp(zac->meno.priezvisko,vloz->meno.priezvisko) &&zac->rok>vloz->rok){
        men = 1;
    }
    if(men){
    pom=zac;
    zac=vloz;
    vloz->dalsi = pom;
    return zac;
    }
    //nemusime menit zaciatok ideme dalej
    pom=akt; //pom bude teraz predosly v podstate
    while(akt!=NULL){ 
        if(strcmp(akt->meno.krstne,vloz->meno.krstne)>1) // krstne meno herca, ktoreho vkladame je skor v abecede
            men=1;
        else if(!strcmp(zac->meno.krstne,vloz->meno.krstne) && strcmp(zac->meno.priezvisko,vloz->meno.priezvisko)>1)
            men=1;
        //ak sa aj priezvisko rovna kontrolujeme rok
        else if(!strcmp(zac->meno.krstne,vloz->meno.krstne) && !strcmp(zac->meno.priezvisko,vloz->meno.priezvisko) &&
        zac->rok>vloz->rok)
            men = 1;
        if(men){
            pom->dalsi = vloz;
			vloz ->dalsi = akt;
            return zac;
        }
        pom = akt;
        akt = akt->dalsi;
    }
	//ak ma ist tento herec na koniec
	akt = vloz;
	return zac;
}
void vypis_hercov(HEREC *zac){
    if (zac==NULL) return;
    while(zac!=NULL){
        printf("%s %s (%d)",zac->meno.krstne, zac->meno.priezvisko, zac->rok);
        if(zac->dalsi!=NULL) printf(", ");
		zac = zac->dalsi;
    }
    putchar('\n');
}
void uvolni_zoznam_hercov(HEREC *zac){
    if (zac==NULL) return;
	HEREC* pom=zac;
    while(zac!=NULL){
        pom = zac;
        zac = zac->dalsi;
        free(pom);
    }
}
void rok(FILM* zac_f, int rok) { //vypise zoznam hercov ktori hrali vo filmoch v danom roku, zoradene abecedne a neopakuju sa
	HEREC* zac_zh = NULL; //zaciatok zoznamu hercov
	HEREC *vloz_h= (HEREC *)malloc(sizeof(HEREC)); //inicializujem prvy prvok zoznamu hercov
	FILM* akt = najdi_film_rok(zac_f, rok); //najdem prvy film
	if (akt == NULL) {
		printf("Neexistuje film z daneho roku");
		return;
	}
	while(akt!=NULL){ //idem cez zoznam vsetkych filmov
	    HEREC *akt_h=akt->herci;
	    if (akt_h==NULL) //ak tu neni ziadny iny herec tak idem najst dalsi film z toho roku
	        akt = najdi_film_rok(akt->dalsi,rok);
		//vloz_h = akt_h iba spravilo ze vloz_h ukazoval na aktualneho herca;
		 //nastavim vzdy ze neukazuje este nikam tento herec
	    while(akt_h!=NULL) { //pokial nie som na konci hercov
           zac_zh = pridaj_herca_zoradene(zac_zh, akt_h); //pridam herca do zoznamu
           akt_h = akt_h->dalsi;
        }
		akt = najdi_film_rok(akt->dalsi, rok);
	}
	//tu by som mal mat teda zoznam vsetkych hercov
	vypis_hercov(zac_zh);
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
		if (!strcmp(prikaz, "filmy")) {
			char krstne[100];
			char priezvisko[100];
			if (scanf("%s %s", krstne, priezvisko) == 2)
				filmy(zaciatok_filmov, krstne, priezvisko);
		}
		if (!strcmp(prikaz, "herci")) {
			char film1[100];
			char film2[100];
			if (fgets(film1, 100, stdin) != NULL && fgets(film2, 100, stdin)) {
				film1[strlen(film1) - 1] = '\0'; //lebo inac by bol posledny char \n
				film2[strlen(film2) - 1] = '\0';
				herci(zaciatok_filmov, film1, film2);
			}
				
		}
		if (!strcmp(prikaz, "vymaz")) {
			char nazov[100];
			fgets(nazov, 100, k);
			nazov[strlen(nazov) - 1] = '\0';
			vymaz(&zaciatok_filmov,nazov);
		}
		if (!strcmp(prikaz, "rok")) {
			char buffer[100];
			fgets(buffer, 100, stdin);
			int n;
			sscanf(buffer, "%d", &n);
			rok(zaciatok_filmov, n);
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
