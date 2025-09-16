#include "movimientos.h"
#include "mapa.h"

void moversePorElMapa(tTablero * tablero, tEstadisticas * estadisticas, tCola * colaMovJugador, tCola * colaMovMaquina)
{
    char movimientoJugador;

    while((tablero->posJugador[0] != tablero->posSalida[0] ||
          tablero->posJugador[1] != tablero->posSalida[1]) &&
          estadisticas->vidas > 0)
    {
        movimientoJugador = moverAlJugador(tablero,estadisticas);
        estadisticas->cantMov++;
        encolarMovimientos(colaMovJugador,movimientoJugador);
        moverALosFantasmas(tablero,estadisticas,colaMovMaquina);
        mostrarMapa(tablero);
        mostrarEstadisticas(estadisticas);
    }

    if(tablero->posJugador[0] == tablero->posSalida[0] &&
       tablero->posJugador[1] == tablero->posSalida[1])
    {
        printf("\nFELICIDADES!!! GANASTE EL JUEGO!!!\n");
    } else if(estadisticas->vidas == 0)
    {
        printf("\nLO SIENTO!!! PERDISTE TODAS LAS VIDAS!!!\n");
    }
}

char moverAlJugador(tTablero * tablero, tEstadisticas * estadisticas)
{
    char movimientoElegido;
    int verificadorProximaCelda, verificadorAcceso;
    int movEnX, movEnY, i;

    movimientoElegido = obtenerSaloicitudDeMovimiento(CADENA_DE_MOVIMIENTOS_VALIDOS);

    switch(movimientoElegido)
    {
        case 'w':
        case 'W': //me muevo hacia arriba
            if((tablero->posJugador[0] - 1) >= 0)
            {
                movEnX = -1;
                movEnY = 0;
                verificadorProximaCelda = verificarMovimientosValidos(tablero,movEnX,movEnY);

                if(verificadorProximaCelda != PISANDO_FRONTERAS)
                    verificadorAcceso = ACCESO_CONCEDIDO;
                else
                    verificadorAcceso = ACCESO_NO_CONCEDIDO;
            }
            break;
        case 's':
        case 'S': //me muevo hacia abajo
            if((tablero->posJugador[0] + 1) <= tablero->cantidadDeFilas - 1)
            {
                movEnX = 1;
                movEnY = 0;
                verificadorProximaCelda = verificarMovimientosValidos(tablero,movEnX,movEnY);

                if(verificadorProximaCelda != PISANDO_FRONTERAS)
                    verificadorAcceso = ACCESO_CONCEDIDO;
                else
                    verificadorAcceso = ACCESO_NO_CONCEDIDO;
            }
            break;
        case 'a':
        case 'A': //me muevo hacia la izquierda
            if((tablero->posJugador[1] - 1) >= 0)
            {
                movEnX = 0;
                movEnY = -1;
                verificadorProximaCelda = verificarMovimientosValidos(tablero,movEnX,movEnY);

                if(verificadorProximaCelda != PISANDO_FRONTERAS)
                    verificadorAcceso = ACCESO_CONCEDIDO;
                else
                    verificadorAcceso = ACCESO_NO_CONCEDIDO;
            }
            break;
        case 'd':
        case 'D': //me muevo hacia la derecha
            if((tablero->posJugador[1] + 1) <= tablero->cantidadDeFilas - 1)
            {
                movEnX = 0;
                movEnY = 1;
                verificadorProximaCelda = verificarMovimientosValidos(tablero,movEnX,movEnY);

                if(verificadorProximaCelda != PISANDO_FRONTERAS)
                    verificadorAcceso = ACCESO_CONCEDIDO;
                else
                    verificadorAcceso = ACCESO_NO_CONCEDIDO;
            }
            break;
    }

    if(verificadorAcceso == ACCESO_CONCEDIDO)
    {
        switch(verificadorProximaCelda)
        {
            case PISANDO_CAMINOS:
                if(tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] == 'E')
                    tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] = 'E';
                else
                    tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] = '.';
                tablero->mapa[tablero->posJugador[0] + movEnX][tablero->posJugador[1] + movEnY] = 'J';

                tablero->posJugador[0] += movEnX;
                tablero->posJugador[1] += movEnY;
                break;
            case PISANDO_PREMIOS:
                if(tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] == 'E')
                    tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] = 'E';
                else
                    tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] = '.';
                tablero->mapa[tablero->posJugador[0] + movEnX][tablero->posJugador[1] + movEnY] = 'J';

                tablero->posJugador[0] += movEnX;
                tablero->posJugador[1] += movEnY;
                estadisticas->premios--;
                estadisticas->contadorPremios++;
                break;
            case PISANDO_VIDAS_EXTRA:
                if(tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] == 'E')
                    tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] = 'E';
                else
                    tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] = '.';
                tablero->mapa[tablero->posJugador[0] + movEnX][tablero->posJugador[1] + movEnY] = 'J';

                tablero->posJugador[0] += movEnX;
                tablero->posJugador[1] += movEnY;
                estadisticas->vidasExtra--;
                estadisticas->vidas++;
                break;
            case PISANDO_FANTASMAS:
                if(tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] == 'E')
                    tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] = 'E';
                else
                    tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] = '.';
                tablero->mapa[tablero->posJugador[0] + movEnX][tablero->posJugador[1] + movEnY] = '.';

                for(i = 0; i < tablero->posFantasmas.cantItems; i++) //for(i = 0; i < estadisticas->fantasmas; i++)
                {
                    if((tablero->posJugador[0] + movEnX) == tablero->posFantasmas.posItems[i][0] &&
                        (tablero->posJugador[1] + movEnY) == tablero->posFantasmas.posItems[i][1])
                    {
                        tablero->posFantasmas.posItems[i][2] = FANTASMA_MUERTO;
                        estadisticas->fantasmas--;
                    }
                }
                tablero->posJugador[0] = tablero->posEntrada[0];
                tablero->posJugador[1] = tablero->posEntrada[1];
                estadisticas->vidas--;
                break;
            case PISANDO_SALIDA:
                tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] = '.';
                tablero->posJugador[0] += movEnX;
                tablero->posJugador[1] += movEnY;
                break;
        }

        return movimientoElegido;
    }
}

