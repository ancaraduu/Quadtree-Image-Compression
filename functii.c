    /*  RADU Anca - 315 CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "structs.h"

//  Functie ce aloca memorie si contruieste un nod de tip 0 pe nivelul dat ca parametru
TArbore Constr_Nod_0(int nivel) {
    //  se aloca spatiul
    TArbore aux = (TArbore)malloc(sizeof(TNod));
    if (!aux)   //  alocarea a esuat
        return NULL;

    aux->tip = 0;   //  se initializeaza tipul nodului
    int i;
    for(i = 0; i < 4; i++)
        aux->fiu[i] = NULL;
    aux->nivel = nivel;
    return aux;
    }

//  Functie ce aloca memorie si contruieste un nod de tip 1 pe nivelul dat ca parametru
TArbore Constr_Nod_1(unsigned char rosu, unsigned char verde, unsigned char albastru, int nivel) {
    //  se aloca spatiul
    TArbore aux = (TArbore)malloc(sizeof(TNod));
    if (!aux)   //  alocarea a esuat
        return NULL;

    aux->tip = 1;   //  se initializeaza tipul nodului
    int i;
    for(i = 0; i < 4; i++)
        aux->fiu[i] = NULL;
    //  se aloca spatiu pentru vectorul de culori din nod
    aux->culoare = malloc(3 * sizeof(unsigned char));
        if(!aux->culoare)
            return NULL;      
    aux->culoare[0] = rosu;
    aux->culoare[1] = verde;
    aux->culoare[2] = albastru;
    aux->nivel = nivel;
    return aux;
    }

//  Functie ce contruieste nod de tip 0 pe nivelul dat ca parametru
void Nod_0(TArbore aux, int nivel) {

    aux->tip = 0;   //  se initializeaza tipul nodului
    int i;
    for(i = 0; i < 4; i++)
        aux->fiu[i] = NULL;
    aux->nivel = nivel;
    return;
}

//  Functie ce contruieste nod de tip 1 pe nivelul dat ca parametru
void Nod_1(TArbore aux, unsigned char rosu, unsigned char verde, unsigned char albastru, int nivel) {

    aux->tip = 1;   //  se initializeaza tipul nodului
    int i;
    for(i = 0; i < 4; i++)
        aux->fiu[i] = NULL;
    //  se aloca spatiu pentru vectorul de culori din nod
    aux->culoare = malloc(3 * sizeof(unsigned char));
        if(!aux->culoare)
            return;      
    aux->culoare[0] = rosu;
    aux->culoare[1] = verde;
    aux->culoare[2] = albastru;
    aux->nivel = nivel;
    return;
    }

//  Functia ce construieste arborele cuaternar
void Constr_Arb(TArbore Arb, int linie, int coloana, int size, int prag, Culori_ptr **grid, int og_size, int *min, int *frunze, int *max, int nivel) {
    int i, j;
    unsigned long long mean;

    //  ne oprim daca coordonatele depasesc limitele matricei
    if(linie > og_size || coloana > og_size)
        return;

    //  se calculeaza mediile culorilor din regiunea aferenta nodului
    Culori_ptr aux = (Culori_ptr)malloc(sizeof(Culori));
    *aux = Culori_submatrice(linie, coloana, size, grid); 
    //  se calculeaza scorul folosind formula
    mean = 0;
    for(i = linie; i < linie + size; i++)
        for(j = coloana; j < coloana + size; j++)
            mean = mean + power_2(aux->rosu - grid[i][j]->rosu) + power_2(aux->verde - grid[i][j]->verde) + power_2(aux->albastru - grid[i][j]->albastru);
    mean = ((mean / 3) / size) / size;
    mean = (int)mean;

    if (mean <= prag) { //  scorul respecta limitele
        (*frunze) = (*frunze) + 1;  //  creste numarul de noduri frunze
        if(size > (*max))
            (*max) = size;
        if(size < (*min))
            (*min) = size;
        //  nodul devine de tip 1
        Nod_1(Arb, (unsigned char)aux->rosu, (unsigned char)aux->verde, (unsigned char)aux->albastru, nivel);
    }
    else {  //scorul nu respecta limitele, se imparte pixelul in alti 4
        //  nodul devine de tip 0
        Nod_0(Arb, nivel);
        //  alocam memorie pentru copiii nodului
        for(i = 0; i < 4; i++) {
            Arb->fiu[i] = (TArbore)malloc(sizeof(TNod));
            if (!Arb->fiu[i])   //  alocarea a esuat
                return;
        }
        //  apelam functia pentru ficare fiu, mutand coordonatele si impartind dimensiunea la 2
        Constr_Arb(Arb->fiu[0], linie, coloana, size / 2, prag, grid, og_size, min, frunze, max, nivel + 1);
        Constr_Arb(Arb->fiu[1], linie, coloana + size / 2, size / 2, prag, grid, og_size, min, frunze, max, nivel + 1);
        Constr_Arb(Arb->fiu[2], linie + size / 2, coloana + size / 2, size / 2, prag, grid, og_size, min, frunze, max, nivel + 1);
        Constr_Arb(Arb->fiu[3], linie + size / 2, coloana, size / 2, prag, grid, og_size, min, frunze, max, nivel + 1);
    }
    free(aux);
    return;
}

//  Functie ce calculeaza mediile pentru fiecare culoare dintr-o anumita regiune din matrice
Culori Culori_submatrice(int linie, int coloana, int size, Culori_ptr **grid) {
    unsigned long long rosu = 0, verde = 0, albastru = 0;
    int i, j;
    //  se face suma fiecarei culori din toate elementele din regiunea ceruta
    for(i = linie; i < linie + size; i++)
        for(j = coloana; j < coloana + size; j++) {
            rosu = rosu + grid[i][j]->rosu;
            verde = verde + grid[i][j]->verde;
            albastru = albastru + grid[i][j]->albastru;
        }
    rosu = (rosu / size);
    if(rosu)
        rosu = (rosu / size);
    rosu = (int)rosu;

    verde = (verde / size);
    if(verde)
        verde = (verde / size);
    verde = (int)verde;

    albastru = (albastru / size);
    if(albastru)
        albastru = (albastru / size);
    albastru = (int)albastru;
    Culori aux;
    aux.rosu = rosu;
    aux.verde = verde;
    aux.albastru = albastru;
    return aux;
}

//  Functia de distrugere a arborelui
void Distruge_Arb(TArbore *Arb){
    if (!(*Arb))
        return;

    Distruge_Arb_Aux(*Arb);
    *Arb = NULL;
}

//  Functia auxiliara de distrugere a arborelui
void Distruge_Arb_Aux(TArbore Arb) {
    if (!Arb)
        return;

    int i;
    for(i = 0; i < 4; i++) {
        if(Arb->fiu[i]) {
            //  apelam functia pentru fiecare fiu al nodului
            Distruge_Arb_Aux(Arb->fiu[i]);
        }
    }
    //  eliberam memoria pentru vectorul de culori
    if(Arb->tip == 1)
        free(Arb->culoare);
    //  eliberam memoria alocata pentru nod
    free(Arb);
}

//  Funtie  ce ridica un numar la puterea a doua
unsigned long long power_2(unsigned long long x){
    x = x * x;
    return x;
}

//  Functie ce afla nivelul pe care se afla un nod in functie de
//  dimensiunea nodului si dimensiunea imaginii mari
int AflaNrNiv(int size, int og_size) {
    int nivel = 0;

    while(size != og_size){
        nivel++;
        og_size = og_size / 2; 
    }
    return nivel;
}
//  Creeaza coada vida
TCoada* InitQ () { 
  TCoada* c;          /* spatiu pentru descriptor coada */
  c = (TCoada*)malloc(sizeof(TCoada));
  if (! c)
    return NULL;                  /* nu exista spatiu -> "esec" */

  c->inc = c->sf = NULL;
  return c;          /* intoarce adresa descriptorului cozii */
}

