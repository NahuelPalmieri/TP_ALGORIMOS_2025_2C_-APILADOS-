#ifndef MOVIMIENTOS_H_INCLUDED
#define MOVIMIENTOS_H_INCLUDED

#include "estructurasYMacros.h"
#include "cola dinamica.h"
#include <string.h>


/**
 * Descripcion: La funcion se encarga de llamar a muchas otras funciones, con la intencion de que por cada ronda transcurrida
 * (un movimiento por parte del jugador y un movimiento para cada fantasma por parte de la maquina), muestre en la pantalla
 * una actualizacion del mapa, y de las estadisticas del jugador (cuantas vidas y cuantos puntos acumulados tiene en tiempo real).
 * Esta funcion encola los movimientos realizados por el jugador en tCola * colaMovJugador y cuenta cuantos movimientos va a haciendo
 * el jugador.
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego donde encontraremos el mapa del juego con todas las posiciones a analizar.
 * tEstadisticas * estadisticas: Puntero a la estructura tEstadisticas donde encontraremos informacion de la cantidad de items
 * especiales que quedan en el mapa.
 * tCola * colaMovJugador: variable del tipo tCola en donde se van a encolar los movimientos del jugador.
 * tCola * colaMovMaquina: variable del tipo tCola en donde se van a encolar los movimientos de la maquina (encola fantasma por fantasma).
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void moversePorElMapa(tTablero * tablero, tEstadisticas * estadisticas, tCola * colaMovJugador, tCola * colaMovMaquina);

/**
 * Descripcion: La funcion mueve a los fantasmas en caso de ser posible, teniendo en cuenta el valor que recibe de la funcion
 * int verificarMovimientosValidosFantasmas(tTablero * tablero, int i, int movEnX, int movEnY) a la que invoca. Analiza cada
 * caso posible que le puede ocurrir a un fantasma, y en caso de ser posible, actualiza la posicion en el tablero y en las
 * matrices de posiciones correspondiente (y tambien tiene en cuenta el estado del fantasma, es decir, si esta vivo o muerto y lo actualiza).
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego donde encontraremos el mapa del juego con todas las posiciones a analizar.
 * tEstadisticas * estadisticas: Puntero a la estructura tEstadisticas donde encontraremos informacion de la cantidad de items
 * especiales que quedan en el mapa (en este caso nos sirve mas que todo para descontar el contador de fantasmas al momento de
 * que uno de estos alcance a un jugador).
 * int movEnX: Variable del tipo entero que registra la cantidad de posiciones que el fantasma se quiere
 * mover sobre las filas (3 valores posibles: -1, 0, 1).
 * int movEnY: Variable del tipo entero que registra la cantidad de posiciones que el fantasma se quiere
 * mover sobre las columnas (3 valores posibles: -1, 0, 1).
 * int i: variable del tipo entero que registra el numero de fantasma con el que estamos trabajando. El
 * rango de valores posibles de esta variable es desde i = 0, hasta i < (tablero->Posantasmas.contItems).
 *
 * Valor De Retorno: de tipo caracter, retorna un caracter que indica hacia que direccion se movio el jugador. Ese valor
 * luego es enviado a la llamada a funcion void encolarMovimientos(tCola * cola, char movimiento), para que a traves de
 * ese caracter encole el string correspondiente (STRING_MOV_W: "ARRIBA", STRING_MOV_S: "ABAJO", STRING_MOV_A: "IZQUIERDA",
 * STRING_MOV_D: "DERECHA").
 */
char moverAlJugador(tTablero * tablero, tEstadisticas * estadisticas);

