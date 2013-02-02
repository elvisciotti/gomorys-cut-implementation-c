#include <limits.h>
#include <time.h>
#include <string.h>
#include <stdlib.h> //rand
#include <math.h> //ceil
#include "vars.h"

int atoi2(int argc, char **argv, int i)
{
	if (i >= argc)
	{ 
		printf("ERRORE: parametro %d non esistente, esco!!\n", i);
		exit(1);
	}

	return atoi(argv[i]);
}



void leggiArgv(int argc, char **argv)
{
	/* --l 3 --h 2 --p 2 2 2 4 1 1 3 --output outarc.html */
	int i,j, p;
	int esisteElencoPezzi = 0;

	X=Y=M=0;
	
	for(i=0; i<argc; i++)
	{
		if (strcmp(argv[i], "--pa")==0)
			PROFITTI_COME_AREE=1;
	}

	for(i=0; i<argc; i++)
	{
		if (strcmp(argv[i], "--help")==0)
		{
			printf(" *** PROGRAMMA TAGLI 2D GHIGLIOTTINA by Elvis Ciotti ***\n\nUso:\n2d.exe  --l <L> --h <H> --m <M> --p <p1> <p2> ...  <pM> --pa --output <NOMEFILE.HTML> \ndove.\n  L, M = larghezza e altezza piano\n  M = numero pezzi\n  p1, p2, pM = elenco dei pezzi nel formato pM = <L> <H> <V>\n    (L,H,V sono larghezza, altezza e profitto)\n  --pa = setta il profitto dei pezzi proporzionale all'area\n  NOMEFILE = nome file html di output\n\n\nesempi:\n  ot.exe --l 3 --h 2 --m 2 --p 2 2 4 1 1 3\n  ot.exe --l 3000 --h 2000 --m 20\n");
			exit(1);
		}
		else if (strcmp(argv[i], "--l")==0)
			X = atoi2(argc, argv, ++i); /*atoi(argv[++i]);*/
		else if (strcmp(argv[i], "--h")==0)
			Y = atoi2(argc, argv, ++i);
		else if (strcmp(argv[i], "--m")==0)
			M = atoi2(argc, argv, ++i);
		else if (strcmp(argv[i], "--p")==0)
		{
			esisteElencoPezzi = 1;
			inizializzaStrDati(); //perchè conosco M !
			p = 0;
			j = i+1; //j è indice secondario che parte da i e continua
			while ( j < (i + M*3)  ) // M=2 => leggo 6 numeri
			{
				L[p] = atoi2(argc, argv, j++); //2 //p=0
				H[p] = atoi2(argc, argv, j++); //2
				V[p] = atoi2(argc, argv, j++); //4
				//
				if (PROFITTI_COME_AREE)
					V[p] = (int)(L[p] * H[p]/ SCALA_PROFITTI);	
					
				p++;
			}
			//i = j-2; //salto i dati dei pezzi
		}
		else if (strcmp(argv[i], "--output")==0)
			strcpy(NOMEFILE, argv[++i]); 
	}
	
	if (X==0 || Y==0 || M==0)
		printf("aggiungi l'opzione --help per la guida comandi\n");

	//se mancano alcuni dati..
	if (X==0)
		X = leggi("Larghezza rettangolo: ");
	
	if (Y==0)
		Y = leggi("Altezza rettangolo: ");
	
	if (M==0)
	{
		M = leggi("Inserisci numero di pezzi: "); 
	}

	//MP2 = M;
	
	//genero pezzi random se non avevo passato M o nn c'è elenco
	if (M==0 || esisteElencoPezzi==0)
	{
		inizializzaStrDati();
		generaPezziRandom();
	}
}

//lanciarla con zero

void normalCuts()
{
	int i;
	
	/*for(i=0; i<M; i++)
		printf("%d %d %d\n", L[i], H[i], V[i]);*/
	
	for(i=0; i<X; i++)
		NCUTSX[0] = 0;

	for(i=0; i<Y; i++)
		NCUTSY[0] = 0;

	for(i=0; i<MP2; i++)
	{
		if (!PEZZI_INUTILI[i])
		{
			ncx(L[i]);
			ncy(L[i]);	
		}
	}

	
	/* specchio valori ! */
	/*for(i=X/2; i<X; i++)
		if (NCUTSX[i])
			NCUTSX[X-i] = 1;	

	for(i=Y/2; i<Y; i++)
		if (NCUTSY[i])
			NCUTSY[Y-i] = 1;*/	

	NCUTSX[0] = NCUTSY[0] = 0;
	
	NCUTSX[X] = NCUTSY[Y] = 1;
}



void ncx(int l_corr)
{
	int i;
	
	if (l_corr >= X || NCUTSX[l_corr] == 1)
		return;

	NCUTSX[l_corr] = 1;

	for(i=0; i<MP2; i++)
		  ncx(l_corr + L[i]);
}

