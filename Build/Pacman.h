#ifndef PACMAN_H_INCLUDED
#define PACMAN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include <time.h>

#define MINIMO(X,Y) ((X<Y)?(X):(Y))

#define MAX_BUFFER 28
#define MAX_NOMBRE 10

#define VACIO 0
#define LIMITE 1
#define ENTRADA 2
#define SALIDA 3
#define FANTASMA 4
#define PREMIO 5
#define PARED 6
#define JUGADOR 7
#define VIDA 8

///
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

// Inicializa Winsock
int init_winsock();

// Conecta al servidor y devuelve el socket
SOCKET connect_to_server(const char *server_ip, int port);

// Envía una solicitud y recibe la respuesta
int send_request(SOCKET sock, const char *request, char *response);

// Cierra la conexión y limpia Winsock
void close_connection(SOCKET sock);
///

 typedef struct sNodo {

    void* info;
    unsigned tamInfo;
    struct sNodo* sig;

 }tNodo;

 typedef struct {

    tNodo* pri;
    tNodo* ult;

 }tCola;

typedef struct {

    int filas;
    int columnas;
    int vidas;
    int max_fantasmas;
    int max_premios;
    int max_vidas;

}Configuracion;

typedef struct {

    int filas;
    int columnas;
    int** celdas;

}Tablero;

typedef struct {

    int fila;
    int columna;

}Posicion;

typedef struct {
    char tipo;
    int fila;
    int columna;
    char tecla;
} Movimiento;

typedef struct {

    int id;
    char nombre[MAX_NOMBRE];
    int puntuacion;
    int vidas;
    int partidas;
    int* cantidadMovimientos;
} Jugador;

typedef struct{
    char nombre[MAX_NOMBRE];
    int puntuacion;
    int cantidadMovimientos;
}tUsuario;

///=================MENÚ Y PRINCIPAL=================
void mostrarMenu();
tUsuario jugarPartida(char* archivoConfig);

///=================ARCHIVO=================
void guardarMapaEnArchivo(Tablero* tablero, Jugador* jugador, const char* nombreArchivo);

///=================UTILIDADES=================
void actualizarMatriz(int matriz[][4], int fc);
void mostrarMatriz(int matriz[][4], int fc);

///===============COLA DINÁMICA===============
void crear_cola(tCola* pc);
void vaciar_cola(tCola* pc);
int encolar(tCola* pc, const void* elem, unsigned tamElem);
int ver_primero_cola(const tCola* pc, void* elem, unsigned tamElem);
int desencolar(tCola* pc, void* elem, unsigned tamElem);
int cola_vacia(const tCola* pc);
int cola_llena(const tCola* pc, unsigned tamElem);

///===============CONFIGURACIÓN===============
void inicializarJugador(Configuracion configTablero, Jugador* jugadorActual);
void leerConfiguracion(const char* file_name, Configuracion* configTablero, size_t tamElem);
void trozado(char* linea, Configuracion* configTablero, int* elem);

///=================CREACIÓN==================
void asignarMemoria(Configuracion* configTablero, Tablero* juegoTablero, Posicion** fantasmas);
void crearFantasmas(Configuracion* configTablero, Posicion** fantasmas, Tablero* juegoTablero);
void crearJuego(Configuracion* configTablero, Tablero* juegoTablero, Posicion* jugadorPos, Posicion** fantasmas);

void asignarJuego(Configuracion* configTablero, Tablero* juegoTablero);
void asignarBordes(Tablero* juegoTablero);
void asignarEntradaSalida(Tablero* juegoTablero, Posicion* entradaTablero, Posicion* salidaTablero);

void asignarParedesCamino(Configuracion* configTablero, Tablero* juegoTablero, Posicion* entradaTablero, Posicion* salidaTablero);
void limpiarParedes(Tablero* juegoTablero);
void generarParedesAleatorias(Configuracion* configTablero, Tablero* juegoTablero);

int validarTablero(Tablero* juegoTablero, Posicion* entradaTablero, Posicion* salidaTablero);
int verificarConectividadTotal(Tablero* juegoTablero, Posicion* entradaTablero, Posicion* salidaTablero);
void marcasZonasAlcanzables(Tablero* juegoTablero, Posicion* entradaTablero, int visitados[juegoTablero->filas][juegoTablero->columnas]);
int BFS(Tablero* juegoTablero, Posicion* entradaTablero, Posicion* salidaTablero);

void asignarFantasmas(Configuracion* configTablero, Tablero* juegoTablero, Posicion** fantasmas);
void asignarPremio(Configuracion* configTablero, Tablero* juegoTablero);
void asignarVidas(Configuracion* configTablero, Tablero* juegoTablero);
void asignarJugador(Tablero* juegoTablero, Posicion* entradaTablero, Posicion* jugadorPos);

void imprimirTablero(Tablero* tablero, Jugador* jugadorAct);

///=================MOVIMIENTOS================
void juego(Configuracion* configTablero, Tablero* juegoTablero, Jugador* jugadorActual, Posicion* jugadorPos, Posicion** fantasmas);
void guardarInicioJugador(Posicion* jugadorPos, Movimiento* movJ);
void guardarFantasmas(tCola* colaMovimientos, Movimiento* movF, Posicion** fantasmas, Configuracion* configTablero);
void mostrarRegistro(tCola* colaMovimientos);
void guardarPosicionInicialFantasmas(Posicion** fantasmas, Posicion** fantasmasIniciales, int cantidad, Tablero* juegoTablero);
char leerMovimiento();
int moverJugador(Configuracion* configTablero, Tablero* juegoTablero, Jugador* jugadorActual, Posicion* jugadorPos, char tecla);
int moverFantasmas(Configuracion* configTablero, Tablero* juegoTablero, Posicion* jugadorPos, Posicion** fantasmas);
void resetearFantasmas(Configuracion* configTablero, Tablero* juegoTablero, Posicion** fantasmas, Posicion* fantasmasIniciales);

///=================LIBERACIÓN================
void destruirJuego(Tablero* juegoTablero);


#endif // PACMAN_H_INCLUDED
