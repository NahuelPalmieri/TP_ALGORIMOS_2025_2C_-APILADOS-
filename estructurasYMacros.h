#ifndef ESTRUCTURASYMACROS_H_INCLUDED
#define ESTRUCTURASYMACROS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCHIVO_DE_TEXTO_CONFIGURACION "config.txt"

#define MAX_TAM_CADENA 30
#define MAX_TAM_SUBCADENA 30

#define ARRIBA 20
#define ABAJO 21
#define DERECHA 22
#define IZQUIERDA 23

#define FANTASMA_VIVO 40
#define FANTASMA_MUERTO 41

#define TODO_OK 1
#define ERROR_LECTURA_ARCHIVO 2

#define CADENA_DE_MOVIMIENTOS_VALIDOS "aAsSdDwW"

#define PISANDO_FRONTERAS 30
#define PISANDO_CAMINOS 31
#define PISANDO_FANTASMAS 32
#define PISANDO_VIDAS_EXTRA 33
#define PISANDO_PREMIOS 34
#define PISANDO_JUGADOR 35
#define PISANDO_SALIDA 35

#define ACCESO_CONCEDIDO 50
#define ACCESO_NO_CONCEDIDO 51

#define MOVIMIENTOS_FANTASMA_EFECTUADO 60
#define MOVIMIENTOS_FANTASMA_NO_EFECTUADO 61
#define MOVIMIENTOS_FANTASMA_VALIDO 62
#define MOVIMIENTOS_FANTASMA_NO_VALIDO 63

#define FALLO_AL_ASIGNAR_MEMORIA_DINAMICA 70

#define STRING_MOV_W "ARRIBA"
#define STRING_MOV_S "ABAJO"
#define STRING_MOV_A "IZQUIERDA"
#define STRING_MOV_D "DERECHA"

typedef struct{
    unsigned short vidas;
    unsigned short vidasExtra;
    unsigned short premios;
    unsigned int contadorPremios;
    unsigned int cantMov;
    unsigned short int fantasmas;
} tEstadisticas;

typedef struct{
    short int cantItems;
    int ** posItems;
} tUbicacionItems;

typedef struct{
    char ** mapa;
    int cantidadDeFilas;
    int cantidadDeColumnas;
    int posEntrada[3];
    int posSalida[3];
    int posJugador[2];
    tUbicacionItems posFantasmas;
    tUbicacionItems posPremios;
    tUbicacionItems posVidasExtra;
} tTablero;

#endif // ESTRUCTURASYMACROS_H_INCLUDED