void moverALosFantasmas(tTablero * tablero, tEstadisticas * estadisticas, tCola * colaMovMaquina)
{
    int movEfectuado, juegoRandom, i, movEnX, movEnY, numRandom;

    for(i = 0; i < tablero->posFantasmas.cantItems; i++)
    {

        if(tablero->posFantasmas.posItems[i][2] == FANTASMA_VIVO)
        {
            movEfectuado = MOVIMIENTOS_FANTASMA_NO_EFECTUADO;

            while(movEfectuado == MOVIMIENTOS_FANTASMA_NO_EFECTUADO)
            {
                juegoRandom = rand() % 10;

                switch(juegoRandom)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                        {
                            numRandom = rand() % 4;

                            switch(numRandom)
                            {
                                case 0: //el fantasma se mueve hacia arriba sobre la misma columna
                                    if(tablero->posJugador[0] < tablero->posFantasmas.posItems[i][0])
                                    {
                                        movEnX = -1;
                                        movEnY = 0;
                                        movEfectuado = moverFantasmasSiSePuede(tablero,estadisticas,i,movEnX,movEnY);
                                        if(movEfectuado == MOVIMIENTOS_FANTASMA_EFECTUADO)
                                            encolarMovimientosDeFantasmas(colaMovMaquina,i,ARRIBA);
                                    }
                                    break;
                                case 1: //el fantasma se mueve hacia abajo sobre la misma columna
                                    if(tablero->posJugador[0] > tablero->posFantasmas.posItems[i][0])
                                    {
                                        movEnX = 1;
                                        movEnY = 0;
                                        movEfectuado = moverFantasmasSiSePuede(tablero,estadisticas,i,movEnX,movEnY);
                                        if(movEfectuado == MOVIMIENTOS_FANTASMA_EFECTUADO)
                                            encolarMovimientosDeFantasmas(colaMovMaquina,i,ABAJO);
                                    }
                                    break;
                                case 2: //el fantasma se mueve hacia la derecha sobre la misma fila
                                    if(tablero->posJugador[1] > tablero->posFantasmas.posItems[i][1])
                                    {
                                        movEnX = 0;
                                        movEnY = 1;
                                        movEfectuado = moverFantasmasSiSePuede(tablero,estadisticas,i,movEnX,movEnY);
                                        if(movEfectuado == MOVIMIENTOS_FANTASMA_EFECTUADO)
                                            encolarMovimientosDeFantasmas(colaMovMaquina,i,DERECHA);
                                    }
                                    break;
                                case 3: //el fantasma se mueve hacia la izquierda sobre la misma fila
                                    if(tablero->posJugador[1] < tablero->posFantasmas.posItems[i][1])
                                    {
                                        movEnX = 0;
                                        movEnY = -1;
                                        movEfectuado = moverFantasmasSiSePuede(tablero,estadisticas,i,movEnX,movEnY);
                                        if(movEfectuado == MOVIMIENTOS_FANTASMA_EFECTUADO)
                                            encolarMovimientosDeFantasmas(colaMovMaquina,i,IZQUIERDA);
                                    }
                                    break;
                            }
                            break;
                        }
                    case 9:
                        {
                            numRandom = rand() % 4;

                            switch(numRandom)
                            {
                                case 0: //el fantasma se mueve hacia arriba sobre la misma columna
                                    movEnX = -1;
                                    movEnY = 0;
                                    movEfectuado = moverFantasmasSiSePuede(tablero,estadisticas,i,movEnX,movEnY);
                                    if(movEfectuado == MOVIMIENTOS_FANTASMA_EFECTUADO)
                                            encolarMovimientosDeFantasmas(colaMovMaquina,i,ARRIBA);
                                    break;
                                case 1: //el fantasma se mueve hacia abajo sobre la misma columna
                                    movEnX = 1;
                                    movEnY = 0;
                                    movEfectuado = moverFantasmasSiSePuede(tablero,estadisticas,i,movEnX,movEnY);
                                    if(movEfectuado == MOVIMIENTOS_FANTASMA_EFECTUADO)
                                            encolarMovimientosDeFantasmas(colaMovMaquina,i,ABAJO);
                                    break;
                                case 2: //el fantasma se mueve hacia la derecha sobre la misma fila
                                    movEnX = 0;
                                    movEnY = 1;
                                    movEfectuado = moverFantasmasSiSePuede(tablero,estadisticas,i,movEnX,movEnY);
                                    if(movEfectuado == MOVIMIENTOS_FANTASMA_EFECTUADO)
                                            encolarMovimientosDeFantasmas(colaMovMaquina,i,DERECHA);
                                    break;
                                case 3: //el fantasma se mueve hacia la izquierda sobre la misma fila
                                    movEnX = 0;
                                    movEnY = -1;
                                    movEfectuado = moverFantasmasSiSePuede(tablero,estadisticas,i,movEnX,movEnY);
                                    if(movEfectuado == MOVIMIENTOS_FANTASMA_EFECTUADO)
                                            encolarMovimientosDeFantasmas(colaMovMaquina,i,IZQUIERDA);
                                    break;
                            }
                            break;
                        }
                }
            }
        }
    }
}