//  Adauga element la sfarsitul cozii
int IntrQ(TCoada *c, TArbore nod) {
  TLista aux;
  aux = (TLista)malloc(sizeof(TCelula));      /* aloca o noua celula */
  if ( ! aux) return 0;             /* alocare imposibila -> "esec" */

  aux->nod = nod; aux->urm = NULL;

  if (c->sf != NULL)          /* coada nevida */
    c->sf->urm = aux;                   /* -> leaga celula dupa ultima din coada */
  else                              /* coada vida */
    c->inc = aux;                    /* -> noua celula se afla la inceputul cozii */
  c->sf = aux;  	            /* actualizeaza sfarsitul cozii */
  return 1;                         /* operatie reusita -> "succes" */
}

// Sterge primul element din coada
void ExtrQ(TCoada*c) {
    TLista aux;
    if(!c->inc)
        return;

    aux = c->inc;   //  retinem inceputul cozii
    c->inc = aux->urm;  //  actualizam inceputul cozii
    if(!c->inc) //  coada nu mai are niciun element
        c->sf = NULL;   //->actualizam sfarsitul
    free(aux);  //  eliberam memoria ocupata de celula stearsa
    return;         
}

//  Distruge coada
void DistrQ(TCoada **c) {
  TLista p, aux;
  p = (*c)->inc;
  while(p)
  {
    aux = p;
    p = p->urm;
    free(aux);
  }
  free(*c);
  *c = NULL;
}

//  Creeaza coada vida de tipul 2
TCoada_2* InitQ_2 () { 
  TCoada_2* c;          /* spatiu pentru descriptor coada */
  c = (TCoada_2*)malloc(sizeof(TCoada_2));
  if (! c)
    return NULL;                  /* nu exista spatiu -> "esec" */

  c->inc = c->sf = NULL;
  return c;          /* intoarce adresa descriptorului cozii */
}

