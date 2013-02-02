#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vars.h"




double mu(double val)
{
	return (val * MULTIPLO);

}


char *seps(int d) //1234567 => 3.234.567
{
	char *ret;
	int m=0,c=0,u=0;
    
	ret = (char*)calloc(80,sizeof(char));
	
	sprintf(ret, "%d", d);
	return ret;


	m = d/1000000; //3
	c = d/1000 /*3234*/- m*1000/*3000*/;    //234
	u = d - m * 1000000 - c * 1000;	   //567
	
	if (m>999)
		sprintf(ret, "%d", d);
	else if (m)
		sprintf(ret, "%d . %3 , #d.%3 , #d", m,c,u);
	else if (c)
		sprintf(ret, "%3,#d . %3,#d", c,u);
	else  sprintf(ret, "%3,#d",u);

	return ret;
	
}

void stampaContenuto(nodo_t *nodoPartenza)
{
   int j;
   //char contenuto; //'o' || 'V' || 'p'

   //struct nodo *sopra_o_sx;
   //struct nodo *sotto_o_dx;
   //int livello; //per comodità rapp
   

   //if (nodoPartenza==NULL)
	//   exit(1);
	if (nodoPartenza==NULL) return;

   if (nodoPartenza->contenuto=='p')
   {
        j = nodoPartenza->posizioneTaglioOrPezzo;
        fprintf(OUT, "<!-- PEZZO %d -->\n<div style=\"background-color: #%s; width:%.0fpx; height:%.0fpx; position:absolute; overflow: hidden; bottom:0px; left:0px;z-index:1;\">",
                                                  j,   colori[j%M],  /*L[j]*/mu(nodoPartenza->x)-0,    /*H[j]*/ mu(nodoPartenza->y )-0 );  

		/*if ( mu(L[j]) < 5.0 || mu(H[j]) < 5.0 )
		{ 	fprintf (OUT,"P%d", j); }
		else*/
		
		fprintf(OUT, "<div class=\"textp\" style=\"margin-top: %.0fpx; margin-left: %.0fpx;\"><b>%d</b></div>",
                                             mu(nodoPartenza->y)/2-8.0,  mu(nodoPartenza->x)/2-18.0,     j%M);  

		fprintf(OUT, "</div>\n");

   }
   else if (nodoPartenza->contenuto=='b')
   {
       //pezzo sotto
	   fprintf(OUT, "<!-- TAGLIO ORIZZ SOTTO -->\n<div style=\"width:%.0fpx;height:%.0fpx; position:absolute;overflow: hidden; bottom:0px; left:0px; border-top:#000000 %.0fpx solid; z-index: %d;\">\n   ",
			mu(nodoPartenza->sotto_o_dx->x ),
			mu(nodoPartenza->sotto_o_dx->y)-0,
			BORDER,
			Z_INDEX++);
        
	   stampaContenuto(nodoPartenza->sotto_o_dx);
       //pezzo sopra
	   fprintf(OUT, "\n</div>\n<!-- TAGLIO ORIZZ SOPRA-->\n\n<div style=\"width:%.0fpx;height:%.0fpx; position:absolute; overflow: hidden; left:0px; bottom:%.0fpx; border-bottom:#000000 %dpx solid; z-index: %d;\">\n   ",
			mu(nodoPartenza->sopra_o_sx->x),
			mu(nodoPartenza->sopra_o_sx->y )-0,
			mu(nodoPartenza->sotto_o_dx->y ),
			BORDER,
			Z_INDEX++);
        
	   stampaContenuto(nodoPartenza->sopra_o_sx);
       
	   fprintf(OUT, "</div>\n<!-- FINE TAGLIO ORIZZ -->\n\n"); 
        
   }
   else if (nodoPartenza->contenuto=='a')
   {
        
	   fprintf(OUT, "<!-- TAGLIO VERT SX -->\n<div style=\"width:%.0fpx;height:%.0fpx; position:absolute; overflow: hidden; bottom:0px; left:0px; border-right:#000000 %dpx solid;  z-index: %d;\">\n   ",
			mu(nodoPartenza->sopra_o_sx->x )-0,
			mu(nodoPartenza->sopra_o_sx->y ),
			BORDER,
			Z_INDEX++);
        
	   stampaContenuto(nodoPartenza->sopra_o_sx);
       


	   //if ((nodoPartenza->sopra_o_sx->x * MULTIPLO)>0)
	    fprintf(OUT, "\n</div>\n<!-- TAGLIO VERT DX -->\n<div style=\"width:%.0fpx;height:%.0fpx; position:absolute;overflow: hidden; left:%.0fpx; bottom:0px; border-left:#000000 %dpx solid; z-index: %d;\">\n   ",
			mu(nodoPartenza->sotto_o_dx->x )-0,
			mu(nodoPartenza->sotto_o_dx->y ),
			mu(nodoPartenza->sopra_o_sx->x ), //left:
			BORDER,
			Z_INDEX++);
	   /*else
		fprintf(OUT, "\n</div>\n<!-- TAGLIO VERT DX -->\n<div style=\"width:%.0fpx;height:%.0fpx; position:absolute; right:0px; bottom:0px; border-left:#000000 %dpx solid; z-index: %d;\">\n   ",
			nodoPartenza->sotto_o_dx->x * MULTIPLO,
			nodoPartenza->sotto_o_dx->y * MULTIPLO,
			BORDER,
			Z_INDEX++);*/
        
	   stampaContenuto(nodoPartenza->sotto_o_dx);
       
	   fprintf(OUT, "</div>\n"); 
   }
   else if (nodoPartenza->contenuto=='s') //scarto
   {
       fprintf(OUT, "<!-- SCARTO -->\n<div style=\"background-color: #ffffff; width:%.0fpx; height:%.0fpx; position:absolute;overflow: hidden; bottom:0px; left:0px;z-index:1;\"></div>\n",
                                                   mu(nodoPartenza->x)-0,   mu(nodoPartenza->y )-0 ); 
   }
   else
       printf("ERRORE contenuto\n");
  
}


