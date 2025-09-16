#include "mapa.h"

void mostrarMapa(tTablero * tablero)
{
    int i, j;

    for(i = 0; i < tablero->cantidadDeFilas; i++)
    {
        for(j = 0; j < tablero->cantidadDeColumnas; j++)
        {
            printf("%c ",*(*(tablero->mapa + i) + j));
            //printf("%c ",tablero->mapa[i][j]);
        }
        printf("\n");
    }
}

void creadorMapaDeJuegoAleatorio(tTablero * tablero, tEstadisticas * estadisticas)
{
    settearMapaEnZ(tablero);
    settearFronterasDelMapa(tablero);

    tablero->posJugador[0] = tablero->posEntrada[0];
    tablero->posJugador[1] = tablero->posEntrada[1];

    generadorDeCaminos(tablero);
    generadorDeItemsVarios(tablero,estadisticas);
}

void prepararElEntorno(char * nombreDeArchivo, tTablero * tablero, tEstadisticas * estadisticas)
{
    extraerDatosDeArchivoDeTexto(nombreDeArchivo,tablero,estadisticas);
    establecerPrimerosParametros(tablero,estadisticas);
    tablero->mapa = crearMatrizDinamicaDeCaracteres(tablero->cantidadDeFilas,tablero->cantidadDeColumnas);
}

int extraerDatosDeArchivoDeTexto(char * nombreDeArchivo, tTablero * tablero, tEstadisticas * estadisticas)
{
    FILE * arch;
    char cadena[MAX_TAM_CADENA];
    char subcadena[MAX_TAM_SUBCADENA];

    arch = fopen(nombreDeArchivo,"rt");
    if(arch == NULL)
    {
        printf("No se ha podido leer el archivo %s!!!\n",nombreDeArchivo);
        return ERROR_LECTURA_ARCHIVO;
    }


    fgets(cadena,MAX_TAM_CADENA,arch); //numero de filas del tablero
    sscanf(cadena,"%s%d",subcadena,&tablero->cantidadDeFilas);

    fgets(cadena,MAX_TAM_CADENA,arch); //numero de columnas del tablero
    sscanf(cadena,"%s%d",subcadena,&tablero->cantidadDeColumnas);

    fgets(cadena,MAX_TAM_CADENA,arch); //numero de vidas
    sscanf(cadena,"%s%hd",subcadena,&estadisticas->vidas);

    fgets(cadena,MAX_TAM_CADENA,arch); //numero de fantasmas
    sscanf(cadena,"%s%hd",subcadena,&estadisticas->fantasmas);

    fgets(cadena,MAX_TAM_CADENA,arch); //numero de premios
    sscanf(cadena,"%s%hd",subcadena,&estadisticas->premios);

    fgets(cadena,MAX_TAM_CADENA,arch); //numero de vidas extra
    sscanf(cadena,"%s%hd",subcadena,&estadisticas->vidasExtra);

    fclose(arch);

    return TODO_OK;
}

void establecerPrimerosParametros(tTablero * tablero, tEstadisticas * estadisticas)
{
    tablero->posFantasmas.posItems = crearMatrizDinamicaDeEnteros(estadisticas->fantasmas,3);
    tablero->posFantasmas.cantItems = estadisticas->fantasmas;

    tablero->posPremios.posItems = crearMatrizDinamicaDeEnteros(estadisticas->premios,2);
    tablero->posPremios.cantItems = estadisticas->premios;

    tablero->posVidasExtra.posItems = crearMatrizDinamicaDeEnteros(estadisticas->vidasExtra,2);
    tablero->posVidasExtra.cantItems = estadisticas->vidasExtra;

    estadisticas->contadorPremios = 0;
    estadisticas->cantMov = 0;
}

