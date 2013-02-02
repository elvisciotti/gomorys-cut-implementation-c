#include <stdio.h>
#include "vars.h"



//copia pezzi nella seconda parte array ruotandoli
void raddoppiaRuotati()
{
	int i;
    

	MP2 = M * 2;
    for (i=0; i<M; i++)
	{
		L[i+M] = H[i];
	    H[i+M] = L[i];
	    V[i+M] = V[i];
	} 	
}

void debug()
{
    int i,j;
    
    printf("cuts X\n");
    for (i=1; i<=X; i++)
	{
		if (NCUTSX[i])
			printf("%3d", i);
	} 
	printf("\n");
	printf("cuts Y\n");
    for (i=1; i<=Y; i++)
	{
		if (NCUTSY[i])
			printf("%3d", i);
	} 
	printf("\n");
	printf("F0\n");
    for (i=1; i<=X; i++)
	{
		for (j=1; j<=Y; j++)
		    printf("%5d", F0[i][j]);
		printf("\n");
	} 
	
	printf("FXY\n");
    for (i=1; i<=X; i++)
	{
		for (j=1; j<=Y; j++)
		    printf("%5d", FXY[i][j]);
		printf("\n");
	} 
	
	printf("M1\n");
    for (i=1; i<=X; i++)
	{
		for (j=1; j<=Y; j++)
		    printf("    %c", M1[i][j]);
		printf("\n");
	} 
	
	printf("M2\n");
    for (i=1; i<=X; i++)
	{
		for (j=1; j<=Y; j++)
		    printf("%5d", M2[i][j]);
		printf("\n");
	} 
	system("pause");
}



int f(int x_pezzo, int y_pezzo)
{
	int b, a, temp, max;

	int s_fmax=0; int s_indice = 0;
	int profittoTaglioB = 0; int posB = 0;
	int profittoTaglioA = 0; int posA = 0;
	int indicepezzo = -1,valorePezzo=-1;
    
	//CHIAMATE++;

	//printf("Chiamata di F(%d)(%d)\n", x_pezzo, y_pezzo);
	
    //profitto di questo pezzo già calcolato ?
	temp = FXY[x_pezzo][y_pezzo];
    if ( temp >= 0 )  // ovvero != -1
       return temp;

	//calcolo il pezzo SINGOLO che mi dà valore massimo

	indicepezzo = F0[x_pezzo][y_pezzo];
	if ( indicepezzo>=0 ) /* se è maggiore di zero, allora c'è un pezzo (inizialmente i valori sono -1) */
		valorePezzo = V[indicepezzo];
	else 
	{
		//se nessun pezzo ci sta, inutile che cerco con il taglio, ritorno ZERO !
		M1[x_pezzo][y_pezzo] = 's';
		FXY[x_pezzo][y_pezzo] = 0;
		return 0;
	}
	

	//calcolo i profitti da TUTTI i possibili TAGLI ORIZZONTALI
	for(b = y_pezzo/2; b < y_pezzo/* /2 + 1*/; b++)
	{
		if (NCUTSY[b])
		{
			/*if (DEBUG_INFO)
				printf("Chiamata di F(%d)(%d) - calcolo pezzo b=%d\n", x_pezzo, y_pezzo, b);
			*/
			temp = f(x_pezzo, b) + f(x_pezzo, y_pezzo - b);
			//se ottengo un proditto maggiore del taglio precedente...
			if (temp > profittoTaglioB)
			{
				profittoTaglioB = temp;
				posB = b;
			}
		}
		
	}

	//calcolo i profitti da TUTTI i possibili TAGLI VERTICALI
	for(a=x_pezzo/2; a < x_pezzo /*/2 + 1*/; a++)
	{
		if (NCUTSX[a])
		{
			/*if (DEBUG_INFO)
			printf("Chiamata di F(%d)(%d) - calcolo pezzo a=%d\n", x_pezzo, y_pezzo, a);*/

			temp = f(a, y_pezzo) + f(x_pezzo-a, y_pezzo);
			//se ottengo un proditto maggiore del taglio precedente...
			if (temp > profittoTaglioA)
			{
				profittoTaglioA = temp;
				posA=a;
			}
		}
		
	}
	//non fa il taglio A in F(3,2), rivedi codice, forse dovuto a  if ( indicepezzo>=0 )
    //vedo il massimo tra i 3
	//privilegio i tagli, così mi taglia sempre accanto i pezzi anche se c'è scarto accanto
    
	max = 0;
    
	if ( profittoTaglioB >= valorePezzo && profittoTaglioB >= profittoTaglioA )
    {
        max =  profittoTaglioB; 
		/*if (DEBUG_INFO)
			printf("Chiamata di F(%d)(%d) - MAX [%d][%d][%d] = profittoTaglioB\n", x_pezzo, y_pezzo, valorePezzo,profittoTaglioA, profittoTaglioB);
        */
		M1[x_pezzo][y_pezzo] = 'b';
        M2[x_pezzo][y_pezzo] = posB;
    }
	else if ( profittoTaglioA >= profittoTaglioB && profittoTaglioA >= valorePezzo )
    {
        max =  profittoTaglioA; 
		/*if (DEBUG_INFO)
			printf("Chiamata di F(%d)(%d) - MAX [%d][%d][%d] = profittoTaglioA\n", x_pezzo, y_pezzo, valorePezzo,profittoTaglioA, profittoTaglioB);
        */
		M1[x_pezzo][y_pezzo] = 'a';     
        M2[x_pezzo][y_pezzo] = posA;
    }
	else if ( valorePezzo >= profittoTaglioA && valorePezzo >= profittoTaglioB )
    {
        max =  valorePezzo;
		/*if (DEBUG_INFO)
			printf("Chiamata di F(%d)(%d) - MAX [%d][%d][%d] = valorePezzo\n", x_pezzo, y_pezzo, valorePezzo,profittoTaglioA, profittoTaglioB);
        */
		M1[x_pezzo][y_pezzo] = 'p';
        M2[x_pezzo][y_pezzo] = indicepezzo;
    }

	/*else
	{
		printf("errore f[%d][%d] =[%d][%d][%d]", x_pezzo, y_pezzo, profittoTaglioA,profittoTaglioB,valorePezzo);

		system("pause");
	}*/
		
	//if (max==0)
	//	M1[x_pezzo][y_pezzo] = 's';	
	FXY[x_pezzo][y_pezzo] = max; 
   
   //PASSO++;


	/*PASSO++;
    if (PASSO%100000==0) 
      printf("%d su %d\n", PASSO, X*Y);
	*/
	return max;
}


