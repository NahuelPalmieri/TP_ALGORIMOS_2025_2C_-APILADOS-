#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

//#include "servidor_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIN_MEM -1
#define TODO_OK 1
#define OK 0
#define ERR_ARCH -2
#define MINIMO2(X,Y) ((X<Y)?X:Y)
#define NUEVO_REGISTRO 4
#define REGISTRO_ENCONTRADO 5

#define NOMB_ARCH_USU "ArchUsuarios.dat"
#define NOMB_ARCH_PARTIDAS "ArchPartidas.dat"
#define NOMB_ARCH_IND "Indice.dat"

 typedef struct sNodoArbol{
    void * info;
    unsigned tamElem;
    struct sNodoArbol * menor;
    struct sNodoArbol * mayor;
}tNodoArbol;

typedef struct{
    int id;
    char nombre[11];
    int pos;
}sIndice;

typedef struct{
    int id;
    char nombre[11];
    int puntuacion;
    int movimientos;
}sUsuario;

typedef struct{
    int id;
    int id_usuario;
    int puntuacion;
    int movimientos;
}sPartida;

typedef struct sNodoArbol * tArbol;
typedef int (*Cmp)(const void*,const void*);
typedef void (*Accion)(const void*, const void*);


int manejarArchUsuarios(char * nombreArch, char * nombreArchPartida,char * nombreArchIndice, char * infoUsuario, tArbol * arbol);
int leerArchBinario(char * nombreArch);
int leerArchBinarioPartida(char * nombreArch);
int leerArchBinarioIndice(char * nombreArch);
int regenerarArbol(tArbol * arbol, char * nombreArchIndice);

//primitivas arbol
void crearArbol(tArbol * pa);
int destruirArbol(tArbol * arbol);
int arbolVacio(tArbol * pa);
int _insertarEnArbolR(tArbol * pa, void * info, unsigned tamElem,Cmp);
void _arbolInOrden(tArbol*pa,Accion accion);
int arbolInOrden(tArbol*pa);
void mostrarInd(const void * elem, const void * aux);
int insertarEnArbolR(tArbol * pa, void * info, unsigned tamElem);
int nivelArbol(tArbol * arbol);

//buscar en arbol
tNodoArbol ** buscarNodoEnArbolRxClave(tArbol * pa, void * buscado, Cmp cmp);
int buscarElemEnArbolRxClave(tArbol * pa, void * info, unsigned tamElem, Cmp cmp);
tNodoArbol ** buscarNodoEnArbolRxNoClave(tArbol * pa, void * info, Cmp cmp);
int buscarElemEnArbolRxNOClave(tArbol * pa, void * info, unsigned tamElem, Cmp cmp);

//proceso de crear el indice
int insertarArchEnArbol(tArbol * arbol, char*nombreArch);
int ArchDesdeArbolInOrden(tArbol*pa, char * nombreArch);
void _arbolInOrdenArch(tArbol*pa,FILE * pf, Accion accion);
void insertarEnArch(const void * ind, const void * pf);
int ArbolBalanceadoDesdeInd(tArbol * arbol, char * nombreArch);
int genArbolBBBalanceado(tArbol * arbol, int ini, int fin, FILE * pf);
int cargarDatosOrdenados(tArbol *pa, void *ds, int(*leer)(void *,
                                    void *, unsigned, unsigned tamDato), int li ,int ls,unsigned tamDato);
int arbolCargarArchivoOrdenado(tArbol *pa, const char *path, unsigned tamDato);

int crearArchIndice(tArbol * arbol,char * nombreArch);
int mostrarArchBinInd(char * nombreArch);

//auxiliares
int cmpNombre(const void * e1, const void * e2);


#endif // ARBOL_H_INCLUDED
