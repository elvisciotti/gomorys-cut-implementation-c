#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
#include "vars.h"



int main(int argc, char **argv)
{

	int i, pt;
	SCALA_PROFITTI = 1.0;
	PROFITTI_COME_AREE = 0;
	VARIANTE_ROTAZIONE = 1;
	VARIANTE_PEZZI_PICCOLI = 1;
	MP2 = M;

	if (argc>1)
	{
		leggiArgv(argc, argv);
		
		//generaPezziRandom();
	}
	else
	{
		//input dati rettangolo
		X = leggi("Larghezza rettangolo: ");
		Y = leggi("Altezza rettangolo: ");
		
		printf("Rettangolo %dx%d\n", X, Y);
		//leggo #pezzi e alloco
		M = leggi("Inserisci numero di pezzi (5,10 20)"); 
		inizializzaStrDati();
		generaPezziRandom();
	}
	
	raddoppiaRuotati();
	togliPezziInutili();
	
	DEBUG_INFO = 0;

	printf("inizio nc\n");	
	calcola_minimo(); //serve a f per capire quando c'è scarto, dovrebbe velocizzare f
	normalCuts(); 
	
	//for (i=0; i<X; i++)
	//	NCUTSX[i]=1;

	for (i=0, pt=0; i<X; i++)
		if (NCUTSX[i])
			pt++;
	printf("%d posizioni di taglio vert\n", pt);	

	inizializzaF0();
	
    /*GO !!!!!!!*/
	FONTSIZE=15;
	BORDER = 1;
	
	printf("inizio f !!\n");
	CLOCK_START = clock();
	FTOT = f(X,Y);
	CLOCK_END = clock();
	TEMPO_ESECUZ = ((double)(CLOCK_END-CLOCK_START))/CLOCKS_PER_SEC;
	printf("fine f !!\n");

	ROOT = espandi(X,Y);
	stampa();
	
	disalloca();

	printf("Profitto totale: %d\n\n\ntempo: %.2f sec\n", FTOT, TEMPO_ESECUZ);
	//system("pause");

	return 0;
}

