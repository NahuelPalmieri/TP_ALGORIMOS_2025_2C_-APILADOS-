#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

#include "estructurasYMacros.h"

/**
 * Descripcion: Muestra en pantalla el mapa actual del tablero.
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego.
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void mostrarMapa(tTablero * tablero);

/**
 * Descripcion: Crea un mapa de juego aleatorio con al menos un camino que comience desde la
 * entrada y termine en la salida, e inserta todos los items descritos en la estructura tablero.
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego.
 * tEstadisticas * estadisticas: Puntero a la estructura de estadisticas para registrar parámetros iniciales.
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void creadorMapaDeJuegoAleatorio(tTablero * tablero, tEstadisticas * estadisticas);

/**
 * Descripcion: Prepara el entorno de juego a partir de un archivo de configuración.
 *
 * Parametros:
 * char * nombreDeArchivo: string contenedor del nombre del archivo de texto del cual vamos a extraer la
 * informacion para preparar el entorno
 * tTablero * tablero: Puntero al tablero del juego.
 * tEstadisticas * estadisticas: Puntero a la estructura de estadisticas para registrar parámetros iniciales.
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void prepararElEntorno(char * nombreDeArchivo, tTablero * tablero, tEstadisticas * estadisticas);

/**
 * Descripcion: Extrae datos de un archivo de texto y los carga en las estructuras tablero y estadisticas.
 *
 * Parametros:
 * char * nombreDeArchivo: string contenedor del nombre del archivo de texto del cual vamos a extraer la
 * informacion para preparar el entorno
 * tTablero * tablero: Puntero al tablero del juego.
 * tEstadisticas * estadisticas: Puntero a la estructura de estadisticas para registrar parámetros iniciales.
 *
 * Valor De Retorno: de tipo int, el valor de retorno indica si se leyeron o no correctamente los registros del archivo.
 */
int extraerDatosDeArchivoDeTexto(char * nombreDeArchivo, tTablero * tablero, tEstadisticas * estadisticas);

/**
 * Descripcion: Establece los primeros parámetros de las estructuras tablero y estadisticas.
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego.
 * tEstadisticas * estadisticas: Puntero a la estructura de estadisticas para registrar parámetros iniciales.
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void establecerPrimerosParametros(tTablero * tablero, tEstadisticas * estadisticas);

/**
 * Descripcion: Crea una matriz de caracteres dinamica.
 *
 * Parametros:
 * unsigned cantFilas: Cantidad de filas de la matriz dinamica.
 * unsigned cantColumnas: Cantidad de columnas de la matriz dinamica.
 *
 * Valor De Retorno: del tipo char **, el valor de retorno devuelve un puntero a un puntero de char, es
 * decir, un puntero doble a una matriz del tipo char.
 */
char ** crearMatrizDinamicaDeCaracteres(unsigned cantFilas, unsigned cantColumnas);

/**
 * Descripcion: Crea una matriz de enteros dinamica.
 *
 * Parametros:
 * unsigned cantFilas: Cantidad de filas de la matriz dinamica.
 * unsigned cantColumnas: Cantidad de columnas de la matriz dinamica.
 *
 * Valor De Retorno: del tipo char **, el valor de retorno devuelve un puntero a un puntero de int, es
 * decir, un puntero doble a una matriz del tipo int.
 */
int ** crearMatrizDinamicaDeEnteros(unsigned cantFilas, unsigned cantColumnas);