char ** crearMatrizDinamicaDeCaracteres(unsigned cantFilas, unsigned cantColumnas)
{
    int i;
    char ** mapa;

    mapa = malloc(cantFilas*sizeof(char *));
    if(mapa == NULL)
        return NULL;
    for(i = 0; i < cantFilas; i++)
    {
        *(mapa + i) = malloc(cantColumnas);
        if(*(mapa + i) == NULL)
            return NULL;
    }
    return mapa;
}

int ** crearMatrizDinamicaDeEnteros(unsigned cantFilas, unsigned cantColumnas)
{
    int i;
    int ** mapa;

    mapa = malloc(cantFilas*sizeof(int *));
    if(mapa == NULL)
        return NULL;
    for(i = 0; i < cantFilas; i++)
    {
        *(mapa + i) = malloc(cantColumnas*sizeof(int));
        if(*(mapa + i) == NULL)
            return NULL;
    }
    return mapa;
}

void generadorDeCaminos(tTablero * tablero)
{
    int numRandom,
        x_1, y_1,
        x_2, y_2;

    x_1 = tablero->posEntrada[0];
    y_1 = tablero->posEntrada[1];
    x_2 = tablero->posSalida[0];
    y_2 = tablero->posSalida[1];

    if(tablero->posEntrada[2] == ARRIBA || tablero->posEntrada[2] == ABAJO) //la entrada o la salida estan en la pared superior del mapa
    {
        if(tablero->posEntrada[2] == ARRIBA) //la entrada esta en la pared superior del mapa
        {
            x_1++; //muevo el valor de x de la entrada un lugar mas abajo
            x_2--; //muevo el valor de x de la salida un lugar mas arriba
        }
        else if(tablero->posEntrada[2] == ABAJO) //la salida esta en la pared superior del mapa
        {
            x_1--; //muevo el valor de x de la entrada un lugar mas arriba
            x_2++; //muevo el valor de x de la entrada un lugar mas abajo
        }

        tablero->mapa[x_1][y_1] = '.'; //setteo la primer posicion del mapa con un camino ('.')
    } else if(tablero->posEntrada[2] == IZQUIERDA || tablero->posEntrada[2] == DERECHA) //la entrada o la salida estan en la pared izquieda del mapa
    {
        if(tablero->posEntrada[2] == IZQUIERDA) //la entrada esta en la pared izquierda del mapa
        {
            y_1++; //muevo el valor de y de la entrada un lugar mas a la derecha
            y_2--; //muevo el valor de y de la salida un lugar mas a la izquieda
        }
        else if(tablero->posEntrada[2] == DERECHA) //la salida esta en la pared derecha del mapa
        {
            y_1--; //muevo el valor de y de la entrada un lugar mas a la izquierda
            y_2++; //muevo el valor de y de la entrada un lugar mas a la derecha
        }

        tablero->mapa[x_1][y_1] = '.'; //setteo la primer posicion del mapa con un camino ('.')
    }

    while(tablero->mapa[x_2][y_2] != '.') //genero un primer camino
    {
        numRandom = rand() % 2;

        if(numRandom == 0)
        {
            numRandom = rand() % 4; //4 valores posibles: 0 -> me muevo hacia arriba
                                    //                    1 -> me muevo hacia abajo
                                    //                    2 -> me muevo hacia la izquierda
                                    //                    3 -> me muevo hacia la derecha
            switch(numRandom)
            {
                case 0: //0 -> me muevo hacia arriba
                    if(x_1 > 1)
                        x_1--;
                    break;
                case 1: //1 -> me muevo hacia abajo
                    if(x_1 < tablero->cantidadDeFilas - 2)
                        x_1++;
                    break;
                case 2: //2 -> me muevo hacia la izquierda
                    if(y_1 > 1)
                        y_1--;
                    break;
                case 3: //3 -> me muevo hacia la derecha
                    if(y_1 < tablero->cantidadDeColumnas - 2)
                        y_1++;
                    break;
            }
        } else
        {
            numRandom = rand() % 4; //4 valores posibles: 0 -> me muevo hacia arriba
                                    //                    1 -> me muevo hacia abajo
                                    //                    2 -> me muevo hacia la izquierda
                                    //                    3 -> me muevo hacia la derecha
            switch(numRandom)
            {
                case 0: //0 -> me muevo hacia arriba
                    if(x_1 > 1 && x_1 > x_2)
                        x_1--;
                    break;
                case 1: //1 -> me muevo hacia abajo
                    if(x_1 < tablero->cantidadDeFilas - 2 && x_1 < x_2)
                        x_1++;
                    break;
                case 2: //2 -> me muevo hacia la izquierda
                    if(y_1 > 1 && y_1 > y_2)
                        y_1--;
                    break;
                case 3: //3 -> me muevo hacia la derecha
                    if(y_1 < tablero->cantidadDeColumnas - 2 && y_1 < y_2)
                        y_1++;
                    break;
            }
        }
        tablero->mapa[x_1][y_1] = '.'; //setteo la posicion random del mapa con un camino ('.') si es posible
    }
}