int moverFantasmasSiSePuede(tTablero * tablero, tEstadisticas * estadisticas, int i, int movEnX, int movEnY)
{
    int verificadorProximaCelda;

    verificadorProximaCelda = verificarMovimientosValidosFantasmas(tablero,i,movEnX,movEnY);
    if(verificadorProximaCelda == PISANDO_CAMINOS)
    {
        tablero->mapa[tablero->posFantasmas.posItems[i][0]][tablero->posFantasmas.posItems[i][1]] = '.';
        tablero->posFantasmas.posItems[i][0] += movEnX;
        tablero->posFantasmas.posItems[i][1] += movEnY;
        tablero->mapa[tablero->posFantasmas.posItems[i][0]][tablero->posFantasmas.posItems[i][1]] = 'F';


        return MOVIMIENTOS_FANTASMA_EFECTUADO;
    } else if(verificadorProximaCelda == PISANDO_JUGADOR)
    {
        tablero->mapa[tablero->posFantasmas.posItems[i][0]][tablero->posFantasmas.posItems[i][1]] = '.';
        tablero->posFantasmas.posItems[i][0] += movEnX;
        tablero->posFantasmas.posItems[i][1] += movEnY;
        if(tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] == 'E')
            tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] = 'E';
        else
            tablero->mapa[tablero->posJugador[0]][tablero->posJugador[1]] = '.';
        tablero->posFantasmas.posItems[i][2] = FANTASMA_MUERTO;
        estadisticas->vidas--;
        estadisticas->fantasmas--;
        tablero->posJugador[0] = tablero->posEntrada[0];
        tablero->posJugador[1] = tablero->posEntrada[1];
        return MOVIMIENTOS_FANTASMA_EFECTUADO;
    } else
        return MOVIMIENTOS_FANTASMA_NO_EFECTUADO;
}

