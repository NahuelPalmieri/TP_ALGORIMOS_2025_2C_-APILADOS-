#include "Pacman.h"

int init_winsock() {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET connect_to_server(const char *server_ip, int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) return INVALID_SOCKET;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        closesocket(sock);
        return INVALID_SOCKET;
    }

    return sock;
}

int send_request(SOCKET sock, const char *request, char *response) {
    if (send(sock, request, strlen(request), 0) < 0) {
        return -1;
    }

    int bytes_received = recv(sock, response, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0) {
        return -1;
    }

    response[bytes_received] = '\0';
    return 0;
}

void close_connection(SOCKET sock) {
    closesocket(sock);
    WSACleanup();
}
///

/**
 * @brief Muestra el menú principal del juego Pacman.
 *
 * Imprime las opciones disponibles en pantalla:
 * 1. Jugar partida
 * 2. Ver ranking
 * 3. Salir
 */

void mostrarMenu() {

    printf("====== PACMAN ASCII ======\n");
    printf("1 - Jugar partida\n");
    printf("2 - Ver ranking\n");
    printf("3 - Salir\n");

}

/**
 * @brief Inicia una nueva partida del juego Pacman.
 *
 * Carga la configuración desde archivo, inicializa el tablero, jugador y fantasmas,
 * crea el entorno del juego y luego entra en el bucle principal mediante `juego()`.
 * Finalmente libera la memoria utilizada.
 *
 * @param archivoConfig Ruta del archivo de configuración (.txt).
 */

tUsuario jugarPartida(char* archivoConfig) {

    Configuracion configTablero;
    Tablero juegoTablero;
    Jugador jugadorActual;
    Posicion jugadorPos;
    Posicion* fantasmas;
    tUsuario usuario;

    srand(time(NULL));
    leerConfiguracion(archivoConfig, &configTablero, sizeof(Tablero));
    inicializarJugador(configTablero, &jugadorActual);
    asignarMemoria(&configTablero, &juegoTablero, &fantasmas);
    crearJuego(&configTablero, &juegoTablero, &jugadorPos, &fantasmas);
    imprimirTablero(&juegoTablero, &jugadorActual);
    juego(&configTablero, &juegoTablero, &jugadorActual, &jugadorPos, &fantasmas);
    destruirJuego(&juegoTablero);
    free(fantasmas);

    usuario.cantidadMovimientos = *(jugadorActual.cantidadMovimientos);
    free(jugadorActual.cantidadMovimientos);

    strcpy(usuario.nombre,jugadorActual.nombre);
    usuario.puntuacion = jugadorActual.puntuacion;

    return usuario;
}

/**
 * @brief Inicializa la estructura del jugador al inicio de la partida.
 *
 * - Solicita el nombre del jugador por teclado.
 * - Valida longitud máxima y evita cadenas vacías.
 * - Asigna vidas iniciales y valores por defecto.
 *
 * @param configTablero Estructura de configuración con las vidas iniciales.
 * @param jugadorActual Puntero a la estructura del jugador a inicializar.
 */