void generadorDeItemsVarios(tTablero * tablero, tEstadisticas * estadisticas)
{
    int numRandom, i, j;

    insertarNItemsDeXTipo(tablero,estadisticas->fantasmas,'F',tablero->posFantasmas.posItems);
    for(i = 0; i < estadisticas->fantasmas; i++)
        tablero->posFantasmas.posItems[i][2] = FANTASMA_VIVO;
    insertarNItemsDeXTipo(tablero,estadisticas->vidasExtra,'V',tablero->posVidasExtra.posItems);
    insertarNItemsDeXTipo(tablero,estadisticas->premios,'P',tablero->posPremios.posItems);

    for(i = 1; i < tablero->cantidadDeFilas - 1; i++)
    {
        for(j = 1; j < tablero->cantidadDeColumnas - 1; j++)
        {
            if(tablero->mapa[i][j] == 'Z')
            {
                numRandom = rand() % 3;

                switch(numRandom)
                {
                    case 0:
                    case 1:
                        tablero->mapa[i][j] = '#';
                        break;
                    case 2:
                        tablero->mapa[i][j] = '.';
                        break;
                }
            }
        }
    }
}

void insertarNItemsDeXTipo(tTablero * tablero, int maxCantItem, char caracterAInsertar, int ** posicionesAInsertar)
{
    int xRandom,
        yRandom;


    while(maxCantItem > 0)
    {
        xRandom = rand() % (tablero->cantidadDeFilas - 2); //xRandom tiene cantidadDeFilas - 2 posibles numeros
        yRandom = rand() % (tablero->cantidadDeColumnas - 2); //yRandom tiene cantidadDeColumnas - 2 posibles numeros

        xRandom++; //le sumo 1 a xRandom para que las opciones no incluyan las esquinas del mapa
        yRandom++; //le sumo 1 a yRandom para que las opciones no incluyan las esquinas del mapa

        if(tablero->mapa[xRandom][yRandom] == 'Z' &&
           (tablero->mapa[xRandom + 1][yRandom] == '.' ||
           tablero->mapa[xRandom - 1][yRandom] == '.' ||
           tablero->mapa[xRandom][yRandom + 1] == '.' ||
           tablero->mapa[xRandom][yRandom - 1] == '.'))
        {
            tablero->mapa[xRandom][yRandom] = caracterAInsertar;

            posicionesAInsertar[maxCantItem - 1][0] = xRandom;
            posicionesAInsertar[maxCantItem - 1][1] = yRandom;

            maxCantItem--;
        }
    }
}

void settearMapaEnZ(tTablero * tablero)
{
    int i, j;

    for(i = 0; i < tablero->cantidadDeFilas; i++)
    {
        for(j = 0; j < tablero->cantidadDeColumnas; j++)
        {
            *(*(tablero->mapa + i) + j) = 'Z';
            //tablero->mapa[i][j] = 'Z';
        }
    }
}