void ncy(int l_corr)
{
	int i;
	
	if (l_corr >= Y || NCUTSY[l_corr] == 1)
		return;

	NCUTSY[l_corr] = 1;

	for(i=0; i<MP2; i++)
		  ncy(l_corr + L[i]);
}


void calcola_minimo()
{
	int i;
	
	MIN_XY_PEZZO = RAND_MAX;
	//cerco h e l minimi
	for(i=0; i<M; i++)   
	{
		if (L[i] < MIN_XY_PEZZO)
		  MIN_XY_PEZZO = L[i];
		if (H[i] < MIN_XY_PEZZO)
		  MIN_XY_PEZZO = H[i];
	}
}

void errore(char *s)
{
	printf("ERRORE !!! %s\n", s);
	system("pause");
	
}

void generaPezziRandom()
{
    int i, temp;
	double coeff;
	double maxInt;

	maxInt = RAND_MAX;
    //L = (int *)calloc(M, sizeof(int));
	//H = (int *)calloc(M, sizeof(int));
	//V = (int *)calloc(M, sizeof(int));
    
	srand((unsigned int)time(NULL));
	
	//imposto pezzi
    if (M<=5)
	{
	   for(i=0; i<M; i++)
	   {
		  L[i] = X/3 + ( rand() % (X*2/3 - X/3) );
		  if ( L[i] ==0 ) L[i] = 1;
		  H[i] = Y/3 + ( rand() % (Y*2/3 - Y/3) );
		  
		  if ( H[i] ==0 ) H[i] = 1;
		  
		 
		  coeff = (PROFITTI_COME_AREE) ? (1.0) : (1.0 + rand()/(double)maxInt); 
		  V[i] = (int)(L[i] * H[i] * coeff / SCALA_PROFITTI);
		  

		  if ( V[i] ==0 ) V[i] = 1;

		  /*printf("pezzo %d: %dx%d, V=%d\n",i, L[i], H[i], V[i]);*/
	   }
	}
	else if (M<=10)
	{
	   for(i=0; i<M; i++)
	   {
		  L[i] = X/3 + ( rand() % (X/2 - X/3) );
		  H[i] = Y/3 + ( rand() % (Y/2 - Y/3) );
		  
		  coeff = (PROFITTI_COME_AREE) ? (1.0) : (1.0 + rand()/(double)maxInt); 
		  V[i] = (int)(L[i] * H[i] * coeff / SCALA_PROFITTI);
	   }
	}
	else // (M<=20)
	{
	   for(i=0; i<M; i++)
	   {
		  L[i] = X/4 + ( rand() % (X/2 - X/4) );
		  H[i] = Y/4 + ( rand() % (Y/2 - Y/4) );
		  
		  coeff = (PROFITTI_COME_AREE) ? (1.0) : (1.0 + rand()/(double)maxInt); 
		  V[i] = (int)(L[i] * H[i] * coeff / SCALA_PROFITTI);
	   }
	}
	//return; //temp

	// un quarto dei pezzi lo ruoto
	if (VARIANTE_ROTAZIONE)
	{
	   for(i=M*2/4; i<M*3/4; i++)
	   {
		  temp = L[i];
		  L[i] = H[i];
		  H[i] = temp;
	   }

	}

	//dimezzo un altro quarto dei pezzi
	if (VARIANTE_PEZZI_PICCOLI)
	{
		for(i=M*3/4; i<M; i++)
		{
			L[i] = L[i]/2;
			H[i] = H[i]/2;
			V[i] = V[i]/5;
		}

	}


	
	
}

void togliPezziInutili()
{

	int i,j;
	
	//pezzi inutili
	for(i=0; i<MP2; i++)
	{
		for(j=0; j<MP2; j++)
		{
			if (i != j && L[i]<=L[j] && H[i]<=H[j] && V[i]>V[j] )
			{
				//printf("-->Il pezzo %d è inutile (%d è piccolo e con piu valore)\n", j, i);
				PEZZI_INUTILI[j] = 1;
				NUM_PEZZI_INUTILI++;
			}

		}	

	}
	printf("%d PEZZI INUTILI\n", NUM_PEZZI_INUTILI/2);
}