void inicializarJugador(Configuracion configTablero, Jugador* jugadorActual) {

    int valido;

    jugadorActual->puntuacion = 0;
    jugadorActual->partidas = 0;
    jugadorActual->vidas = configTablero.vidas;
    jugadorActual->cantidadMovimientos = NULL;

    do {
        valido = 1;
        printf("Ingrese su nombre de jugador (max %d caracteres): ", MAX_NOMBRE - 1);

        if (!fgets(jugadorActual->nombre, MAX_NOMBRE, stdin)) {
            jugadorActual->nombre[0] = '\0';
            valido = 0;
            continue;
        }

        if (strchr(jugadorActual->nombre, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            printf("El nombre supera el maximo de %d caracteres. Intente nuevamente.\n", MAX_NOMBRE - 1);
            jugadorActual->nombre[0] = '\0';
            valido = 0;
            continue;
        }

        jugadorActual->nombre[strcspn(jugadorActual->nombre, "\n")] = '\0';

        if (strlen(jugadorActual->nombre) == 0) {
            printf("El nombre no puede estar vacio.\n");
            valido = 0;
        }

    } while (!valido);

}

/**
 * @brief Lee la configuración del juego desde un archivo de texto.
 *
 * Interpreta líneas del archivo separadas por “:”, y asigna valores
 * a los campos de la estructura `Configuracion`.
 *
 * @param file_name Nombre del archivo de configuración.
 * @param configTablero Puntero a la estructura donde se guardará la configuración.
 * @param tamElem Tamaño del elemento (sin uso directo, por compatibilidad).
 */

void leerConfiguracion(const char* file_name, Configuracion* configTablero, size_t tamElem) {

    int i = 0;
    char buffer[MAX_BUFFER];

    FILE* pArchivo = fopen(file_name, "rt");

    if(pArchivo == NULL) {
        printf("Error al Abrir el Archivo\n");
        exit(1);
    }

    while(fgets(buffer, MAX_BUFFER, pArchivo) != NULL) {
        trozado(buffer, configTablero, &i);
        i++;
    }

    fclose(pArchivo);
}

/**
 * @brief Divide una línea del archivo de configuración y asigna el valor a la estructura.
 *
 * Busca el carácter “:” y separa clave-valor. Luego, según el índice de línea,
 * guarda el valor correspondiente (filas, columnas, vidas, etc.).
 *
 * @param linea Cadena leída del archivo.
 * @param configTablero Estructura de configuración a completar.
 * @param elem Índice del campo actual.
 */

void trozado(char* linea, Configuracion* configTablero, int* elem) {

    char* pLim = strrchr(linea, ':');
    *pLim = '\0';
    pLim++;

    if(*elem == 0) {
        sscanf(pLim, "%d", &(configTablero->filas));
    } else if(*elem == 1) {
        sscanf(pLim, "%d", &(configTablero->columnas));
    } else if(*elem == 2) {
        sscanf(pLim, "%d", &(configTablero->vidas));
    } else if(*elem == 3) {
        sscanf(pLim, "%d", &(configTablero->max_fantasmas));
    } else if(*elem == 4) {
        sscanf(pLim, "%d", &(configTablero->max_premios));
    } else if(*elem == 5){
        sscanf(pLim, "%d", &(configTablero->max_vidas));
    } else {
        printf("Error al leer archivo de configuracion\n");
    }

}

/**
 * @brief Reserva memoria para el tablero y crea las estructuras base.
 *
 * Asigna memoria para la matriz del tablero y el array de fantasmas.
 *
 * @param configTablero Configuración general del tablero.
 * @param juegoTablero Puntero al tablero del juego.
 * @param fantasmas Doble puntero donde se almacenarán los fantasmas.
 */

void asignarMemoria(Configuracion* configTablero, Tablero* juegoTablero, Posicion** fantasmas) {

    asignarJuego(configTablero, juegoTablero);
    crearFantasmas(configTablero, fantasmas, juegoTablero);

}

/**
 * @brief Asigna memoria dinámica para las posiciones de los fantasmas.
 *
 * Si la asignación falla, libera recursos del tablero y finaliza el programa.
 *
 * @param configTablero Configuración con la cantidad máxima de fantasmas.
 * @param fantasmas Doble puntero al arreglo de fantasmas.
 * @param juegoTablero Puntero al tablero actual.
 */

void crearFantasmas(Configuracion* configTablero, Posicion** fantasmas, Tablero* juegoTablero) {

    *fantasmas = (Posicion*)malloc(sizeof(Posicion) * configTablero->max_fantasmas);

    if(!(*fantasmas)) {
        printf("Error al asignar memoria para los fantasmas\n");
        destruirJuego(juegoTablero); ///ACA
        exit(1);
    }

}

/**
 * @brief Genera un nuevo tablero de juego con todos sus elementos.
 *
 * Define los límites, entrada/salida, paredes, fantasmas, premios, vidas y jugador inicial.
 *
 * @param configTablero Configuración general.
 * @param juegoTablero Puntero al tablero del juego.
 * @param jugadorPos Puntero a la posición actual del jugador.
 * @param fantasmas Doble puntero a las posiciones de los fantasmas.
 */

void crearJuego(Configuracion* configTablero, Tablero* juegoTablero, Posicion* jugadorPos, Posicion** fantasmas) {

    Posicion entradaTabla, salidaTabla;

    asignarBordes(juegoTablero);
    asignarEntradaSalida(juegoTablero, &entradaTabla, &salidaTabla);
    asignarParedesCamino(configTablero, juegoTablero, &entradaTabla, &salidaTabla);
    asignarFantasmas(configTablero, juegoTablero, fantasmas);
    asignarPremio(configTablero, juegoTablero);
    asignarVidas(configTablero, juegoTablero);
    asignarJugador(juegoTablero, &entradaTabla, jugadorPos);

}

/**
 * @brief Reserva memoria dinámica para la matriz del tablero de juego.
 *
 * Crea una matriz bidimensional de enteros según las filas y columnas definidas
 * en la configuración.
 *
 * @param configTablero Configuración con tamaño del tablero.
 * @param juegoTablero Puntero al tablero donde se asignará la memoria.
 */

void asignarJuego(Configuracion* configTablero, Tablero* juegoTablero) {

    int i, j;

    juegoTablero->filas = configTablero->filas;
    juegoTablero->columnas = configTablero->columnas;

    juegoTablero->celdas = malloc(configTablero->filas * sizeof(int*));

    if(juegoTablero->celdas == NULL) {
        printf("Error al asignar memoria a las filas\n");
        exit(1);
    }

    for(i = 0; i < configTablero->filas; i++) {
        juegoTablero->celdas[i] = malloc(configTablero->columnas * sizeof(int));

        if(juegoTablero->celdas[i] == NULL) {
            printf("Error al asignar memoria a las columnas\n");
            for(j = 0; j < i; j++) {
                free(juegoTablero->celdas[j]);
            }
            free(juegoTablero->celdas);
            exit(1);
        }
    }

}

/**
 * @brief Asigna los bordes del tablero como límites y el resto como espacio vacío.
 *
 * Recorre toda la matriz asignando `LIMITE` en los bordes y `VACIO` en el interior.
 *
 * @param juegoTablero Puntero al tablero actual.
 */

void asignarBordes(Tablero* juegoTablero) {

    int i, j;

    for(i = 0; i < juegoTablero->filas; i++) {
        for(j = 0; j < juegoTablero->columnas; j++) {
            if((i == 0) || (i == (juegoTablero->filas - 1)) || (j == 0) || (j == (juegoTablero->columnas - 1))) {
                juegoTablero->celdas[i][j] = LIMITE;
            } else {
                juegoTablero->celdas[i][j] = VACIO;
            }
        }
    }

}

/**
 * @brief Define posiciones aleatorias para la entrada y salida del tablero.
 *
 * Selecciona aleatoriamente un borde del tablero para colocar la entrada y el opuesto para la salida.
 *
 * @param juegoTablero Tablero del juego.
 * @param entradaTablero Puntero a la posición donde se guardará la entrada.
 * @param salidaTablero Puntero a la posición donde se guardará la salida.
 */

void asignarEntradaSalida(Tablero* juegoTablero, Posicion* entradaTablero, Posicion* salidaTablero) {

    int ladoEntrada, entradaPos, salidaPos;

    ladoEntrada = (rand()%4);

    switch (ladoEntrada) {
        case 0: // Arriba
            entradaPos = 1 + rand() % (juegoTablero->columnas - 2);
            salidaPos  = 1 + rand() % (juegoTablero->columnas - 2);

            juegoTablero->celdas[0][entradaPos] = ENTRADA;
            juegoTablero->celdas[juegoTablero->filas - 1][salidaPos] = SALIDA;

            entradaTablero->fila = 0;
            entradaTablero->columna = entradaPos;
            salidaTablero->fila = juegoTablero->filas - 1;
            salidaTablero->columna = salidaPos;
            break;

        case 1: // Abajo
            entradaPos = 1 + rand() % (juegoTablero->columnas - 2);
            salidaPos  = 1 + rand() % (juegoTablero->columnas - 2);

            juegoTablero->celdas[juegoTablero->filas - 1][entradaPos] = ENTRADA;
            juegoTablero->celdas[0][salidaPos] = SALIDA;

            entradaTablero->fila = juegoTablero->filas - 1;
            entradaTablero->columna = entradaPos;
            salidaTablero->fila = 0;
            salidaTablero->columna = salidaPos;
            break;

        case 2: // Izquierda
            entradaPos = 1 + rand() % (juegoTablero->filas - 2);
            salidaPos  = 1 + rand() % (juegoTablero->filas - 2);

            juegoTablero->celdas[entradaPos][0] = ENTRADA;
            juegoTablero->celdas[salidaPos][juegoTablero->columnas - 1] = SALIDA;

            entradaTablero->fila = entradaPos;
            entradaTablero->columna = 0;
            salidaTablero->fila = salidaPos;
            salidaTablero->columna = juegoTablero->columnas - 1;
            break;

        case 3: // Derecha
            entradaPos = 1 + rand() % (juegoTablero->filas - 2);
            salidaPos  = 1 + rand() % (juegoTablero->filas - 2);

            juegoTablero->celdas[entradaPos][juegoTablero->columnas - 1] = ENTRADA;
            juegoTablero->celdas[salidaPos][0] = SALIDA;

            entradaTablero->fila = entradaPos;
            entradaTablero->columna = juegoTablero->columnas - 1;
            salidaTablero->fila = salidaPos;
            salidaTablero->columna = 0;
            break;
    }

}

/**
 * @brief Genera paredes aleatorias y valida que haya un camino entre entrada y salida.
 *
 * Intenta crear paredes aleatorias dentro del tablero y valida mediante BFS
 * que haya conectividad entre la entrada y la salida. Si no se logra tras
 * varios intentos, se limpia el tablero.
 *
 * @param configTablero Configuración general con los parámetros de generación.
 * @param juegoTablero Tablero del juego.
 * @param entradaTablero Posición de la entrada.
 * @param salidaTablero Posición de la salida.
 */

void asignarParedesCamino(Configuracion* configTablero, Tablero* juegoTablero, Posicion* entradaTablero, Posicion* salidaTablero) {

    int intentos = 0;
    int tableroValido = 0;
    const int MAX_INTENTOS = 400;

    while(!tableroValido && intentos < MAX_INTENTOS) {

        if(intentos > 0) {
            limpiarParedes(juegoTablero);
        }

        generarParedesAleatorias(configTablero, juegoTablero);
        tableroValido = validarTablero(juegoTablero, entradaTablero, salidaTablero);
        intentos++;
    }

    if(!tableroValido) {
        limpiarParedes(juegoTablero);
    }

}

/**
 * @brief Imprime en pantalla el estado actual del tablero.
 *
 * Muestra cada celda con un carácter representativo y debajo imprime
 * el número de vidas y puntaje del jugador.
 *
 * @param tablero Puntero al tablero actual.
 * @param jugadorAct Jugador con sus datos actuales.
 */

void imprimirTablero(Tablero* tablero, Jugador* jugadorAct) {

    int i, j;

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    for(i = 0; i < tablero->filas; i++) {
        for(j = 0; j < tablero->columnas; j++) {
            char c;
            switch(tablero->celdas[i][j]) {
                case 0: c = '.'; break;  // VACIO
                case 1: c = '#'; break;  // LIMITE
                case 2: c = 'E'; break;  // ENTRADA
                case 3: c = 'S'; break;  // SALIDA
                case 4: c = 'F'; break;  // FANTASMA
                case 5: c = 'P'; break;  // PUNTOS
                case 6: c = '#'; break;  // PARED
                case 7: c = 'J'; break;  // JUGADOR
                case 8: c = 'V'; break;  // VIDA
                default: c = '?'; break;
            }
            printf("%c ", c);
        }
        printf("\n");
    }

    printf("\nVidas: %d  |  Puntaje: %d\n", jugadorAct->vidas, jugadorAct->puntuacion);
}

/**
 * @brief Limpia todas las paredes del tablero dejando solo los límites.
 *
 * Restaura todas las celdas internas a estado `VACIO`.
 *
 * @param juegoTablero Puntero al tablero a limpiar.
 */

void limpiarParedes(Tablero* juegoTablero) {

    int i, j;

    for(i = 1; i < juegoTablero->filas - 1; i++) {
        for(j = 1; j < juegoTablero->columnas - 1; j++) {
            juegoTablero->celdas[i][j] = VACIO;
        }
    }

}

/**
 * @brief Genera paredes internas de forma aleatoria dentro del tablero.
 *
 * Calcula una cantidad proporcional de paredes según las celdas libres
 * y distribuye aleatoriamente las posiciones sin bloquear todo el camino.
 *
 * @param configTablero Configuración con parámetros del tablero.
 * @param juegoTablero Tablero sobre el cual se generarán las paredes.
 */

void generarParedesAleatorias(Configuracion* configTablero, Tablero* juegoTablero) {

    int i, fila, columna, celdasInternas, celdasOcupadas, totalParedes;

    celdasInternas = (juegoTablero->filas - 2) * (juegoTablero->columnas - 2);
    celdasOcupadas = configTablero->max_fantasmas + configTablero->max_premios + 1;


    totalParedes = (celdasInternas - celdasOcupadas) / 6;

    for(i = 0; i < totalParedes; i++) {
        do {
            fila = (rand() % (juegoTablero->filas - 2)) + 1;
            columna = (rand() % (juegoTablero->columnas - 2)) + 1;
        } while (juegoTablero->celdas[fila][columna] != VACIO);
        juegoTablero->celdas[fila][columna] = PARED;
    }

}

/**
 * @brief Valida que el tablero tenga un camino y conectividad total.
 *
 * Ejecuta BFS para comprobar si la entrada y salida están conectadas,
 * y luego verifica que todas las celdas vacías sean accesibles desde la entrada.
 *
 * @param juegoTablero Tablero del juego.
 * @param entradaTablero Posición de entrada.
 * @param salidaTablero Posición de salida.
 * @return int Retorna 1 si el tablero es válido, 0 si no lo es.
 */

int validarTablero(Tablero* juegoTablero, Posicion* entradaTablero, Posicion* salidaTablero) {

    int hayCaminoES, conectividadTotal;


     hayCaminoES = BFS(juegoTablero, entradaTablero, salidaTablero);

     if(!hayCaminoES) {
        return 0; ///FALLA CRITICA
     }

     conectividadTotal = verificarConectividadTotal(juegoTablero, entradaTablero, salidaTablero);

     if(!conectividadTotal) {
        return 0;
     }

    return 1;
}

/**
 * @brief Implementa una búsqueda BFS para verificar si hay camino entre dos puntos.
 *
 * Recorre el tablero desde la entrada hasta la salida evitando paredes y límites.
 *
 * @param juegoTablero Tablero actual.
 * @param entradaTablero Posición de entrada.
 * @param salidaTablero Posición de salida.
 * @return int 1 si existe un camino, 0 si no.
 */

int BFS(Tablero* juegoTablero, Posicion* entradaTablero, Posicion* salidaTablero) {

    int i, tipoCelda;
    int visitados[juegoTablero->filas][juegoTablero->columnas];
    int celdasExploradas = 0;
    tCola colaZonas;

    Posicion vecDirecciones[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    Posicion celdaIni, celdaAct, nuevaPos;

    memset(visitados, 0, juegoTablero->filas* juegoTablero->columnas* sizeof(int));

    crear_cola(&colaZonas);

    visitados[entradaTablero->fila][entradaTablero->columna] = 1;
    celdaIni.fila = entradaTablero->fila;
    celdaIni.columna = entradaTablero->columna;
    encolar(&colaZonas, &celdaIni, sizeof(Posicion));

    while(!cola_vacia(&colaZonas)) {

        desencolar(&colaZonas, &celdaAct, sizeof(Posicion));
        celdasExploradas++;  //  CONTAR
        if(celdaAct.fila == salidaTablero->fila && celdaAct.columna == salidaTablero->columna) {
            vaciar_cola(&colaZonas);
            return 1;
        }

        for(i = 0; i < 4; i++) {

            nuevaPos.fila = celdaAct.fila + vecDirecciones[i].fila;
            nuevaPos.columna = celdaAct.columna + vecDirecciones[i].columna;
            if(nuevaPos.fila < 0 || nuevaPos.fila >= juegoTablero->filas || nuevaPos.columna < 0 || nuevaPos.columna >= juegoTablero->columnas) {
                continue;
            }

            if(visitados[nuevaPos.fila][nuevaPos.columna] == 1) {
                continue;
            }

            tipoCelda = juegoTablero->celdas[nuevaPos.fila][nuevaPos.columna];

            if(tipoCelda == PARED || tipoCelda == LIMITE) {
                continue;
            }

            visitados[nuevaPos.fila][nuevaPos.columna] = 1;
            encolar(&colaZonas, &nuevaPos, sizeof(Posicion));
        }

     }

    vaciar_cola(&colaZonas);
    return 0;
}

/**
 * @brief Verifica si todas las celdas vacías son alcanzables desde la entrada.
 *
 * Utiliza un recorrido BFS para marcar todas las zonas accesibles y verifica
 * que ninguna celda vacía quede aislada.
 *
 * @param juegoTablero Tablero del juego.
 * @param entradaTablero Posición de entrada.
 * @param salidaTablero Posición de salida.
 * @return int 1 si todas las celdas son accesibles, 0 en caso contrario.
 */

int verificarConectividadTotal(Tablero* juegoTablero, Posicion* entradaTablero, Posicion* salidaTablero) {


    int i, j; ///i representa filas y j columnas
    int visitados[juegoTablero->filas][juegoTablero->columnas];

    marcasZonasAlcanzables(juegoTablero, entradaTablero, visitados);

    for(i = 1; i < juegoTablero->filas - 1; i++) {

        for(j = 1; j < juegoTablero->columnas - 1; j++) {

            if(juegoTablero->celdas[i][j] == VACIO) {
                if(visitados[i][j] == 0) {
                    return 0;
                }
            }
        }
    }

    if(visitados[salidaTablero->fila][salidaTablero->columna] == 0) {
        return 0;
    }


    return 1;
}

/**
 * @brief Marca todas las celdas alcanzables desde la entrada usando BFS.
 *
 * Rellena la matriz `visitados` con 1 en las posiciones alcanzables y 0 en las no accesibles.
 *
 * @param juegoTablero Tablero de juego.
 * @param entradaTablero Posición inicial de entrada.
 * @param visitados Matriz de enteros usada para marcar las zonas exploradas.
 */

void marcasZonasAlcanzables(Tablero* juegoTablero, Posicion* entradaTablero, int visitados[juegoTablero->filas][juegoTablero->columnas]) {

    int i, nuevaFila, nuevaCol, tipoCelda;
    tCola colaZonas;
    Posicion vecDirecciones[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    Posicion celdaIni, celdaAct, nuevaPos;

    memset(visitados, 0, juegoTablero->filas * juegoTablero->columnas * sizeof(int)); ///Para poner todos los bytes en 0, que en el caso del tipo int representa los 0 en binario

    crear_cola(&colaZonas);

    visitados[entradaTablero->fila][entradaTablero->columna] = 1;
    celdaIni.fila = entradaTablero->fila;
    celdaIni.columna = entradaTablero->columna;
    encolar(&colaZonas, &celdaIni, sizeof(Posicion));

    while(!cola_vacia(&colaZonas)) {

        desencolar(&colaZonas, &celdaAct, sizeof(Posicion));

        for(i = 0; i < 4; i++) {

            nuevaFila = celdaAct.fila + vecDirecciones[i].fila;
            nuevaCol = celdaAct.columna + vecDirecciones[i].columna;

            ///VALIDAR LIMITES
            if(nuevaFila < 0 || nuevaFila >= juegoTablero->filas || nuevaCol < 0 || nuevaCol >= juegoTablero->columnas) {
                continue;
            }

            ///VALIDAR LIMITES
            if(visitados[nuevaFila][nuevaCol] == 1) {
                continue;
            }

            tipoCelda = juegoTablero->celdas[nuevaFila][nuevaCol];
            if(tipoCelda == PARED || tipoCelda == LIMITE) {
                continue;
            }

            visitados[nuevaFila][nuevaCol] = 1;

            nuevaPos.fila = nuevaFila;
            nuevaPos.columna = nuevaCol;
            encolar(&colaZonas, &nuevaPos, sizeof(Posicion));

        }

    }


    vaciar_cola(&colaZonas);
}

/**
 * @brief Coloca los fantasmas en posiciones aleatorias válidas dentro del tablero.
 *
 * Genera coordenadas aleatorias para cada fantasma, asegurándose de que la celda esté vacía.
 * Cada fantasma se ubica en una posición interna (no en los bordes) para evitar colisiones con límites.
 *
 * @param configTablero Configuración general con la cantidad máxima de fantasmas.
 * @param juegoTablero Tablero del juego donde se colocan los fantasmas.
 * @param fantasmas Doble puntero al arreglo dinámico de posiciones de fantasmas.
 */

void asignarFantasmas(Configuracion* configTablero, Tablero* juegoTablero, Posicion** fantasmas) {

    int i, fila, columna;

    for(i = 0; i < configTablero->max_fantasmas; i++) {
        do {
            fila = 1 + rand() % (juegoTablero->filas - 2);
            columna = 1 + rand() % (juegoTablero->columnas - 2);
        } while (juegoTablero->celdas[fila][columna] != VACIO);

        (*fantasmas)[i].fila = fila;
        (*fantasmas)[i].columna = columna;
        juegoTablero->celdas[fila][columna] = FANTASMA;
    }

}

/**
 * @brief Asigna premios en posiciones aleatorias del tablero.
 *
 * Cada premio se coloca en una celda vacía (representada por `VACIO`).
 * Los premios incrementan el puntaje del jugador cuando son recogidos.
 *
 * @param configTablero Configuración con la cantidad máxima de premios.
 * @param juegoTablero Tablero donde se ubicarán los premios.
 */

void asignarPremio(Configuracion* configTablero, Tablero* juegoTablero) {

    int i, fila, columna;

    for(i = 0; i < configTablero->max_premios; i++) {
        do {
            fila = 1 + rand() % (juegoTablero->filas - 2);
            columna = 1 + rand() % (juegoTablero->columnas - 2);
        } while (juegoTablero->celdas[fila][columna] != VACIO);
        juegoTablero->celdas[fila][columna] = PREMIO;
    }

}

/**
 * @brief Coloca vidas extra en el tablero en posiciones aleatorias vacías.
 *
 * Cada vida extra se representa con la celda `VIDA`. Si el jugador la recoge,
 * su contador de vidas aumenta en 1.
 *
 * @param configTablero Configuración con la cantidad máxima de vidas extra.
 * @param juegoTablero Tablero del juego donde se colocarán las vidas.
 */

void asignarVidas(Configuracion* configTablero, Tablero* juegoTablero) {

    int i, fila, columna;

    for(i = 0; i < configTablero->max_vidas; i++) {
        do {
            fila = 1 + rand() % (juegoTablero->filas - 2);
            columna = 1 + rand() % (juegoTablero->columnas - 2);
        } while (juegoTablero->celdas[fila][columna] != VACIO);
        juegoTablero->celdas[fila][columna] = VIDA;
    }

}

/**
 * @brief Posiciona al jugador dentro del tablero en relación con la entrada.
 *
 * El jugador se coloca justo dentro del tablero, adyacente a la celda de entrada,
 * dependiendo de en qué borde se encuentre dicha entrada.
 *
 * @param juegoTablero Tablero del juego.
 * @param entradaTablero Posición de la entrada (fuera del límite).
 * @param jugadorPos Puntero donde se guardará la posición inicial del jugador.
 */

void asignarJugador(Tablero* juegoTablero, Posicion* entradaTablero, Posicion* jugadorPos) {

    int filaJugador = entradaTablero->fila;
    int colJugador = entradaTablero->columna;

    if(filaJugador == 0) {            // Entrada arriba
        filaJugador++;                // jugador debajo
    } else if(filaJugador == juegoTablero->filas - 1) { // Entrada abajo
        filaJugador--;                // jugador arriba
    } else if(colJugador == 0) {     // Entrada izquierda
        colJugador++;                 // jugador a la derecha
    } else if(colJugador == juegoTablero->columnas - 1) { // Entrada derecha
        colJugador--;                 // jugador a la izquierda
    }

    jugadorPos->fila = filaJugador;
    jugadorPos->columna = colJugador;
    juegoTablero->celdas[filaJugador][colJugador] = JUGADOR;

}

/**
 * @brief Libera toda la memoria dinámica asociada al tablero del juego.
 *
 * Recorre todas las filas de la matriz de celdas y libera la memoria asignada
 * a cada una, y luego libera la memoria de la matriz principal.
 *
 * @param juegoTablero Tablero del juego que se va a destruir.
 */

void destruirJuego(Tablero* juegoTablero) {

    int i;

    for(i = 0; i < juegoTablero->filas; i++) {
        free(juegoTablero->celdas[i]);
    }
    free(juegoTablero->celdas);

}

/**
 * @brief Rellena una matriz cuadrada con valores de borde.
 *
 * Asigna el valor `1` en los bordes y `0` en el interior, útil
 * para pruebas o inicialización de estructuras simples.
 *
 * @param matriz Matriz de enteros a actualizar.
 * @param fc Tamaño (filas y columnas) de la matriz cuadrada.
 */

void actualizarMatriz(int matriz[][4], int fc) {

    int i, j;

    for(i = 0; i < fc; i++) {
        for(j = 0; j < fc; j++) {
            if((i == 0) || (i == (fc - 1)) || (j == 0) || (j == (fc - 1))) {
                matriz[i][j] = 1;
            } else {
                matriz[i][j] = 0;
            }
        }
    }

}

/**
 * @brief Bucle principal del juego Pacman.
 *
 * Controla el flujo completo de una partida:
 * - Registra movimientos del jugador y de los fantasmas.
 * - Gestiona colisiones, premios y vidas.
 * - Reinicia posiciones cuando el jugador pierde una vida.
 * - Termina cuando se gana o se agotan las vidas.
 *
 * @param configTablero Configuración del tablero y límites.
 * @param juegoTablero Tablero actual de la partida.
 * @param jugadorActual Jugador que está jugando la partida.
 * @param jugadorPos Posición actual del jugador.
 * @param fantasmas Puntero doble al arreglo de posiciones de fantasmas.
 */

void juego(Configuracion* configTablero, Tablero* juegoTablero, Jugador* jugadorActual, Posicion* jugadorPos, Posicion** fantasmas) {

    char tecla;
    int valorMovimiento;
    Posicion entrada;
    Posicion* fantasmasIniciales;
    Movimiento movJ;
    Movimiento movF;
    tCola colaMovimientos;

    jugadorActual->partidas++;
    jugadorActual->cantidadMovimientos = realloc(jugadorActual->cantidadMovimientos, sizeof(int) * jugadorActual->partidas);

    if(!jugadorActual->cantidadMovimientos) {
        destruirJuego(juegoTablero);
        free(*fantasmas);
        exit(1);
    }

    jugadorActual->cantidadMovimientos[jugadorActual->partidas - 1] = 0;

    crear_cola(&colaMovimientos);

    entrada.fila = jugadorPos->fila;
    entrada.columna = jugadorPos->columna;

    jugadorActual->vidas = configTablero->vidas;

    guardarInicioJugador(jugadorPos, &movJ);
    guardarPosicionInicialFantasmas(fantasmas, &fantasmasIniciales, configTablero->max_fantasmas, juegoTablero);

    while(jugadorActual->vidas != 0) {
        tecla = leerMovimiento();
        valorMovimiento = moverJugador(configTablero, juegoTablero, jugadorActual, jugadorPos, tecla);

        if (valorMovimiento == 0) {
            continue; // movimiento inválido: no mover fantasmas, no imprimir
        }

        guardarInicioJugador(jugadorPos, &movJ);
        movJ.tecla = tecla;

        jugadorActual->cantidadMovimientos[jugadorActual->partidas - 1]++;
        encolar(&colaMovimientos, &movJ, sizeof(Movimiento));

        if (valorMovimiento == 2) {
            printf("Felicidades! Has llegado a la salida y ganado el juego.\n");
            mostrarRegistro(&colaMovimientos);
            break;
        }

        if(valorMovimiento == 3) {
            asignarJugador(juegoTablero, &entrada, jugadorPos);
            resetearFantasmas(configTablero, juegoTablero, fantasmas, fantasmasIniciales);
        }

        valorMovimiento = moverFantasmas(configTablero, juegoTablero, jugadorPos, fantasmas);
        guardarFantasmas(&colaMovimientos, &movF, fantasmas, configTablero);

        if(valorMovimiento == 1) {
            asignarJugador(juegoTablero, &entrada, jugadorPos);
            resetearFantasmas(configTablero, juegoTablero, fantasmas, fantasmasIniciales);
            jugadorActual->vidas--;
        }

        imprimirTablero(juegoTablero, jugadorActual);
        guardarMapaEnArchivo(juegoTablero, jugadorActual, "partida.txt");
    }

    if(jugadorActual->vidas == 0) {
        printf("¡Perdiste!\n");
        mostrarRegistro(&colaMovimientos);
    }

    printf("Movimientos realizados en esta partida: %d\n",
           jugadorActual->cantidadMovimientos[jugadorActual->partidas - 1]);

    free(fantasmasIniciales);
}

/**
 * @brief Guarda la posición inicial del jugador en un movimiento.
 *
 * Llena la estructura Movimiento con los datos del jugador.
 *
 * @param jugadorPos Posición actual del jugador.
 * @param movJ Estructura Movimiento donde se almacenará la información.
 */

void guardarInicioJugador(Posicion* jugadorPos, Movimiento* movJ) {

    movJ->tipo = 'J';
    movJ->fila = jugadorPos->fila;
    movJ->columna = jugadorPos->columna;

}

/**
 * @brief Registra las posiciones actuales de todos los fantasmas.
 *
 * Encola cada posición de fantasma como un movimiento en la cola general.
 *
 * @param colaMovimientos Cola donde se guardan los movimientos.
 * @param movF Movimiento temporal para almacenar datos.
 * @param fantasmas Doble puntero al arreglo de posiciones de fantasmas.
 * @param configTablero Configuración con el número máximo de fantasmas.
 */

void guardarFantasmas(tCola* colaMovimientos, Movimiento* movF, Posicion** fantasmas, Configuracion* configTablero) {
    int i;

    for(i = 0; i < configTablero->max_fantasmas; i++) {
        movF->tipo = 'F';
        movF->fila = (*fantasmas)[i].fila;
        movF->columna = (*fantasmas)[i].columna;
        encolar(colaMovimientos, movF, sizeof(Movimiento));
    }
}

/**
 * @brief Guarda las posiciones iniciales de los fantasmas para poder reiniciarlos.
 *
 * Copia las coordenadas iniciales en un nuevo arreglo.
 *
 * @param fantasmas Doble puntero al arreglo de posiciones de fantasmas.
 * @param fantasmasIniciales Doble puntero donde se guardará la copia.
 * @param cantidad Número total de fantasmas.
 * @param juegoTablero Tablero del juego (usado para liberar en caso de error).
 */

void guardarPosicionInicialFantasmas(Posicion** fantasmas, Posicion** fantasmasIniciales, int cantidad, Tablero* juegoTablero) {

    int i;

    *fantasmasIniciales = (Posicion*)malloc(sizeof(Posicion) * cantidad);
    if(!(*fantasmasIniciales)) {
        printf("Error al asignar memoria para la posicion inicial de los fantasmas\n");
        free(*fantasmas);
        destruirJuego(juegoTablero);
        exit(1);
    }

    for(i = 0; i < cantidad; i++) {
        (*fantasmasIniciales)[i] = (*fantasmas)[i];
    }
}

/**
 * @brief Muestra en pantalla el registro de movimientos del jugador.
 *
 * Desencola los movimientos del jugador y muestra las teclas presionadas en orden.
 *
 * @param colaMovimientos Cola con todos los movimientos de la partida.
 */

void mostrarRegistro(tCola* colaMovimientos) {
    int primero = 1; // Para formatear los separadores
    Movimiento movAux;

    printf("\n=== REGISTRO DE MOVIMIENTOS ===\n");
    printf("Jugador = [");

    while(!cola_vacia(colaMovimientos)) {
        desencolar(colaMovimientos, &movAux, sizeof(Movimiento));
        if (movAux.tipo == 'J') {
            if (!primero) {
                printf(", "); // separador entre movimientos
            }
            printf("%c", movAux.tecla);
            primero = 0;
        }
    }

    printf("]\n");
    vaciar_cola(colaMovimientos);
}

/**
 * @brief Restaura las posiciones iniciales de los fantasmas después de perder una vida.
 *
 * Limpia las posiciones actuales de los fantasmas y las reemplaza por las guardadas al inicio.
 *
 * @param configTablero Configuración general con la cantidad de fantasmas.
 * @param juegoTablero Tablero del juego.
 * @param fantasmas Doble puntero al arreglo de fantasmas actuales.
 * @param fantasmasIniciales Arreglo de posiciones iniciales.
 */

void resetearFantasmas(Configuracion* configTablero, Tablero* juegoTablero, Posicion** fantasmas, Posicion* fantasmasIniciales) {

    int i, f, c;

    // Limpiar las posiciones actuales
    for (i = 0; i < configTablero->max_fantasmas; i++) {
        f = (*fantasmas)[i].fila;
        c = (*fantasmas)[i].columna;
        juegoTablero->celdas[f][c] = VACIO;
    }

    // Restaurar posiciones iniciales
    for (i = 0; i < configTablero->max_fantasmas; i++) {
        (*fantasmas)[i] = fantasmasIniciales[i];
        f = (*fantasmas)[i].fila;
        c = (*fantasmas)[i].columna;
        juegoTablero->celdas[f][c] = FANTASMA;
    }
}

/**
 * @brief Lee una tecla de movimiento sin necesidad de presionar Enter.
 *
 * Acepta solo las teclas WASD (mayúsculas o minúsculas) y las convierte en minúscula.
 *
 * @return char La tecla de movimiento presionada.
 */

char leerMovimiento(void) {
    char mov;
    do {
        mov = _getch();
    } while (mov != 'w' && mov != 'a' && mov != 's' && mov != 'd'
          && mov != 'W' && mov != 'A' && mov != 'S' && mov != 'D');
    if (mov >= 'A' && mov <= 'Z') mov = (char)(mov - 'A' + 'a');
    return mov;
}

/**
 * @brief Mueve al jugador dentro del tablero según la tecla presionada.
 *
 * Evalúa la celda destino:
 * - Si es pared o límite → movimiento inválido.
 * - Si es salida → gana la partida.
 * - Si es fantasma → pierde una vida.
 * - Si es premio → aumenta puntaje.
 * - Si es vida → suma una vida.
 *
 * @param configTablero Configuración del tablero.
 * @param juegoTablero Tablero actual.
 * @param jugadorActual Jugador en curso.
 * @param jugadorPos Posición actual del jugador.
 * @param tecla Movimiento ingresado ('w','a','s','d').
 * @return int Código de estado (0 = inválido, 1 = válido, 2 = salida, 3 = fantasma).
 */

int moverJugador(Configuracion* configTablero, Tablero* juegoTablero, Jugador* jugadorActual, Posicion* jugadorPos, char tecla) {

    int nuevaFila  = jugadorPos->fila;
    int nuevaColumna = jugadorPos->columna;

    switch(tecla) {
        case 'w': case 'W': nuevaFila--;      break;
        case 'a': case 'A': nuevaColumna--;   break;
        case 's': case 'S': nuevaFila++;      break;
        case 'd': case 'D': nuevaColumna++;   break;
        default: break;
    }

    if(juegoTablero->celdas[nuevaFila][nuevaColumna] == SALIDA) {
        return 2;
    }

    if(nuevaFila <= 0 || nuevaFila >= juegoTablero->filas - 1 ||
       nuevaColumna <= 0 || nuevaColumna >= juegoTablero->columnas - 1) {
        return 0;
    }

    if(juegoTablero->celdas[nuevaFila][nuevaColumna] == PARED) {
        return 0;
    }

    if(juegoTablero->celdas[nuevaFila][nuevaColumna] == FANTASMA) {
        jugadorActual->vidas -= 1;
        juegoTablero->celdas[jugadorPos->fila][jugadorPos->columna] = VACIO;
        return 3;
    }

    if(juegoTablero->celdas[nuevaFila][nuevaColumna] == PREMIO) {
        jugadorActual->puntuacion += 100;
        juegoTablero->celdas[nuevaFila][nuevaColumna] = VACIO;
    }

    if(juegoTablero->celdas[nuevaFila][nuevaColumna] == VIDA) {
        jugadorActual->vidas += 1;
        juegoTablero->celdas[nuevaFila][nuevaColumna] = VACIO;
    }

    juegoTablero->celdas[jugadorPos->fila][jugadorPos->columna] = VACIO;
    jugadorPos->fila = nuevaFila;
    jugadorPos->columna = nuevaColumna;
    juegoTablero->celdas[nuevaFila][nuevaColumna] = JUGADOR;

    return 1;
}

/**
 * @brief Mueve todos los fantasmas un paso hacia el jugador, priorizando la dirección más cercana.
 *
 * Cada fantasma calcula la diferencia entre su posición y la del jugador:
 * - Si la distancia vertical (Δfila) es mayor o igual a la horizontal (Δcolumna),
 *   intenta primero moverse verticalmente hacia el jugador.
 * - Si la distancia horizontal es mayor, prioriza moverse en esa dirección.
 *
 * Luego prueba hasta 4 direcciones posibles en orden de prioridad:
 * 1. Dirección principal hacia el jugador.
 * 2. Segunda dirección que también reduce la distancia.
 * 3. Dirección opuesta a la principal (por si está bloqueado).
 * 4. Dirección opuesta a la secundaria (último recurso).
 *
 * Si un movimiento lleva al fantasma sobre la celda del jugador, este pierde una vida.
 * Si la celda está vacía, el fantasma se mueve allí.
 * Si está bloqueada por una pared o límite, prueba la siguiente opción.
 *
 * @param configTablero Configuración general con cantidad de fantasmas y límites.
 * @param juegoTablero Tablero del juego.
 * @param jugadorPos Posición actual del jugador.
 * @param fantasmas Doble puntero al arreglo de posiciones de los fantasmas.
 * @return int Retorna 1 si al menos un fantasma alcanzó al jugador, 0 si no ocurrió colisión.
 */

int moverFantasmas(Configuracion* configTablero, Tablero* juegoTablero, Posicion* jugadorPos, Posicion** fantasmas) {

    int df, dc, ff, fc, k, t, nf, nc, celda;
    int jugadorMuere = 0;
    int dirs[4][2];

    for (int i = 0; i < configTablero->max_fantasmas; i++) {
        ff = (*fantasmas)[i].fila;
        fc = (*fantasmas)[i].columna;

        // Diferencias con el jugador (para decidir prioridad de movimiento)
        df = jugadorPos->fila - ff;
        dc = jugadorPos->columna - fc;

        // Direcciones candidatas (dy, dx)
        k = 0;

        // Priorizamos la dirección con mayor distancia
        if (abs(df) >= abs(dc)) {
            // primero vertical hacia jugador
            if (df != 0) { dirs[k][0] = (df > 0) ? 1 : -1; dirs[k++][1] = 0; }
            if (dc != 0) { dirs[k][0] = 0; dirs[k++][1] = (dc > 0) ? 1 : -1; }
        } else {
            // primero horizontal hacia jugador
            if (dc != 0) { dirs[k][0] = 0; dirs[k++][1] = (dc > 0) ? 1 : -1; }
            if (df != 0) { dirs[k][0] = (df > 0) ? 1 : -1; dirs[k++][1] = 0; }
        }

        // Alternativas opuestas
        dirs[k][0] = -dirs[0][0]; dirs[k++][1] = -dirs[0][1];
        dirs[k][0] = -dirs[1][0]; dirs[k++][1] = -dirs[1][1];

        // Probar las 4 opciones
        for (t = 0; t < 4; t++) {
            nf = ff + dirs[t][0];
            nc = fc + dirs[t][1];

            if (nf <= 0 || nf >= juegoTablero->filas - 1 ||
                nc <= 0 || nc >= juegoTablero->columnas - 1)
                continue;

            celda = juegoTablero->celdas[nf][nc];

            if (celda == VACIO || celda == JUGADOR) {
                if (celda == JUGADOR) jugadorMuere = 1;

                // Mover fantasma
                juegoTablero->celdas[ff][fc] = VACIO;
                (*fantasmas)[i].fila = nf;
                (*fantasmas)[i].columna = nc;
                juegoTablero->celdas[nf][nc] = FANTASMA;
                break;
            }
        }

        // Si no encuentra movimiento válido, se queda en el mismo lugar (no hacer nada)
    }

    return jugadorMuere;
}

/**
 * @brief Guarda el estado del tablero en un archivo de texto.
 *
 * Crea o sobreescribe un archivo con la representación visual del tablero,
 * las vidas y el puntaje actual del jugador.
 *
 * @param tablero Tablero del juego.
 * @param jugador Jugador con sus vidas y puntaje.
 * @param nombreArchivo Nombre del archivo de salida.
 */

void guardarMapaEnArchivo(Tablero* tablero, Jugador* jugador, const char* nombreArchivo) {

    int i, j;

    FILE* archivo = fopen(nombreArchivo, "w");

    if (!archivo) {
        printf("Error al abrir %s\n", nombreArchivo);
        return;
    }

    for (i = 0; i < tablero->filas; i++) {
        for (j = 0; j < tablero->columnas; j++) {
            char c;
            switch(tablero->celdas[i][j]) {
                case VACIO:    c = '.'; break;
                case LIMITE:   c = '#'; break;
                case ENTRADA:  c = 'E'; break;
                case SALIDA:   c = 'S'; break;
                case FANTASMA: c = 'F'; break;
                case PREMIO:   c = 'P'; break;
                case PARED:    c = '#'; break;
                case JUGADOR:  c = 'J'; break;
                case VIDA:     c = 'V'; break;
                default:       c = '?'; break;
            }
            fprintf(archivo, "%c ", c);
        }
        fprintf(archivo, "\n");
    }

    fprintf(archivo, "\nVidas: %d | Puntaje: %d\n", jugador->vidas, jugador->puntuacion);
    fclose(archivo);
}
