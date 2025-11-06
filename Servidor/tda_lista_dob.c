#include "tda_lista_dob.h"

int cmpPuntuacion(void * e1, void * e2)
{
    sRanking * elem1 = (sRanking*)e1;
    sRanking * elem2 = (sRanking*)e2;
    return elem1->puntuacion - elem2->puntuacion;
}

char * procesarRanking(char * nombArch, char * cad)
{
    tLista lista;

    FILE * pf = fopen(nombArch, "rb"); //ArchUsuario
    if(!pf)
    {
        //no hay archivo usuario
        return NULL;
    }
    crearLista(&lista);
    strcpy(cad,"");
    cargarListaDesdeArch(&lista,pf);
    fseek(pf,0,SEEK_SET);
    extraer5Prim(&lista, cad, pf);
    liberarLista(&lista);
    fclose(pf);
    return cad;
}

void extraer5Prim(tLista * lista, char * cad, FILE * pf)
{
    int i;
    sRanking rank;
    sUsuario usuario;
    char aux[21];
    for(i = 0; i < 5 ; i++)
    {
        if(*lista)
        {
            sacarPrimeroDeLista(lista,&rank,sizeof(sRanking));
            fseek(pf, (rank.id - 1) * sizeof(sUsuario),SEEK_SET);
            fread(&usuario, sizeof(sUsuario),1,pf);
            sprintf(aux,"%d;%s;%d",usuario.id,usuario.nombre,usuario.puntuacion);
            strcat(cad,aux);
            strcat(cad,"|");
        }
    }
}

int cargarListaDesdeArch(tLista * lista, FILE * pf)
{
    sUsuario usuario;
    sRanking ranking;
    fread(&usuario,sizeof(sUsuario),1,pf);
    while(!feof(pf))
    {
        ranking.id = usuario.id;
        ranking.puntuacion = usuario.puntuacion;
        insertarEnListaOrdenado(lista,&ranking,sizeof(sRanking),cmpPuntuacion);
        fread(&usuario,sizeof(sUsuario),1,pf);
    }
    return 1;
}

void crearLista(tLista * pl)
{
    *pl = NULL;
}

int insertarEnListaOrdenado(tLista * pl, void * dato, unsigned tamDato,int (*cmp) (void*,void*))
{
    tNodoLista * actual = *pl;
    tNodoLista * auxSig;
    tNodoLista * auxAnt;
    tNodoLista * nue;

    if(!actual)//si es el primer elem insertado
    {
        auxAnt=NULL;
        auxSig=NULL;
    }
    else //acomodo donde voy a insetar
    {       //siguiente no sea nulo y
        while(actual->sig && cmp(dato,actual->info)<0)//dato sea menor a su sig
        {
            actual = actual->sig; //me muevo a derecha
        }
        //anterior no sea nulo y
        while(actual->ant && cmp(dato,actual->info)>0)//dato sea mayor a su anterior
        {
            actual = actual->ant; //me muevo a izq
        }

        if(cmp(dato,actual->info)<0)//verifico x que condicion sali y veo donde acomodo a donde quede parado
        {
            //dato es mayor al actual (donde quede parado)
            auxAnt = actual;
            auxSig = actual->sig;
        }
        else
        {
            //dato es menor al actual (donde quede parado)
            auxAnt = actual->ant;
            auxSig = actual;
        }
    }
    nue = (tNodoLista*)malloc(sizeof(tNodoLista));
    if(!nue)
    {
        return -2;
    }
    nue->info = malloc(tamDato);
    if(!nue->info)
    {
        free(nue);
        return -2;
    }
    memcpy(nue->info,dato,tamDato);
    nue->tamElem = tamDato;
    nue->sig = auxSig;
    nue->ant = auxAnt;
    //engancho la lista al nuevo nodo
    if(auxAnt != NULL)
    {
        auxAnt->sig = nue;
    }
    if(auxSig != NULL)
    {
        auxSig->ant = nue;
    }
    *pl = nue;

    return TODO_OK;
}


int sacarPrimeroDeLista(tLista* pl, void* elem, unsigned tamElem) {
    if (*pl == NULL)
        return 0;

    // Ir al primer nodo (extremo izquierdo)
    tNodoLista* head = *pl;
    while (head->ant)
        head = head->ant;

    // Copiar datos
    memcpy(elem, head->info, MINIMO(head->tamElem, tamElem));

    // Quitar el nodo de la lista
    tNodoLista* next = head->sig;
    if (next)
        next->ant = NULL;

    // Si el puntero principal apuntaba al nodo que se borra, actualizarlo
    if (*pl == head)
        *pl = next;

    free(head->info);
    free(head);
    return 1;
}


int liberarLista(tLista * pl)
{
    if(!*pl)
    {
        return 0;//lista vacia
    }
    tNodoLista * inicio = *pl;
    tNodoLista * aux = *pl;
    while(aux)
    {
        aux = (*pl)->ant;
        free((*pl)->info);
        free(*pl);
    }
    *pl=inicio;
    while(aux)
    {
        aux = (*pl)->sig;
        free((*pl)->info);
        free(*pl);
    }
    return 1;
}
