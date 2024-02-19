    /*  RADU Anca - 315 CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nod_arbore{
    unsigned char tip;  //  tipul nodului
    unsigned char *culoare;
    struct nod_arbore *fiu[4];
    int nivel;  //  nivelul din arbore al nodului
}   TNod, *TArbore;

typedef struct culori{
    unsigned char rosu;
    unsigned char verde;
    unsigned char albastru;
} Culori, *Culori_ptr;

typedef struct celula
{ 
  TArbore nod;  //  pointer catre un nod al arborelui
  struct celula* urm;
} TCelula, *TLista;

typedef struct coada
{ 
  TLista inc, sf;       /* adresa primei si ultimei celule */
} TCoada; 

//  structura pentru celula cozii de tip 2, ce contine coordonatele nodului stocat
typedef struct celula_2
{ 
  int linie;
  int coloana;
  TArbore nod;
  struct celula_2* urm;
} TCelula_2, *TLista_2;

typedef struct coada_2
{ 
  TLista_2 inc, sf;       /* adresa primei si ultimei celule */
} TCoada_2;

TArbore Constr_Nod_0(int nivel);
TArbore Constr_Nod_1(unsigned char rosu, unsigned char verde, unsigned char albastru, int nivel);
void Nod_1(TArbore aux, unsigned char rosu, unsigned char verde, unsigned char albastru, int nivel);
void Nod_0(TArbore aux, int nivel);
Culori Culori_submatrice(int linie, int coloana, int size, Culori_ptr **grid);
void Constr_Arb(TArbore Arb, int linie, int coloana, int size, int prag, Culori_ptr **grid, int og_size, int *min, int *frunze, int *max, int nivel);
void Distruge_Arb(TArbore *Arb);
void Distruge_Arb_Aux(TArbore Arb);
unsigned long long power_2(unsigned long long x);
int AflaNrNiv(int size, int og_size);
TCoada* InitQ ();
void ExtrQ(TCoada*c);
int IntrQ(TCoada *c, TArbore Arb);
void DistrQ(TCoada **c);
TCoada_2* InitQ_2 ();
void ExtrQ_2(TCoada_2*c);
int IntrQ_2(TCoada_2 *c, TArbore Arb, int linie, int coloana);
void DistrQ_2(TCoada_2 **c);
void Comprimare(TArbore Arb, int height, const char *string);
int AflaSize(int og_size, int nivel);
void Umplere_Grid(TArbore nod, Culori_ptr **grid, int size, int linie, int coloana);