//inizializza F0 e FXY, serve M già calcolato
void inizializzaStrDati()
{
	int i,j;
	//tagliVert = (int *)calloc(X, sizeof(int));
    //tagliOriz = (int *)calloc(Y, sizeof(int));
	

	//colori
	strcpy(colori[0], "FF0000\0");
	strcpy(colori[1], "3300CC\0");
	strcpy(colori[2], "339900\0");
	strcpy(colori[3], "FFCC00\0");
	strcpy(colori[4], "CCCCCC\0");
	strcpy(colori[5], "00FFFF\0");
	strcpy(colori[6], "9900FF\0");
	strcpy(colori[7], "934900\0");
	strcpy(colori[8], "FF9900\0");
	strcpy(colori[9], "FF99CC\0");
	strcpy(colori[10], "00CC00\0");
	strcpy(colori[11], "999999\0");
	strcpy(colori[12], "00CC66\0");
	strcpy(colori[13], "0099FF\0");
	strcpy(colori[14], "99CCCC\0");
	strcpy(colori[15], "9999FF\0");
	strcpy(colori[16], "99CCFF\0");
	strcpy(colori[17], "CCCCFF\0");
	strcpy(colori[18], "FFCCFF\0");
	strcpy(colori[19], "66FF99\0");


	ROOT = NULL;

	Z_INDEX = 1;
	BORDER = 1;
	//PASSO =0;
	//CHIAMATE = 0;
	SCARTO_TOTALE = 0;
	DEBUG_INFO = 0;
	NUM_PEZZI_INUTILI = 0;
	

	MAX_XY = X > Y ? X : Y;

	//normal cuts
	NCUTSX = (int *)calloc(X+3, sizeof(int));
	NCUTSY = (int *)calloc(Y+3, sizeof(int));
	//DISABILITA_N_CUTS = 0;
	//PROFITTI_COME_AREE = 0;
	

	FONTSIZE = 16;

	//matrici calcolo
	F0 =  (int **)calloc(X+1, sizeof(int *));
	FXY = (int **)calloc(X+1, sizeof(int *));
	M1 = (char **)calloc(X+1, sizeof(char *));
	M2 =  (int **)calloc(X+1, sizeof(int *));
	for (j=0; j<X+1; j++)
	{
		F0[j]  = (int *)calloc(Y+1, sizeof(int));
		FXY[j] = (int *)calloc(Y+1, sizeof(int));
		M1[j] = (char *)calloc(Y+1, sizeof(char));
		M2[j] =  (int *)calloc(Y+1, sizeof(int));
		for(i=0; i<Y+1; i++)
		{
           F0[j][i] = -1;  
           FXY[j][i] = -1;
           M1[j][i] = 's'; /* inizialmente sono tutti scarti */
           M2[j][i] = -1;                     
        }
	}
	
	//larghezze pezzi
	L = (int *)calloc(M*2, sizeof(int));
	H = (int *)calloc(M*2, sizeof(int));
	V = (int *)calloc(M*2, sizeof(int));
	PEZZI_INUTILI = (int *)calloc(M*2, sizeof(int));

	NOMEFILE[0] = '\0';

	MIN_XY_PEZZO = RAND_MAX;

}



void inizializzaF0()
{
	int i,j,k,v_fmax,v_indice;
	
	//per ogni cella...
	for (i=1; i<=X; i++)
	{
		for (j=1; j<=Y; j++)
		{
			//cerco pezzo che ci sta meglio e con V MAX
			v_fmax = 0;
			v_indice = -1;
			for(k=0; k<M*2; k++)
			{
				if (!PEZZI_INUTILI[k] && L[k] <= i && H[k] <= j && V[k] > v_fmax)
				{
					v_fmax = V[k];
					v_indice=k;
				}
			}
			F0[i][j] = v_indice;
			//printf("%5d(%d)", v_fmax, v_indice);

		}
		//printf("\n");

	}

}



int leggi(char *s)
{
    int c;
    do
	{
      printf("%s", s);   
      scanf("%d", &c);
    } while(c<1);
    
    return c;
}


void disalloca()
{
	int j;
	
	free(NCUTSX);
	free(NCUTSY);

	F0 =  (int **)calloc(X+1, sizeof(int *));
	FXY = (int **)calloc(X+1, sizeof(int *));
	M1 = (char **)calloc(X+1, sizeof(char *));
	M2 =  (int **)calloc(X+1, sizeof(int *));
	for (j=0; j<X+1; j++)
	{
		free(F0[j]);
		free(FXY[j]);
		free(M1[j]);
		free(M2[j]);
	}
	
	//larghezze pezzi
	free(L);
	free(H);
	free(V);

	/*disallocaNodo(ROOT);*/

}



void disallocaNodo(nodo_t *nodoPartenza)
{
   nodo_t *temp;

   //if (nodoPartenza==NULL)
   if (nodoPartenza==NULL) return;

	if (nodoPartenza->sotto_o_dx != NULL)
	{
	   if (nodoPartenza->sotto_o_dx->contenuto=='p')
	   {
			temp = nodoPartenza->sotto_o_dx;
			free (temp);
			nodoPartenza->sotto_o_dx = NULL;
	   }
	   else
			disallocaNodo( nodoPartenza->sotto_o_dx );
	}
   //
   if (nodoPartenza->sopra_o_sx->contenuto=='p')
   {
        temp = nodoPartenza->sopra_o_sx;
		free (temp);
		nodoPartenza->sopra_o_sx = NULL;
   }
   else
		disallocaNodo( nodoPartenza->sopra_o_sx );  
}

