#include "Pacman.h"
///===============COLA DINAMICA===============
void crear_cola(tCola* pc) {

    pc->pri = NULL;
    pc->ult = NULL;

}

void vaciar_cola(tCola* pc) {

    tNodo* aux;

    while(pc->pri) {
        aux = pc->pri;
        pc->pri = aux->sig;
        free(aux->info);
        free(aux);
    }

}

int encolar(tCola* pc, const void* elem, unsigned tamElem) {

    tNodo* nue;

    if((nue = (tNodo*)malloc(sizeof(tNodo))) == NULL || (nue->info = malloc(tamElem)) == NULL) {
        free(nue);
        return 0;
    }

    memcpy(nue->info, elem, tamElem);
    nue->tamInfo = tamElem;
    nue->sig = NULL;

    if(pc->ult) {
        pc->ult->sig = nue;
    } else {
        pc->pri = nue;
    }
    pc->ult = nue;
    return 1;
}

int ver_primero_cola(const tCola* pc, void* elem, unsigned tamElem) {

    if(pc->pri == NULL) {
        return 0;
    }

    memcpy(elem, pc->pri, MINIMO(tamElem, pc->pri->tamInfo));
    return 1;
}

int desencolar(tCola* pc, void* elem, unsigned tamElem) {

    tNodo* aux = pc->pri;

    if(aux == NULL) {
        return 0;
    }

    pc->pri = aux->sig;
    memcpy(elem, aux->info, MINIMO(tamElem, aux->tamInfo));
    free(aux->info);
    free(aux);
    if(pc->pri == NULL) {
        pc->ult = NULL;
    }
    return 1;
}

int cola_vacia(const tCola* pc) {

    return pc->pri == NULL;

}

int cola_llena(const tCola* pc, unsigned tamElem) {

    tNodo* aux = (tNodo*)malloc(sizeof(tNodo));
    void* info = malloc(tamElem);

    free(aux);
    free(info);

    return aux == NULL || info == NULL;
}































