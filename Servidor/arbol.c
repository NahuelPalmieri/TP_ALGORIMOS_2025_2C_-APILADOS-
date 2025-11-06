#include "arbol.h"

int cmpInt(const void * elem1,const void * elem2)
{
    int * e1 = (int*) elem1;
    int * e2 = (int*) elem2;
    return (*e1) - (*e2);
}

int arbolInOrden(tArbol*pa)
{
    if(*pa == NULL)
   {
       return 0; //vacio
   }
    _arbolInOrden(pa,mostrarInd);
    return 1;
}

void mostrarInd(const void * elem, const void * aux)
{
   sIndice * e = (sIndice*) elem;
   printf("%d %-10s %d\n",e->id,e->nombre,e->pos);
}

int cmpNombre(const void * e1,const void * e2)
{
    sIndice * elem1 = (sIndice*)e1;
    sIndice * elem2 = (sIndice*)e2;
    return strcmpi(elem1->nombre,elem2->nombre);
}

int regenerarArbol(tArbol * arbol, char * nombreArchIndice)
{
    ///Indice - si puede abrirlo es xq existe, sino no lo regenera.
    FILE * pfInd = fopen(nombreArchIndice, "rb");
    if(!pfInd)
    {
        return ERR_ARCH;
    }
    fclose(pfInd);
    crearArbol(arbol);
    ArbolBalanceadoDesdeInd(arbol,nombreArchIndice);
    return TODO_OK;
}

int manejarArchUsuarios(char * nombreArch, char * nombreArchPartida,char * nombreArchIndice, char * infoUsuario, tArbol * arbol)
{
    int rtdo, ultIdPartida;
    sPartida ultPartida;
    sPartida partida;
    sUsuario usuario;
    sUsuario usuarioAinsetar;
    sIndice indice;

    regenerarArbol(arbol,nombreArchIndice);

    ///indice
    FILE * pfInd = fopen(nombreArchIndice, "a+b");
    if(!pfInd)
    {
        return ERR_ARCH;
    }
    ///usuario
    FILE * pf = fopen(nombreArch, "a+b");
    if(!pf)
    {
        return ERR_ARCH;
    }
    ///partida
    FILE * pfPartida = fopen(nombreArchPartida, "r+b");
    if(!pfPartida)
        {
            pfPartida = fopen(nombreArchPartida,"w+b");
            if(!pfPartida)
            {
                return ERR_ARCH;
            }
        }

    sscanf(infoUsuario,"%d|%d|%11[^\n]",&usuarioAinsetar.puntuacion, &usuarioAinsetar.movimientos, usuarioAinsetar.nombre);

    ultPartida.movimientos = usuarioAinsetar.movimientos;
    ultPartida.puntuacion = usuarioAinsetar.puntuacion;

    fseek(pf,(long)sizeof(sUsuario) * -1, SEEK_END);
    if(fread(&usuario,sizeof(sUsuario),1,pf) != 1)
    {   printf("\nPrimer Usuario\n");
        //se que es el primer elem del archivo y del arbol.
        usuario.id = 1;
        ultPartida.id_usuario = usuario.id;
        usuarioAinsetar.id = usuario.id;
        //inserto en el arbol y en los archivos(al final)
        fwrite(&usuarioAinsetar,sizeof(sUsuario),1,pf);

        indice.id = 1;
        strcpy(indice.nombre, usuarioAinsetar.nombre);
        indice.pos=0;
        fwrite(&indice,sizeof(sIndice),1,pfInd);
        crearArbol(arbol);
        insertarEnArbolR(arbol,&indice,sizeof(sIndice));
    }
    else
    {
        //busco si ya esta en el arbol, para saber si esta en los archivos(usuario e indice)
        strcpy(indice.nombre,usuarioAinsetar.nombre);
        if(buscarElemEnArbolRxNOClave(arbol,&indice,sizeof(sIndice),cmpNombre) == 1)//busco x nombre y si lo encuentra, me dev los datos cargados
        {
            printf("\nUsuario encontrado\n");
            //si lo encontro, tengo que modificar el arch usuario, lo cierro y lo abro para modificar.
            fclose(pf);
            pf = fopen(nombreArch,"r+b");
            if(!pf)
            {
                return ERR_ARCH;
            }

            fseek(pf,(long)sizeof(sUsuario) * indice.pos,SEEK_SET);
            fread(&usuario,sizeof(sUsuario),1,pf);
            usuarioAinsetar.id = usuario.id;

            ultPartida.id_usuario = usuario.id;

            usuarioAinsetar.movimientos += usuario.movimientos;
            usuarioAinsetar.puntuacion += usuario.puntuacion;
            fseek(pf,(long)sizeof(sUsuario)* -1,SEEK_CUR);
            fwrite(&usuarioAinsetar,sizeof(sUsuario),1,pf);
        }
        else //si no lo encontro, es un nuevo elem del indice y del arch usuarios.
        {
            printf("\nNuevo Usuario\n");
            fseek(pf,(long)sizeof(sUsuario) * -1, SEEK_END);
            fread(&usuario,sizeof(sUsuario),1,pf);
            usuarioAinsetar.id = usuario.id + 1;
            ultPartida.id_usuario = usuario.id+1;
            indice.id = usuarioAinsetar.id;
            strcpy(indice.nombre, usuarioAinsetar.nombre);
            indice.pos = indice.id - 1;
            fwrite(&indice,sizeof(sIndice),1,pfInd);
            fwrite(&usuarioAinsetar,sizeof(sUsuario),1,pf);
            destruirArbol(arbol);
            crearArbol(arbol);
            fclose(pfInd);
            ArbolBalanceadoDesdeInd(arbol,nombreArchIndice);
        }
    }

    ///ARCHIVO PARTIDA
    fread(&partida,sizeof(sPartida),1,pfPartida);
    if(feof(pfPartida))
    {
        //cargar datos de la primer partida
        partida.id = 1;
        ultPartida.id = partida.id;
        fwrite(&ultPartida,sizeof(sPartida),1,pfPartida);
        //INGRESO EL PRIMER REGISTRO
    }
    else
    {
        fseek(pfPartida,(long)sizeof(sPartida) * -1,SEEK_CUR);
        //cargar datos del nuevo ultimo usuario
        while((rtdo = fread(&partida,sizeof(sPartida),1,pfPartida)) == 1)
        {
            ultIdPartida = partida.id;
        }
        ultPartida.id = ultIdPartida + 1;
        fwrite(&ultPartida,sizeof(sPartida),1,pfPartida);
    }

    fclose(pfInd);
    fclose(pfPartida);
    fclose(pf);
    return 1;
}

