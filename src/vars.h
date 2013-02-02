#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int X,Y; //dimensioni vetro
int MAX_XY;

int *NCUTSX;
int *NCUTSY;

int *L;  //array larghezze pezzi
int *H;  //array altezze pezzi
int *V;  //array profitti pezzi

int MAX_H;

//int *tagliVert;
//int *tagliOriz;
int M; //numero tipi di pezzi
int MP2; //numero tipi di pezzi
int *PEZZI_INUTILI;
int NUM_PEZZI_INUTILI;

int **F0;  //indici dei pezzi che stanno meglio nello spazio relativo
int **FXY; //risultati delle chiamate di F con i profitti
char **M1; //memorizzazione operazione (pezzo || taglio a || taglio b || scarto s) che sono state fatte per dare il profitto max nella relativa dimensione
int **M2;  //altri dati per l'operazione che ha dato profitto (pos. di taglio o #pezzo)

double MULTIPLO; //rapporto stampa html

int Z_INDEX;
int BORDER; //bordo di stampa

char colori[20][7];
FILE *OUT;
char NOMEFILE[600];

//long PASSO;
//long CHIAMATE;
int SCARTO_TOTALE;
int DEBUG_INFO;

int FTOT;

//int DISABILITA_N_CUTS;
int PROFITTI_COME_AREE;
int VARIANTE_ROTAZIONE;
int VARIANTE_PEZZI_PICCOLI;
double SCALA_PROFITTI;
int MIN_XY_PEZZO;

typedef struct nodo
{
   char contenuto; //'o' || 'V' || 'p'
   int posizioneTaglioOrPezzo; //orizz o verticale, in ogni caso da TOP,LEFT || #pezzo
   int x;
   int y;
   struct nodo *sopra_o_sx;
   struct nodo *sotto_o_dx;
   struct nodo *padre;
   int livello; //per comodità rapp
} nodo_t;

nodo_t *ROOT;
int FONTSIZE;

clock_t CLOCK_START, CLOCK_END;
double TEMPO_ESECUZ;


void raddoppiaRuotati();
void debug();
int f(int x_pezzo, int y_pezzo);
nodo_t *espandi(int x_corr, int y_corr);

int atoi2(int argc, char **argv, int i);
void leggiArgv(int argc, char **argv);
void errore(char *s);
void togliPezziInutili();
void generaPezziRandom();
void inizializzaStrDati();
void inizializzaF0();
int leggi(char *s);

double mu(double val);
void stampaContenuto(nodo_t *nodoPartenza);
void stampaContenuto2(nodo_t *nodoPartenza, int livello);
void stampa();
char *seps(int d);

void normalCuts();
void ncx(int l_corr);
void ncy(int h_corr);
void disalloca();
void disallocaNodo(nodo_t *nodoPartenza);
void calcola_minimo();