void settearFronterasDelMapa(tTablero * tablero)
{
    int i, numRandom, posRandom1, posRandom2;

    for(i = 0; i < tablero->cantidadDeColumnas; i++) //setteo los bordes superior e inferior
    {
        *(*(tablero->mapa) + i) = '#';
        //mapa[0][i] = '#';
        *(*(tablero->mapa + tablero->cantidadDeFilas - 1) + i) = '#';
        //mapa[tablero->cantidadDeFilas - 1][i] = '#';
    }

    for(i = 0; i < tablero->cantidadDeFilas; i++) //setteo los bordes derecho e izquierdo
    {
        *(*(tablero->mapa + i)) = '#';
        //mapa[i][0] = '#';
        *(*(tablero->mapa + i) + tablero->cantidadDeColumnas - 1) = '#';
        //mapa[i][tablero->cantidadDeColumnas - 1] = '#';
    }

    numRandom = rand() % 4; //devuelve un numero entre 0 y 3. determina si la entrada y la salida estan en derecha e izquierda o arriba y abajo

    switch(numRandom)
    {
        case 0: //entrada en izquieda y salida en derecha
            posRandom1 = rand() % (tablero->cantidadDeFilas - 2); //posRandom1 tiene cantidadDeFilas - 2 posibles numeros
            posRandom1++; //le sumo 1 a posRandom1 para que las opciones no incluyan las esquinas del mapa
            posRandom2 = rand() % (tablero->cantidadDeFilas - 2); //posRandom2 tiene cantidadDeFilas - 2 posibles numeros
            posRandom2++; //le sumo 1 a posRandom2 para que las opciones no incluyan las esquinas del mapa


            tablero->mapa[posRandom1][0] = 'E'; //entrada: en mapa[posRandom1][0]
            tablero->posEntrada[0] = posRandom1; //guardo la posicion de la fila de donde se encuentra la entrada
            tablero->posEntrada[1] = 0; //guardo la posicion de la columna de donde se encuentra la entrada
            tablero->posEntrada[2] = IZQUIERDA; //guardo en posEntrada[2] que la entrada se encuentra en la parte izquierda del mapa

            tablero->mapa[posRandom2][tablero->cantidadDeColumnas - 1] = 'S'; //salida: en mapa[posRandom2][cantColumnas - 1]
            tablero->posSalida[0] = posRandom2; //guardo la posicion de la fila de donde se encuentra la salida
            tablero->posSalida[1] = tablero->cantidadDeColumnas - 1; //guardo la posicion de la columna de donde se encuentra la salida
            tablero->posSalida[2] = DERECHA; //guardo en posSalida[2] que la salida se encuentra en la parte derecha del mapa

            break;
        case 1: //entrada en derecha y salida en izquierda
            posRandom1 = rand() % (tablero->cantidadDeFilas - 2); //posRandom1 tiene cantidadDeFilas - 2 posibles numeros
            posRandom1++; //le sumo 1 a posRandom1 para que las opciones no incluyan las esquinas del mapa
            posRandom2 = rand() % (tablero->cantidadDeFilas - 2); //posRandom2 tiene cantidadDeFilas - 2 posibles numeros
            posRandom2++; //le sumo 1 a posRandom2 para que las opciones no incluyan las esquinas del mapa


            tablero->mapa[posRandom1][0] = 'S'; //salida: en mapa[posRandom1][0]
            tablero->posSalida[0] = posRandom1; //guardo la posicion de la fila de donde se encuentra la salida
            tablero->posSalida[1] = 0; //guardo la posicion de la columna de donde se encuentra la salida
            tablero->posSalida[2] = IZQUIERDA; //guardo en posSalida[2] que la salida se encuentra en la parte izquierda del mapa

            tablero->mapa[posRandom2][tablero->cantidadDeColumnas - 1] = 'E'; //entrada: en mapa[posRandom2][cantColumnas - 1]
            tablero->posEntrada[0] = posRandom2; //guardo la posicion de la fila de donde se encuentra la entrada
            tablero->posEntrada[1] = tablero->cantidadDeColumnas - 1; //guardo la posicion de la columna de donde se encuentra la entrada
            tablero->posEntrada[2] = DERECHA; //guardo en posEntrada[2] que la entrada se encuentra en la parte derecha del mapa

            break;
        case 2: //en este caso la salida vamos a trabajar arriba y abajo
            posRandom1 = rand() % (tablero->cantidadDeColumnas - 2); //posRandom1 tiene cantidadDeColumnas - 2 posibles numeros
            posRandom1++; //le sumo 1 a posRandom1 para que las opciones no incluyan las esquinas del mapa
            posRandom2 = rand() % (tablero->cantidadDeColumnas - 2); //posRandom2 tiene cantidadDeColumnas - 2 posibles numeros
            posRandom2++; //le sumo 1 a posRandom2 para que las opciones no incluyan las esquinas del mapa


            tablero->mapa[0][posRandom1] = 'E'; //entrada: en mapa[0][posRandom1]
            tablero->posEntrada[0] = 0; //guardo la posicion de la fila de donde se encuentra la entrada
            tablero->posEntrada[1] = posRandom1; //guardo la posicion de la columna de donde se encuentra la entrada
            tablero->posEntrada[2] = ARRIBA; //guardo en posEntrada[2] que la entrada se encuentra en la parte superior del mapa

            tablero->mapa[tablero->cantidadDeFilas - 1][posRandom2] = 'S'; //salida: en mapa[cantFilas - 1][posRandom2]
            tablero->posSalida[0] = tablero->cantidadDeFilas - 1; //guardo la posicion de la fila de donde se encuentra la salida
            tablero->posSalida[1] = posRandom2; //guardo la posicion de la columna de donde se encuentra la salida
            tablero->posSalida[2] = ABAJO; //guardo en posSalida[2] que la salida se encuentra en la parte inferior del mapa

            break;
        case 3: //entrada abajo y salida arriba
            posRandom1 = rand() % (tablero->cantidadDeColumnas - 2); //posRandom1 tiene cantidadDeColumnas - 2 posibles numeros
            posRandom1++; //le sumo 1 a posRandom1 para que las opciones no incluyan las esquinas del mapa
            posRandom2 = rand() % (tablero->cantidadDeColumnas - 2); //posRandom2 tiene cantidadDeColumnas - 2 posibles numeros
            posRandom2++; //le sumo 1 a posRandom2 para que las opciones no incluyan las esquinas del mapa


            tablero->mapa[0][posRandom1] = 'S'; //salida: en mapa[0][posRandom1]
            tablero->posSalida[0] = 0; //guardo la posicion de la fila de donde se encuentra la salida
            tablero->posSalida[1] = posRandom1; //guardo la posicion de la columna de donde se encuentra la salida
            tablero->posSalida[2] = ARRIBA; //guardo en posSalida[2] que la salida se encuentra en la parte superior del mapa

            tablero->mapa[tablero->cantidadDeFilas - 1][posRandom2] = 'E'; //entrada: en mapa[cantFilas - 1][posRandom2]
            tablero->posEntrada[0] = tablero->cantidadDeFilas - 1; //guardo la posicion de la fila de donde se encuentra la entrada
            tablero->posEntrada[1] = posRandom2; //guardo la posicion de la columna de donde se encuentra la entrada
            tablero->posEntrada[2] = ABAJO; //guardo en posEntrada[2] que la entrada se encuentra en la parte inferior del mapa

            break;
    }
}

void destruirMatrizEntera(int ** matriz, unsigned cantFilas)
{
    int i;

    for(i = 0; i < cantFilas; i++)
        free(*(matriz+i));

    free(matriz);
}

void destruirMatrizDeCaracteres(char ** matriz, unsigned cantFilas)
{
    int i;

    for(i = 0; i < cantFilas; i++)
        free(*(matriz+i));

    free(matriz);
}