int leerArchBinarioIndice(char * nombreArch)
{
    FILE * pf = fopen(nombreArch, "rb");
    if(!pf)
    {
        return 0;
    }
    sIndice indice;
    fread(&indice,sizeof(sIndice),1,pf);
    printf("id_usuario - nombre - posicion\n");
    while(!feof(pf))
    {
        printf("%-10d - %-10s - %-10d\n",indice.id,indice.nombre,indice.pos);
        fread(&indice,sizeof(sIndice),1,pf);
    }
    fclose(pf);
    return 1;
}

int leerArchBinario(char * nombreArch)
{
    FILE * pf = fopen(nombreArch, "rb");
    if(!pf)
    {
        return 0;
    }
    sUsuario usuario;
    fread(&usuario,sizeof(sUsuario),1,pf);
    printf("id_usuario - nombres - puntuacion - movimientos\n");
    while(!feof(pf))
    {
        printf("%-10d - %-10s - %-10d - %-10d\n",usuario.id,usuario.nombre,usuario.puntuacion, usuario.movimientos);
        fread(&usuario,sizeof(sUsuario),1,pf);
    }
    fclose(pf);
    return 1;
}

int leerArchBinarioPartida(char * nombreArch)
{
    FILE * pf = fopen(nombreArch, "rb");
    if(!pf)
    {
        return 0;
    }
    sPartida partida;
    fread(&partida,sizeof(sPartida),1,pf);
    printf("id_partida - id_usuario - puntuacion - movimientos\n");
    while(!feof(pf))
    {
        printf("%-10d - %-10d - %-10d - %-10d\n",partida.id,partida.id_usuario, partida.puntuacion, partida.movimientos);
        fread(&partida,sizeof(sPartida),1,pf);
    }
    fclose(pf);
    return 1;
}


void insertarEnArch(const void * indice,const void * arch)
{
    FILE * pf = (FILE*)arch;
    sIndice * ind = (sIndice*)indice;

    fwrite(ind,sizeof(sIndice),1,pf);
}

void _arbolInOrdenArch(tArbol*pa,FILE * pf, Accion accion)
{
   if(*pa == NULL)
   {
       return;
   }

   _arbolInOrdenArch(&(*pa)->menor,pf,accion);
   accion((*pa)->info, pf);
   _arbolInOrdenArch(&(*pa)->mayor,pf,accion);
}

int ArchDesdeArbolInOrden(tArbol*pa, char * nombreArch)
{
    FILE * pf = fopen(nombreArch, "wb");
    if(!pf)
    {
        return 0;
    }

    _arbolInOrdenArch(pa, pf, insertarEnArch);

    fclose(pf);
    return 1;
}


int genArbolBBBalanceado(tArbol * arbol, int ini, int fin, FILE * pf)
{
    int m = (ini+fin)/2, r;

    if(ini > fin)
    {
        return OK;
    }

    tNodoArbol * nue = (tNodoArbol*)malloc(sizeof(tNodoArbol));
    if(!nue)
    {
        return SIN_MEM;
    }
    nue->info = malloc(sizeof(sIndice));
    if(!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }

    fseek(pf,m * sizeof(sIndice),SEEK_SET);
    fread(nue->info,sizeof(sIndice),1,pf);
    nue->tamElem = sizeof(sIndice);
    nue->mayor = NULL;
    nue->menor = NULL;
    *arbol = nue;

    if((r = genArbolBBBalanceado(&(*arbol)->menor, ini, m-1,pf))!=OK)
    {
        return r;
    }
    return genArbolBBBalanceado(&(*arbol)->mayor,m+1,fin,pf);
}

