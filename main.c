    /*  RADU Anca - 315 CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "structs.h"


int main(int argc, char const *argv[]) {

    
    if (strncmp(argv[1], "-c", 2) == 0) {
        FILE *input = fopen(argv[3], "rb"); //	Se deschide fisierul de input
        if (input == NULL) {    //	Deschiderea fisierului esueaza
		    return 0;
	    }
        FILE *output1;
        if (strcmp(argv[1], "-c1") == 0) {
            output1 = fopen(argv[4], "wt"); //  Se deschide fisierul de output pt task 1
            if (output1 == NULL) {    //	Deschiderea fisierului esueaza
		        return 0;
	        }
        }

    int prag = atoi(argv[2]);
    char tip_imagine[2];
    char linie[256];
    int width, height;
    unsigned char max_value;

    fgets(linie, 256, input);   //  se citeste tipul imaginii
    tip_imagine[0] = linie[0];
    tip_imagine[1] = linie[1];
    fgets(linie, 256, input);   //  se citesc dimensiunile
    width = atoi(linie);
    height = width;
    fgets(linie, 256, input);   //  se citeste val maxima a culorilor
    max_value = atoi(linie);

    /*  Alocare spatiu pt matricea de culori   */
    int i, j;
    Culori_ptr **grid = malloc(height * sizeof(Culori_ptr*));
        if(!grid)   //  alocarea a esuat
            free(grid);
    for (i = 0; i < height; i++) {
        grid[i] = malloc(width * sizeof(Culori_ptr));
        if (!grid[i]) {  //  alocarea a esuat
            for(j = 0; j < i; j++)
                free(grid[j]);
            free(grid);
            break;
        }
    }
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++) {
            grid[i][j] = (Culori_ptr)malloc(sizeof(Culori));
            if(!grid[i][j]) {  //  alocarea a esuat
            for(i = 0; i < height; i++)
                free(grid[j]);
            free(grid);
            break;
        }
        }

    /*  Introducere informatie in matricea de culori   */
    for(i = 0; i < height; i++)
        for(j = 0; j < width; j++) {
            fread(&grid[i][j]->rosu, sizeof(unsigned char), 1, input); //se citesc culorile din fisier
            fread(&grid[i][j]->verde, sizeof(unsigned char), 1, input);
            fread(&grid[i][j]->albastru, sizeof(unsigned char), 1, input);
        }
    fclose(input);  //se inchide fisierul de input


    //  Crearea arborelui cuaternar

    TArbore Arb = (TArbore)malloc(sizeof(TNod));
    if (!Arb)   //  alocarea a esuat
        return 0;
    int *min = malloc(sizeof(int)); //  cea mai mica zona 
    int *frunze = malloc(sizeof(int));  //  nr de frunze
    int *max = malloc(sizeof(int)); //  cea mai mare zona nedivizata
    (*min) = height + 1;
    (*frunze) = 0;
    (*max) = -1; 
    Constr_Arb(Arb, 0, 0, height, prag, grid, height, min, frunze, max, 0);

    //  S-a creat arborele

    //  se elibereaza spatiul alocat pt matricea de culori
    for(i = 0; i < height; i++)
        for(j = 0; j < width; j++)
            free(grid[i][j]);
    for(i = 0; i < height; i++)
        free(grid[i]);
    free(grid);

    /*  OUTPUT TASK 1  */   
    if(strcmp(argv[1], "-c1") == 0) {
        int nr_niv = AflaNrNiv(*min, height);   //  nr de niveluri din arbore
        fprintf(output1, "%d\n%d\n%d\n", (nr_niv + 1), (*frunze), (*max));
    }


    /*  OUTPUT TASK 2  */
    if(strcmp(argv[1], "-c2") == 0){
        //  se comprima imaginea folosind arborele creat
        Comprimare(Arb, height, argv[4]);
    }

    free(min);
    free(frunze);
    free(max);
    if(strcmp(argv[1], "-c1") == 0)
        Distruge_Arb(&Arb); //  se elibereaza memoria alocata pt arbore
    if(strcmp(argv[1], "-c1") == 0)
        fclose(output1);    //  se inchide fisierul de output pt task 1
    }


    /*TASK 3*/

    if(strcmp(argv[1], "-d") == 0) {
        FILE *input = fopen(argv[2], "rb"); //	Se deschide fisierul de input
        if (input == NULL) {    //	Deschiderea fisierului esueaza
		    return 0;
	    }
        FILE *output = fopen(argv[3], "wb");
        if (output == NULL) {    //	Deschiderea fisierului esueaza
		    return 0;
	    }

        unsigned int size;
        unsigned char value, rosu_v, verde_v, albastru_v;
        TArbore Arb;
        //  se citeste dimensiunea imaginii
        fread(&size, sizeof(unsigned int), 1, input);

        //  se construieste primul nod al arborelui
        fread(&value, sizeof(unsigned char), 1, input);
        if(value == 1) {
            //  nodul curent este de tip 1
            //  deci e garantat ca urmeaza valorile culorilor
            fread(&rosu_v, sizeof(unsigned char), 1, input);
            fread(&verde_v, sizeof(unsigned char), 1, input);
            fread(&albastru_v, sizeof(unsigned char), 1, input);
            Arb = Constr_Nod_1(rosu_v, verde_v, albastru_v, 0);
        } else {
            Arb = Constr_Nod_0(0);
            TArbore finger = Arb;   //  pointer spre ultimul "tata" (ultimul nod de tip 0)
        
            //  initializam coada care ne ajuta sa cream arborele
            TCoada *c = InitQ();
            if(!c)
                return 0;
            IntrQ(c, Arb);  //  adaugam in coada o celula ce contine primul nod al arborelui

            int contor = 0;
            while(fread(&value, sizeof(unsigned char), 1, input)) {
                if(contor == 4) //  am lucrat cu toti fiii tatalui curent 
                    if(c->inc->urm) {
                        contor = 0;
                        finger = c->inc->urm->nod;  //  trecem la "tatal" urmator
                        ExtrQ(c);   //  eliberam din coada
                    }
                if(value == 0) {
                    //  construim nod de tip 0 pe nivelul urmator
                    finger->fiu[contor] = Constr_Nod_0(finger->nivel + 1);
                    //  introducem in coada doar nodurile tip 0 (care au fii)
                    IntrQ(c, finger->fiu[contor]);
                    contor++;
                }
                if(value == 1) {
                    //  nodul curent este de tip 1
                    //  deci e garantat ca urmeaza valorile culorilor
                    fread(&rosu_v, sizeof(unsigned char), 1, input);
                    fread(&verde_v, sizeof(unsigned char), 1, input);
                    fread(&albastru_v, sizeof(unsigned char), 1, input);
                    //  construim nodul
                    finger->fiu[contor] = Constr_Nod_1(rosu_v, verde_v, albastru_v, finger->nivel + 1);
                    contor++;
                } 
            }
            //  distrugem coada
            DistrQ(&c);
        }

        //  scriem in fisierul decomprimat
        //  tipul imaginii, inaltimea, lungimea,
        //  si valoarea max a culorilor
        fprintf(output, "P6\n%d %d\n255\n", size, size);

        //  initializam o coada de tip 2
        //  care contine si valori pentru indicii liniei si coloanei din matrice
        //  ce corespund nodului stocat in celula
        TCoada_2 *c = InitQ_2();
            if(!c) 
                return 0;
        IntrQ_2(c, Arb, 0, 0);
    
        int linie = 0, coloana = 0;
        int i, j;
        int small_size;

        //  initializam si alocam matricea de culori
        Culori_ptr **grid = malloc(size * sizeof(Culori_ptr*));
        if(!grid)   //  alocarea a esuat
            return 0;
        for(i = 0; i < size; i++)
            grid[i] = malloc(size * sizeof(Culori_ptr));
        for(i = 0; i < size; i++)
            for(j = 0; j < size; j++)
            grid[i][j] = (Culori_ptr)malloc(sizeof(Culori));

        if(Arb->tip == 1)
            //  se umple toata matricea cu aceleasi culori
            Umplere_Grid(Arb, grid, size, 0, 0);

        else {
            while(c->inc){
                //  se actualizeaza linia si coloana
                //  cu cea a nodului celulei curente din coada
                linie = c->inc->linie;
                coloana = c->inc->coloana;
                //  dimensiunea bucatii ocupate de un fiu
                small_size = AflaSize(size, c->inc->nod->nivel + 1);

                if(c->inc->nod->fiu[0])
                    if(c->inc->nod->fiu[0]->tip == 1)
                        //  se umple bucata frunzei
                        Umplere_Grid(c->inc->nod->fiu[0], grid, small_size, linie, coloana);
                    else
                        //  se introduce in coada un nou tata
                        //  celula din coada contine coordonatele
                        //  spre coltul de sus stanga al tatalui
                        IntrQ_2(c, c->inc->nod->fiu[0], linie, coloana);

                //  se repeta pentru fiecare nod
                //  coordonatele se introduc in functie de tipul fiului
                if(c->inc->nod->fiu[1])
                    if(c->inc->nod->fiu[1]->tip == 1)
                        Umplere_Grid(c->inc->nod->fiu[1], grid, small_size, linie, coloana + small_size);
                    else
                        IntrQ_2(c, c->inc->nod->fiu[1], linie, coloana + small_size);

                if(c->inc->nod->fiu[2])
                    if(c->inc->nod->fiu[2]->tip == 1)
                        Umplere_Grid(c->inc->nod->fiu[2], grid, small_size, linie + small_size, coloana + small_size);
                    else
                        IntrQ_2(c, c->inc->nod->fiu[2], linie + small_size, coloana + small_size);

                if(c->inc->nod->fiu[3])
                    if(c->inc->nod->fiu[3]->tip == 1)
                        Umplere_Grid(c->inc->nod->fiu[3], grid, small_size, linie + small_size, coloana);
                    else
                        IntrQ_2(c, c->inc->nod->fiu[3], linie + small_size, coloana);

                //  am terminat de lucrat cu "tatal" curent
                //  il extragem din coada
                ExtrQ_2(c);
            }
        }
        //  distrugem coada
        DistrQ_2(&c);

        //  scriem in fisier culorile stocate in matricea pe care am completat-o
        for(i = 0; i < size;  i++)
            for(j = 0; j < size; j++){
                fwrite(&grid[i][j]->rosu, sizeof(unsigned char), 1, output);
                fwrite(&grid[i][j]->verde, sizeof(unsigned char), 1, output);
                fwrite(&grid[i][j]->albastru, sizeof(unsigned char), 1, output);
            }
        
        //  eliberam spatiul alocat pentru matrice
        for(i = 0; i < size; i++)
            for(j = 0; j < size; j++)
                free(grid[i][j]);
        for(i = 0; i < size; i++)
            free(grid[i]);
        free(grid);

        //  eliberam spatiul alocat pentru arbore
        Distruge_Arb(&Arb);
        //  inchidem fisierele de input si output
        fclose(input);
        fclose(output);
    }

    return 0;
}