char obtenerSaloicitudDeMovimiento(const char * caracteresValidos)
{
    char opcionElegida;

    do
    {
        scanf("%c",&opcionElegida);
    } while(strchr(caracteresValidos,opcionElegida)==NULL);

    return opcionElegida;
}

int verificarMovimientosValidos(tTablero * tablero, int movEnX, int movEnY)
{
    if(tablero->mapa[tablero->posJugador[0] + movEnX][tablero->posJugador[1] + movEnY] == '#' ||
       tablero->mapa[tablero->posJugador[0] + movEnX][tablero->posJugador[1] + movEnY] == 'E')
        return PISANDO_FRONTERAS;
    else if(tablero->mapa[tablero->posJugador[0] + movEnX][tablero->posJugador[1] + movEnY] == '.')
        return PISANDO_CAMINOS;
    else if(tablero->mapa[tablero->posJugador[0] + movEnX][tablero->posJugador[1] + movEnY] == 'F')
        return PISANDO_FANTASMAS;
    else if(tablero->mapa[tablero->posJugador[0] + movEnX][tablero->posJugador[1] + movEnY] == 'V')
        return PISANDO_VIDAS_EXTRA;
    else if(tablero->mapa[tablero->posJugador[0] + movEnX][tablero->posJugador[1] + movEnY] == 'P')
        return PISANDO_PREMIOS;
    else if(tablero->mapa[tablero->posJugador[0] + movEnX][tablero->posJugador[1] + movEnY] == 'S')
        return PISANDO_SALIDA;
}