/**
 * Descripcion: Genera un camino aleatorio dentro de las fronteras del mapa con al menos una solucion que
 * comience desde la posicion de la entrada en el tablero y termine en la posicion de la salida del tablero.
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego.
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void generadorDeCaminos(tTablero * tablero);

/**
 * Descripcion: Rellena los espacios libres dentro de las fronteras del mapa (es decir, que no sean caminos
 * ni fronteras) de manera aleatoria, de tal forma que esos espacios en primera instancia se ocupen con los items
 * especiales que el archivo de texto inicial trae, y que se encuentran en la variable del tipo tEstadisticas * estadisticas.
 * Una vez que se insertaron todos los items especiales, se recorre toda la parte interior del mapa (sin considerar las fronteras),
 * y en los casos en los que no haya un camino ni un item especial, se va a insertar o un nuevo camino ('.' - con una probabilidad
 * de 33.33% de insercion) o una pared ('#' - con una probabilidad de 66.66% de insercion).
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego donde encontraremos el mapa del juego a completar.
 * tEstadisticas * estadisticas: Puntero a la estructura de estadisticas donde se encuentra la cantidad de items especiales de
 * cada tipo a insertar en el mapa.
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void generadorDeItemsVarios(tTablero * tablero, tEstadisticas * estadisticas);

/**
 * Descripcion: Funcion utilizada por la funcion generadorDeItemsVarios(tTablero * tablero, tEstadisticas * estadisticas).
 * Rellena los espacios libres dentro de las fronteras del mapa (es decir, que no sean caminos ni fronteras) de manera aleatoria,
 * de tal forma que esos espacios en primera instancia se ocupen con los items especiales que la funcion le envie por parametros.
 * de 33.33% de insercion) o una pared ('#' - con una probabilidad de 66.66% de insercion).
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego donde encontraremos el mapa del juego a completar.
 * int maxCantItem: Indica la cantidad de items de (caracterAInsertar) tipo dentro del mapa proporcionado por (tablero).
 * char caracterAInsertar: Caracter a insertar en el mapa proporcionado por (tablero) ('F', 'V', 'P').
 * int ** posicionesAInsertar: Es un puntero a un puntero de int, es decir, un puntero doble a una matriz del tipo int.
 * En esta matriz se van a guardar las posiciones en el mapa de donde estan ubicados los items que la funcion inserta.
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void insertarNItemsDeXTipo(tTablero * tablero, int maxCantItem, char caracterAInsertar, int ** posicionesAInsertar);

/**
 * Descripcion: Inicializa todas las posiciones de la matriz de caracteres (el mapa proporcionado por (tablero)) con un valor 'Z'.
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego donde encontraremos el mapa del juego con todas las posiciones a inicializar.
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void settearMapaEnZ(tTablero * tablero);

/**
 * Descripcion: Inicializa todas las posiciones frontera (superior, inferior, izquierda y derecha) de la matriz de caracteres
 * (el mapa proporcionado por (tablero)) con un valor '#'. Luego inserta en una posicion dentro de estas fronteras (sin contar
 * ninguna de las 4 esquinas del mapa) un valor 'E', y a partir de ahí, del lado opuesto busco una posicion aleatoria para insertar
 * el valor 'S' (sin contarninguna de las 4 esquinas del mapa), para de esa forma establecer la entrada ('E') y la salida ('S') del
 * juego (por ejemplo: si 'E' esta arriba, 'S' tiene que estar abajo).
 *
 * Parametros:
 * tTablero * tablero: Puntero al tablero del juego donde encontraremos el mapa del juego con todas las posiciones a inicializar.
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void settearFronterasDelMapa(tTablero * tablero);

/**
 * Descripcion: Destruye la matriz entera dinamica, es decir, cada fila que la matriz tiene la libera,
 * y luego libera el primer malloc realizado (el que contiene los int * que almacenan las filas).
 *
 * Parametros:
 * int ** matriz: Conocido como puntero doble a enntero, puntero a puntero de entero, o como matriz entera.
 * Se envia como parametro el doble puntero de la matriz entera para liberar su memoria.
 * unsigned cantFilas: numero entero de tipo unsigned que determina la cantidad de filas que tiene la matriz
 * dinamica. Este numero se va a utilizar para determinar cuantos free() realizar.
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void destruirMatrizEntera(int ** matriz, unsigned cantFilas);

/**
 * Descripcion: Destruye la matriz de caracteres dinamica, es decir, cada fila que la matriz tiene la libera,
 * y luego libera el primer malloc realizado (el que contiene los char * que almacenan las filas).
 *
 * Parametros:
 * char ** matriz: Conocido como puntero doble a caracter, puntero a puntero de caracter, o como matriz de caracteres.
 * Se envia como parametro el doble puntero de la matriz de caracteres para liberar su memoria.
 * unsigned cantFilas: numero entero de tipo unsigned que determina la cantidad de filas que tiene la matriz
 * dinamica. Este numero se va a utilizar para determinar cuantos free() realizar.
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
void destruirMatrizDeCaracteres(char ** matriz, unsigned cantFilas);

#endif // MAPA_H_INCLUDED