void stampaContenuto2(nodo_t *nodoPartenza, int livello)
{
   int j;
   char liv[500];

   //identazione
   //liv = (char *)calloc(livello+2, sizeof(char));
   for (j=0; j<livello; j++)
	   liv[j] = ' ';
   liv[j] = '\0';	
   //liv[j+1] = '\0';	

   
    //sprintf(liv, "<span style=\"padding-left: %dpx; background-color:#ff0000\"></span>", livello * 5);	

   //if (nodoPartenza==NULL)
   if (nodoPartenza==NULL) return;

   if (nodoPartenza->contenuto=='p')
   {
        j = nodoPartenza->posizioneTaglioOrPezzo;
        fprintf(OUT, "%s [PEZZO %d]\n", liv,  j );  
   }
   else if (nodoPartenza->contenuto=='b')
   {

	   fprintf(OUT, "%sTAGLIO ORIZZ in pos B=%d\n%sSOTTO\n%s{\n",
		   liv,
		   nodoPartenza->posizioneTaglioOrPezzo,
		   liv, liv);
	
	   stampaContenuto2(nodoPartenza->sotto_o_dx, livello+1);

	   fprintf(OUT, "%s}\n%sSOPRA\n%s{%s\n", liv, liv, liv, liv);
        
	   stampaContenuto2(nodoPartenza->sopra_o_sx, livello+1);
       
	   fprintf(OUT, "%s}\n", liv); 
        
   }
   else if (nodoPartenza->contenuto=='a')
   {
        
	   fprintf(OUT, "%sTAGLIO VERT in pos A=%d\n%sSOPRA\n%s{\n",
		   liv,
		   nodoPartenza->posizioneTaglioOrPezzo,
		   liv, liv);
	
	   stampaContenuto2(nodoPartenza->sotto_o_dx, livello+1);

	   fprintf(OUT, "%s}\n%sSX\n%s{\n", liv, liv, liv, liv);
        
	   stampaContenuto2(nodoPartenza->sopra_o_sx, livello+1);
       
	  fprintf(OUT, "%s}\n", liv); 
   }
   else if (nodoPartenza->contenuto=='s') //scarto
   {
       fprintf(OUT, "%s [SCARTO]\n", liv ); 
   }
   else
       printf("ERRORE contenuto\n");
  
}