//crea albero a partire da FXY,M1,M2
nodo_t *espandi(int x_corr, int y_corr)
{
	nodo_t *p;
	int posTaglioOrPezzo;
	int operazioneNellaPosizione;

	if (DEBUG_INFO)
		printf("espansione [%d][%d]\n",x_corr,y_corr);

	p= (nodo_t *)malloc(sizeof(nodo_t));
	p->x = x_corr;
	p->y = y_corr;

	if (DEBUG_INFO)
		printf("Espansione [%d][%d]\n", x_corr, y_corr );

	operazioneNellaPosizione = M1[x_corr][y_corr];

	//scarto
	if ( operazioneNellaPosizione == 's' )
	{
		p->contenuto = 's';
		SCARTO_TOTALE += (x_corr * y_corr);

	}
	//operazioneNellaPosizione pezzo
	else if ( operazioneNellaPosizione == 'p' ) 	
	{
		p->contenuto = 'p';
		posTaglioOrPezzo = M2[x_corr][y_corr];
		p->posizioneTaglioOrPezzo = posTaglioOrPezzo;
		
		//p->x = L[posTaglioOrPezzo];
		//p->y = H[posTaglioOrPezzo];
	}
	//taglio orizz
	else if ( operazioneNellaPosizione == 'b' )
	{
		p->contenuto = 'b';
		posTaglioOrPezzo = M2[x_corr][y_corr];
		p->posizioneTaglioOrPezzo = posTaglioOrPezzo;

		p->sopra_o_sx = espandi(x_corr, y_corr-posTaglioOrPezzo); //sopra
		//p->sopra_o_sx->x = x_corr;
		//p->sopra_o_sx->y = y_corr-posTaglioOrPezzo;

		p->sotto_o_dx = espandi(x_corr, posTaglioOrPezzo);	//sotto
		//p->sotto_o_dx->y = y_corr;

	}
	//taglio vert
	else if (operazioneNellaPosizione == 'a' )
	{
		p->contenuto = 'a';
		posTaglioOrPezzo = M2[x_corr][y_corr];
		p->posizioneTaglioOrPezzo = posTaglioOrPezzo;

		p->sopra_o_sx = espandi(posTaglioOrPezzo, y_corr);	//sx

		p->sotto_o_dx = espandi(x_corr-posTaglioOrPezzo, y_corr);	//dx

	}
	else
	{
		printf("operazione nn specificata [%d][%d]\n",x_corr,y_corr);
		system("pause");
	}


	return p;


}