// Adauga element la sfarsitul cozii de tip 2
int IntrQ_2(TCoada_2 *c, TArbore nod, int linie, int coloana) {
  TLista_2 aux;
  aux = (TLista_2)malloc(sizeof(TCelula_2));      /* aloca o noua celula */
  if ( ! aux) return 0;             /* alocare imposibila -> "esec" */

  aux->nod = nod;
  aux->linie = linie;
  aux->coloana = coloana;
  aux->urm = NULL;

  if (c->sf != NULL)          /* coada nevida */
    c->sf->urm = aux;                   /* -> leaga celula dupa ultima din coada */
  else                              /* coada vida */
    c->inc = aux;                    /* -> noua celula se afla la inceputul cozii */
  c->sf = aux;  	            /* actualizeaza sfarsitul cozii */
  return 1;                         /* operatie reusita -> "succes" */
}

// Sterge primul element din coada de tip 2
void ExtrQ_2(TCoada_2*c) {
    TLista_2 aux;
    if(!c->inc)
        return;
    
    aux = c->inc;   //  retinem inceputul cozii
    c->inc = aux->urm;  //  actualizam inceputul cozii
    if(!c->inc) //  coada nu mai are niciun element
        c->sf = NULL;   //->actualizam sfarsitul
    free(aux);  //  eliberam memoria ocupata de celula stearsa
    return;         
}

// Distruge coada de tip 2
void DistrQ_2(TCoada_2 **c) {
  TLista_2 p, aux;
  p = (*c)->inc;
  while(p)
  {
    aux = p;
    p = p->urm;
    free(aux);
  }
  free(*c);
  *c = NULL;
}

//  Functia ce creeaza fisierul de comprimare a unei imagini
void Comprimare(TArbore Arb, int height, const char *string) {
    //  se deschide fisierul de output
    FILE *output2 = fopen(string, "wb");
        if (output2 == NULL)    //	Deschiderea fisierului esueaza
		    return;
    //  Se scrie dimensiunea imaginii
    fwrite(&height, sizeof(unsigned int), 1, output2);
    //  se initializeaza coada ce ne va ajuta sa parcurgem arborele
    TCoada *c = InitQ();
    if (!c)
        return;
    //  se introduce in arbore prima celula, ce contine radacina arborelui
    IntrQ(c, Arb);
    int nivel;

    if (c->inc) {
        nivel = c->inc->nod->nivel;
    }
    else
        return;

    while (c->inc){
        //  se scrie in fisier tipul nodului
        fwrite(&c->inc->nod->tip, sizeof(unsigned char), 1, output2);
        //  daca e nod de tip 1, se scriu in fisier culorile sale
        if (c->inc->nod->tip == 1) {
            fwrite(&c->inc->nod->culoare[0], sizeof(unsigned char), 1, output2);
            fwrite(&c->inc->nod->culoare[1], sizeof(unsigned char), 1, output2);
            fwrite(&c->inc->nod->culoare[2], sizeof(unsigned char), 1, output2);
        }

        //  se introduc in coada fiii nodului
        if (c->inc->nod->tip == 0) {
            IntrQ(c, c->inc->nod->fiu[0]);
            IntrQ(c, c->inc->nod->fiu[1]);
            IntrQ(c, c->inc->nod->fiu[2]);
            IntrQ(c, c->inc->nod->fiu[3]);
        }
    TArbore aux = c->inc->nod;
    //  se extrage nodul din coada
    ExtrQ(c);
    if(aux->tip == 1)
        free(aux->culoare);
    //  eliberam memoria alocata pentru nod
    free(aux);
    }
    //  se distruge coada
    DistrQ(&c);
    //  se inchide fisierul de output 
    fclose(output2);
    return;
}

//  Functie ce afla dimensiunea ocupata de un nod in functie de
//  nivelul pe care se afla in arbore si dimensiunea imaginii mari
int AflaSize(int og_size, int nivel) {
    int i, size = og_size;
    for (i = 0; i < nivel; i++)
        size = size / 2;
    return size;
}

//  Functie ce completeaza o regiune din matricea de culori
void Umplere_Grid(TArbore nod, Culori_ptr **grid, int size, int linie, int coloana) {
    int i, j;
    //  parcurgem regiunea data de coordonatele si dimensiunea transmise ca parametrii
    for (i = linie; i < linie + size; i++)
        for (j = coloana; j < coloana + size; j++) {
            //  umplem regiunea cu valorile culorilor transmise
            grid[i][j]->rosu = nod->culoare[0];
            grid[i][j]->verde = nod->culoare[1];
            grid[i][j]->albastru = nod->culoare[2];
        }
    return;
}