int ArbolBalanceadoDesdeInd(tArbol * arbol, char * nombreArch)
{
    FILE * pf = fopen(nombreArch,"rb");
    if(!pf)
    {
        return 0;
    }
    fseek(pf,0,SEEK_END);
    int fin = ftell(pf)/sizeof(sIndice) - 1;
    genArbolBBBalanceado(arbol,0,fin,pf);

    fclose(pf);
    return 1;
}
///FUNCIONES PRIMITIVAS DE ARBOL#######################################################################3
void crearArbol(tArbol * pa)
{
    *pa = NULL;
}

int destruirArbol(tArbol * arbol)
{
    if(!*arbol)
    {
        return 0; //arbol vacio
    }
    if(!(*arbol)->menor && !(*arbol)->menor)
    {
        free(*arbol);
        *arbol = NULL;
    }
    else
    {
        destruirArbol(&(*arbol)->menor);
        destruirArbol(&(*arbol)->mayor);
    }
    return 1;
}

int arbolVacio(tArbol * pa)
{
    if(!*pa)
    {
        return 1;//arbol vacio
    }
    return 0;//arbol no vacio
}

int insertarEnArbolR(tArbol * pa, void * info, unsigned tamElem)
{
    return _insertarEnArbolR(pa,info,tamElem,cmpInt);
}

int _insertarEnArbolR(tArbol * pa, void * info, unsigned tamElem, Cmp cmp)
{
    if(*pa != NULL)
    {
        if(cmp((*pa)->info,info) > 0)
        {
            return _insertarEnArbolR(&(*pa)->menor, info, tamElem,cmp);
        }
        else
        {
            if(cmp((*pa)->info,info) < 0)
            {
                return _insertarEnArbolR(&(*pa)->mayor, info, tamElem,cmp);
            }
            else
            {
                return -1;//repetido
            }
        }
    }
        tNodoArbol * nue = (tNodoArbol*)malloc(sizeof(tNodoArbol));
        if(!nue)
        {
            return SIN_MEM;
        }
        nue->info = malloc(sizeof(tamElem));
        if(!nue->info)
        {
            free(nue);
            return SIN_MEM;
        }
        memcpy(nue->info,info,tamElem);
        nue->tamElem = tamElem;
        nue->mayor = NULL;
        nue->menor = NULL;
        *pa = nue;
    return TODO_OK;
}

tNodoArbol ** buscarNodoEnArbolRxClave(tArbol * pa, void * buscado, Cmp cmp) //devuelve el nodo encontrado
{
    if(*pa != NULL)
    {
        if(cmp((*pa)->info,buscado) > 0)
        {
            return buscarNodoEnArbolRxClave(&(*pa)->menor, buscado,cmp);
        }
        else
        {
            if(cmp((*pa)->info,buscado) < 0)
            {
                return buscarNodoEnArbolRxClave(&(*pa)->mayor, buscado,cmp);
            }
            else
            {
                return pa; //lo encontro
            }
        }
    }
    return NULL; //no lo encontro
}

int buscarElemEnArbolRxClave(tArbol * pa, void * info, unsigned tamElem, Cmp cmp)
{
    if((pa = buscarNodoEnArbolRxClave(pa,info,cmp)))
    {
        memcpy(info,(*pa)->info,MINIMO2(tamElem,(*pa)->tamElem));
        return 1;
    }
    return 0;
}

tArbol * buscarNodoEnArbolRxNoClave(tArbol * pa, void * buscado, Cmp cmp) //devuelve el nodo encontrado
{
    tArbol * busq = NULL;
    if(!*pa)
    {
        return NULL;
    }
    if(cmp((*pa)->info,buscado)==0)
    {
        return pa;
    }
    else
    {
        if((busq = buscarNodoEnArbolRxNoClave(&(*pa)->menor,buscado,cmp)))
        {
            return busq;
        }
        else
        {
           if((busq = buscarNodoEnArbolRxNoClave(&(*pa)->mayor,buscado,cmp)))
           {
                return busq;
           }
        }
    }
    return NULL; //no lo encontro
}


int buscarElemEnArbolRxNOClave(tArbol * pa, void * info, unsigned tamElem, Cmp cmp)
{
    if((pa = buscarNodoEnArbolRxNoClave(pa,info,cmp)))
    {
        memcpy(info,(*pa)->info,MINIMO2(tamElem,(*pa)->tamElem));
        return 1;
    }
    return 0;
}

void _arbolInOrden(tArbol*pa,Accion accion)
{
   if(*pa == NULL)
   {
       return;
   }

   _arbolInOrden(&(*pa)->menor,accion);
   accion((*pa)->info,(*pa)->info);
   _arbolInOrden(&(*pa)->mayor,accion);
}


int nivelArbol(tArbol * arbol)
{
    int hi=-1,hd=-1;
    if(*arbol == NULL)
    {
        return -1;
    }
    hi = nivelArbol(&(*arbol)->menor);
    hd = nivelArbol(&(*arbol)->mayor);
    return (hi>hd?(hi+1):(hd+1));
}