int verificarMovimientosValidosFantasmas(tTablero * tablero, int i, int movEnX, int movEnY)
{
    if(tablero->mapa[tablero->posFantasmas.posItems[i][0] + movEnX][tablero->posFantasmas.posItems[i][1] + movEnY] == '#' ||
       tablero->mapa[tablero->posFantasmas.posItems[i][0] + movEnX][tablero->posFantasmas.posItems[i][1] + movEnY] == 'E')
        return PISANDO_FRONTERAS;
    else if(tablero->mapa[tablero->posFantasmas.posItems[i][0] + movEnX][tablero->posFantasmas.posItems[i][1] + movEnY] == '.')
        return PISANDO_CAMINOS;
    else if(tablero->mapa[tablero->posFantasmas.posItems[i][0] + movEnX][tablero->posFantasmas.posItems[i][1] + movEnY] == 'F')
        return PISANDO_FANTASMAS;
    else if(tablero->mapa[tablero->posFantasmas.posItems[i][0] + movEnX][tablero->posFantasmas.posItems[i][1] + movEnY] == 'V')
        return PISANDO_VIDAS_EXTRA;
    else if(tablero->mapa[tablero->posFantasmas.posItems[i][0] + movEnX][tablero->posFantasmas.posItems[i][1] + movEnY] == 'P')
        return PISANDO_PREMIOS;
    else if(tablero->mapa[tablero->posFantasmas.posItems[i][0] + movEnX][tablero->posFantasmas.posItems[i][1] + movEnY] == 'J')
        return PISANDO_JUGADOR;
    else if(tablero->mapa[tablero->posFantasmas.posItems[i][0] + movEnX][tablero->posFantasmas.posItems[i][1] + movEnY] == 'S')
        return PISANDO_SALIDA;
}

void mostrarEstadisticas(tEstadisticas * estadisticas)
{
    printf("\nVidas: %d - Puntos: %d\n",estadisticas->vidas,estadisticas->contadorPremios);
}

void encolarMovimientos(tCola * cola, char movimiento)
{
    movimiento = toupper(movimiento);
    int cantBytes;

    switch(movimiento)
    {
        case 'W':
            cantBytes = strlen(STRING_MOV_W) + 1;
            ponerEnCola(cola,STRING_MOV_W,cantBytes);
            break;
        case 'S':
            cantBytes = strlen(STRING_MOV_S) + 1;
            ponerEnCola(cola,STRING_MOV_S,cantBytes);
            break;
        case 'A':
            cantBytes = strlen(STRING_MOV_A) + 1;
            ponerEnCola(cola,STRING_MOV_A,cantBytes);
            break;
        case 'D':
            cantBytes = strlen(STRING_MOV_D) + 1;
            ponerEnCola(cola,STRING_MOV_D,cantBytes);
            break;
    }
}

void encolarMovimientosDeFantasmas(tCola * cola, int i, int movimiento)
{
    int vecAuxiliar[2];

    vecAuxiliar[0] = i; //asigno el numero de fantasma
    vecAuxiliar[1] = movimiento; //asigno la direccion en la que se movio el fantasma

    ponerEnCola(cola,vecAuxiliar,sizeof(vecAuxiliar));
}

void mostrarMovimientosEncoladosDeJugador(tCola * cola)
{
    char cadAux[10];
    printf("\nMovimientos De La Partida:\n");
    while(!colaVacia(cola))
    {
        sacarDeCola(cola,cadAux,sizeof(cadAux));
        printf("%s\n",cadAux);
    }
}

void mostrarMovimientosEncoladosDeFantasmas(tCola * cola)
{
    char cadAux[13];
    char movAux[10];
    char numFantasma;
    int vecAux[2];

    printf("\nMovimientos De Los Fantasmas De La Partida:\n");

    while(!colaVacia(cola))
    {
        sacarDeCola(cola,vecAux,sizeof(vecAux));

        strcpy(cadAux,"Fantasma ");
        sprintf(&numFantasma,"%d",vecAux[0]);
        strcat(cadAux,&numFantasma);
        strcat(cadAux,": ");

        switch(vecAux[1])
        {
            case ARRIBA:
                strcpy(movAux,STRING_MOV_W);
                break;
            case ABAJO:
                strcpy(movAux,STRING_MOV_S);
                break;
            case DERECHA:
                strcpy(movAux,STRING_MOV_D);
                break;
            case IZQUIERDA:
                strcpy(movAux,STRING_MOV_A);
                break;
        }

        printf("%s%s\n",cadAux,movAux);
    }
}