//dagli coordinate dei pezzi (TOP,LEFT) e TAGLI TOP-LEFT
void stampa()
{
    int i, temp, esiste;
	FILE *ftemp;
	
	MULTIPLO = 800.0/(double)X; //es: 0.5567
    MAX_H = 0;
	for(i=0; i<M; i++)
		MAX_H = (H[i] > MAX_H ) ? H[i] : MAX_H;
    
	if (NOMEFILE[0]	== '\0')
	{
		
		temp=0;
		do
		{
			
			sprintf(NOMEFILE, "out_%dx%d_%dpezzi_%d(%d).html", X, Y, M, (unsigned int)time(NULL), temp++  ) ;
			if (ftemp = fopen(NOMEFILE,"r"))
			{
				esiste = 1;
				fclose(ftemp);
			}
			else
				esiste=0;
		} 
		while (esiste);

	}


	OUT = fopen(NOMEFILE,"w");
    
    fprintf(OUT, "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />\n<title>Risultato taglio</title>\n");
    //stili
    fprintf(OUT, "<style type=\"text/css\">body, td, div{font-family:Verdana;font-size:%dpx;} .text { background-color:#ffffff; margin: 4px; width: auto; font-size: 14px;}.textp { background-color:#ffffff;width: 34px; height:auto; text-align:center;font-size: 14px;}\n", FONTSIZE);    
    
    fprintf(OUT, "</style></head>");
    //body
    fprintf(OUT, "<body>\n<div style=\"width:100%%; clear:both\"><h1>Pezzi</h1>%d pezzi</div>\n", M);
    
    //quadri pezzi 
    fprintf(OUT, "<div style=\"float:left; clear:left\">\n");
    for(i=0; i<M; i++)
		/*fprintf(OUT,"<div  style=\"width:%.0fpx; height:%.0fpx; float:left\"><div  style=\"background-color: #%s;width:%.0fpx; height:%.0fpx;margin:5px; float:left\"><div class=\"text\">PEZZO <b>%d</b><br>L=<b>%s</b>  H=<b>%s</b><br>A=<b>%s</b><br>V%d = <b>%s</b></div></div></div>\n",
											mu(L[i])+10,	mu(MAX_H)+10, colori[i%M], mu(L[i]), mu(H[i]),   i, seps(L[i]), seps(H[i]), seps(L[i]* H[i]), i, seps(V[i]) );*/
		fprintf(OUT,"<div  style=\"width:%.0fpx; height:%.0fpx; float:left\"><div  style=\"background-color: #%s;width:%.0fpx; height:%.0fpx;margin:5px; float:left\"><div class=\"textp\" style=\"margin-top: %.0fpx; margin-left: %.0fpx;\"><b>%d</b></div></div></div>\n",
										mu(L[i])+10,	mu(MAX_H)+10,                                colori[i%M],  mu(L[i]),   mu(H[i]),                                                                     mu(H[i])/2-8.0 ,  mu(L[i])/2-18.0 ,   i );

	fprintf(OUT, "\n</div>\n");

	fprintf(OUT, "<div style=\"float:left; clear:left\"><table border=\"1\"><tr><th>PEZZO</th><th>L</th><th>H</th><th>A</th><th>V</th></tr>\n");
	for(i=0; i<M; i++)
		fprintf(OUT,"<tr>\n<td  style=\"background-color: #%s\"><b>%d</b></td><td>%d</td><td>%d</td><td>%d</td><td>%d</td></tr>\n",
							  colori[i%M], i, L[i],H[i], L[i]* H[i], V[i] );
	fprintf(OUT, "\n</table></div>\n");
    
    //risultato taglio
    fprintf(OUT, "<div style=\"width:100%%; clear:both;\">\n<h1>Tagli</h1>Piano: L=<b>%d</b> H=<b>%d</b><br>Profitto totale: <b>%d</b><br>Tempo di esecuzione <b>%.0f ms</b><br>Area scarti: <b>%d</b><br>Riga di comando: ", 
					X,Y,FTOT, TEMPO_ESECUZ * 1000, SCARTO_TOTALE);
    
	fprintf(OUT, "--l %d --h %d --m %d --p ", X, Y, M);
	for(i=0; i<M; i++)
	{
		fprintf(OUT, "%d %d %d ", L[i], H[i], V[i]);
	}

	//stampa riga di comando per riottenere gli outputs
	
	fprintf(OUT, "</div>\n\n\n");

	fprintf(OUT,"<div style=\"width:100%%; clear:both; \">\n<br><b>SEQUENZA TAGLI: </b><br><pre style=\"font-size:10px\">");
	stampaContenuto2(ROOT, 0);
	fprintf(OUT, "</pre></div>");
	
	//PIANO RISULTATO
	fprintf(OUT, "<div style=\"width:%.0fpx; height:%.0fpx; border:#000000 %dpx solid; position:absolute; margin: 10px 0px 20px 0px ; \" >\n  ",
					  mu(X), mu(Y), 2);
        
    stampaContenuto(ROOT);
        
    fprintf(OUT,"</div>\n");
    
	
	

	fprintf(OUT, "\n</body>\n</html>");
    fclose(OUT);
	printf("Vedi risultato in %s\n", NOMEFILE) ;
}