/**
 * Descripcion: La funcion itera y llama a funciones de manera sucesivas (en este caso lo hace la maquina),
 * de tal manera que un fantasma (de los varios que tengo) se mueva hacia una direccion (numero del 0 al 3
 * que se corresponde con 4 macros en especifico: ARRIBA, ABAJO, IZQUIERDA, DERECHA). Esta direccion es
 * determinada de manera "aleatoria", pero intentando seguir al jugador para acorralarlo y atraparlo.
 * Esto lo hace mediante el uso de probabilidades, en las que por un lado, puede mover a un fantasma una posicion
 * de manera aleatoria (con una probabilidad de 10% de que eso suceda), o mover a un fantasma una posicion de
 * manera tal que este intente acorralar al jugador y atraparlo (con una probabilidad de 90% de que eso suceda).
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego donde encontraremos el mapa del juego con todas las posiciones a analizar.
 * tEstadisticas * estadisticas: Puntero a la estructura tEstadisticas donde encontraremos informacion de la cantidad de items
 * especiales que quedan en el mapa (en este caso nos sirve mas que todo para descontar el contador de fantasmas al momento de
 * que uno de estos alcance a un jugador).
 * int movEnX: Variable del tipo entero que registra la cantidad de posiciones que el fantasma se quiere
 * mover sobre las filas (3 valores posibles: -1, 0, 1).
 * int movEnY: Variable del tipo entero que registra la cantidad de posiciones que el fantasma se quiere
 * mover sobre las columnas (3 valores posibles: -1, 0, 1).
 * int i: variable del tipo entero que registra el numero de fantasma con el que estamos trabajando. El
 * rango de valores posibles de esta variable es desde i = 0, hasta i < (tablero->Posantasmas.contItems).
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void moverALosFantasmas(tTablero * tablero, tEstadisticas * estadisticas, tCola * colaMovMaquina);

/**
 * Descripcion: La funcion mueve a los fantasmas en caso de ser posible, teniendo en cuenta el valor que recibe de la funcion
 * int verificarMovimientosValidosFantasmas(tTablero * tablero, int i, int movEnX, int movEnY) a la que invoca. Analiza cada
 * caso posible que le puede ocurrir a un fantasma, y en caso de ser posible, actualiza la posicion en el tablero y en las
 * matrices de posiciones correspondiente (y tambien tiene en cuenta el estado del fantasma, es decir, si esta vivo o muerto y lo actualiza).
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego donde encontraremos el mapa del juego con todas las posiciones a analizar.
 * tEstadisticas * estadisticas: Puntero a la estructura tEstadisticas donde encontraremos informacion de la cantidad de items
 * especiales que quedan en el mapa (en este caso nos sirve mas que todo para descontar el contador de fantasmas al momento de
 * que uno de estos alcance a un jugador).
 * int movEnX: Variable del tipo entero que registra la cantidad de posiciones que el fantasma se quiere
 * mover sobre las filas (3 valores posibles: -1, 0, 1).
 * int movEnY: Variable del tipo entero que registra la cantidad de posiciones que el fantasma se quiere
 * mover sobre las columnas (3 valores posibles: -1, 0, 1).
 * int i: variable del tipo entero que registra el numero de fantasma con el que estamos trabajando. El
 * rango de valores posibles de esta variable es desde i = 0, hasta i < (tablero->Posantasmas.contItems).
 *
 * Valor De Retorno: de tipo int, retorna un entero que define si el movimiento que el fantasma numero (i) queria
 * hacer, se pudo o no (retorno a partir de dos macros: MOVIMIENTOS_FANTASMA_EFECTUADO -> 60,
 * MOVIMIENTOS_FANTASMA_NO_EFECTUADO -> 61).
 */
int moverFantasmasSiSePuede(tTablero * tablero, tEstadisticas * estadisticas, int i, int movEnX, int movEnY);

/**
 * Descripcion: La funcion itera y espera mientras aguarda a que el usuario (en este caso el jugador),
 * inserte la opcion que desea realizar (en este caso: 'w' o 'W' para arriba, 's' o 'S' para abajo, 'a'
 * o 'A' para la izquierda, 'd' o 'D' para la derecha, giuiandose de la macro CADENA_DE_MOVIMIENTOS_VALIDOS).
 * Hasta que la opcion ingresada por el usuario no este contemplada en el string (opcionesValidas) recibido
 * como parametro, la funcion va a seguir esperando a que el jugador ingrese una opcion que este incluida
 * dentro de las opciones validas.
 *
 * Parametros:
 * const char * opcionesValidas: puntero a cadena de tipo const (ya que no se va a modificar a lo largo de
 * la ejecucion de la funcion), con el texto de las opciones validas a verificar en la funcion. En el caso
 * de este juego, lo enviamos como parametro a traves de la utilizacion de una macro (CADENA_DE_MOVIMIENTOS_VALIDOS).
 * Este parametro se utilizara para validar el corte del ciclo do - while de la funcion (el que determina
 * hasta cuando voy a pedirle al usuario que ingrese una opcion valida).
 *
 * Valor De Retorno: de tipo char, retorna un caracter valido que se encuentre dentro de la cadena
 * CADENA_DE_MOVIMIENTOS_VALIDOS que le va a indicar a la funcion que la llama hacia donde pretende
 * moverse el jugador.
 */
char obtenerSaloicitudDeMovimiento(const char * caracteresValidos);

/**
 * Descripcion: Consulta e informa cual es el estado de la celda a la que el jugador quiere acceder
 * mediante su solicitud de movimiento.
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego donde encontraremos el mapa a analizar.
 * int movEnX: Variable del tipo entero que registra la cantidad de posiciones que el jugador se quiere
 * mover sobre las filas (3 valores posibles: -1, 0, 1).
 * int movEnY: Variable del tipo entero que registra la cantidad de posiciones que el jugador se quiere
 * mover sobre las columnas (3 valores posibles: -1, 0, 1).
 *
 * Valor De Retorno: de tipo int, el valor de retorno indica que tipo de celda va a pisar el jugador en
 * caso de que se efectue el movimiento (estos valores surgen de las macros: PISANDO_FRONTERAS -> 30,
 * PISANDO_CAMINOS -> 31, PISANDO_FANTASMAS -> 32, PISANDO_VIDAS_EXTRA -> 33, PISANDO_PREMIOS -> 34,
 * PISANDO_SALIDA -> 35).
 */
