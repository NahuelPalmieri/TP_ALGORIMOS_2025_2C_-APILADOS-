#ifndef TDA_LISTA_DOB_H_INCLUDED
#define TDA_LISTA_DOB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbol.h"
#define SIN_MEM -1
#define TODO_OK 1
#define OK 0
#define MINIMO(X,Y) ((X<Y)?X:Y)
#define NUEVO_REGISTRO 4
#define REGISTRO_ENCONTRADO 5

typedef struct sNodoLista{
    void * info;
    unsigned tamElem;
    struct sNodoLista * sig;
    struct sNodoLista * ant;
}tNodoLista;

typedef struct{
    int id;
    int puntuacion;
}sRanking;

typedef tNodoLista * tLista;

char * procesarRanking(char * nombArch, char * cad);
int cargarListaDesdeArch(tLista * lista, FILE * pf);
int cmpPuntuacion(void * e1, void * e2);
void extraer5Prim(tLista * lista, char * cad, FILE * pf);

void crearLista(tLista * pl);
int insertarEnListaOrdenado(tLista * pl, void * dato, unsigned tamDato,int (*cmp) (void*,void*));
int sacarPrimeroDeLista(tLista * p, void * d,unsigned cantBytes);
int liberarLista(tLista * pl);


#endif // TDA_LISTA_DOB_H_INCLUDED