int verificarMovimientosValidos(tTablero * tablero, int movEnX, int movEnY);

/**
 * Descripcion: Consulta e informa cual es el estado de la celda a la que la maquina quiere acceder
 * mediante las posiciones en tiempo real de los multiples fantasmas con vida.
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego donde encontraremos el mapa a analizar.
 * int movEnX: Variable del tipo entero que registra la cantidad de posiciones que el fantasma se quiere
 * mover sobre las filas (3 valores posibles: -1, 0, 1).
 * int movEnY: Variable del tipo entero que registra la cantidad de posiciones que el fantasma se quiere
 * mover sobre las columnas (3 valores posibles: -1, 0, 1).
 * int i: variable del tipo entero que registra el numero de fantasma con el que estamos trabajando. El
 * rango de valores posibles de esta variable es desde i = 0, hasta i < (tablero->Posantasmas.contItems).
 *
 * Valor De Retorno: de tipo int, el valor de retorno indica que tipo de celda va a pisar el fantasma en
 * caso de que se efectue el movimiento (estos valores surgen de las macros: PISANDO_FRONTERAS -> 30,
 * PISANDO_CAMINOS -> 31, PISANDO_FANTASMAS -> 32, PISANDO_VIDAS_EXTRA -> 33, PISANDO_PREMIOS -> 34,
 * PISANDO_JUGADOR -> 35, PISANDO_SALIDA -> 35).
 */
int verificarMovimientosValidosFantasmas(tTablero * tablero, int i, int movEnX, int movEnY);

/**
 * Descripcion: Muestra (mediante una impresion en pantalla) las estadisticas en tiempo real del jugador, tales
 * como cuantas vidas y cuantos premios acumulados tiene.
 *
 * Parametros:
 * tEstadisticas * estadisticas: Puntero a la estructura de estadisticas donde se encuentran los datos de (vidas)
 * y (contadorPremios) dentro de esta estructura
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void mostrarEstadisticas(tEstadisticas * estadisticas);

/**
 * Descripcion: Guarda el movimiento enviado por parametro (movimiento) que fue realizando el jugador en cada iteracion de la partida.
 *
 * Parametros:
 * tCola * cola: Puntero del tipo tCola donde se encuentran encolados los movimientos que fue realizando el jugador a lo largo de la parida.
 * char movimiento: tipo de dato caracter que almacena 8 valores posibles ('w' o 'W' (ARRIBA), 's' o 'S' (ABAJO), 'a' o 'A' (IZQUIERDA),
 * 'd' o 'D' (DERECHA)), para luego ser procesado y poder poner en la cola la cadena correspondiente ("ARRIBA", "ABAJO", "IZQUIERDA", "DERECHA").
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void encolarMovimientos(tCola * cola, char movimiento);

/**
 * Descripcion: Guarda el movimiento enviado por parametro (movimiento) que fue realizando el jugador en cada iteracion de la partida.
 *
 * Parametros:
 * tCola * cola: Puntero del tipo tCola donde se encuentran encolados los movimientos que fue realizando el jugador a lo largo de la parida.
 * int movimiento: tipo de dato entero que almacena 4 valores posibles asignados por macros (ARRIBA: 20, ABAJO: 21, IZQUIERDA: 22, DERECHA: 23),
 * para luego ser procesado y luego en la funcion void mostrarMovimientosEncoladosDeFantasmas(tCola * cola) poder mostrar las cadenas
 * correspondientes (STRING_MOV_W: "ARRIBA", STRING_MOV_S: "ABAJO", STRING_MOV_A: "IZQUIERDA", STRING_MOV_D: "DERECHA").
 * int i: tipo de dato entero que almacena el numero de fantasma que se va a encolar (puede ir desde cero, hasta estadisticas->fatasmas).
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void encolarMovimientosDeFantasmas(tCola * cola, int i, int movimiento);

/**
 * Descripcion: Muestra todos los movimientos que fue realizando el jugador a lo lago de la parida.
 *
 * Parametros:
 * tCola * cola: Puntero del tipo tCola donde se encuentran encolados los movimientos que fue realizando el jugador a lo largo de la parida.
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void mostrarMovimientosEncoladosDeJugador(tCola * cola);

/**
 * Descripcion: Muestra todos los movimientos que fue realizando la maquina (los de cada fantasma vivo) a lo largo de la parida.
 *
 * Parametros:
 * tCola * cola: Puntero del tipo tCola donde se encuentran encolados los movimientos que fue realizando la maquina (los de cada
 * fantasma vivo) a lo largo de la parida
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void mostrarMovimientosEncoladosDeFantasmas(tCola * cola);


#endif // MOVIMIENTOS_H_INCLUDED
