#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MENSAJE_DE_OPCIONES "Elija una opcion:\n\n"                              \
                            "r. Ver Ranking DE Jugadores\n"                      \
                            "j. Jugar Una Nueva Partida\n"                       \
                            "f. Salir Del Juego\n"                               \
                            "--->"

#define CADENA_DE_OPCIONES "rjfRJF"

/**
 * Descripcion: Muestra el menu conformado por (mensajeDeOpciones) y le solicita al usuario (en este
 * caso el jugador), que inserte la opcion que desea realizar. Hasta que la opcion ingresada por el usuario
 * no este contemplada en el string (opcionesValidas) recibido como parametro, la funcion le va a seguir
 * solicitando al jugador que ingrese una opcion que este incluida en el menu que se muestra en pantalla.
 *
 * Parametros:
 * const char * mensadeDeOpciones: puntero a cadena de tipo const (ya que no se va a modificar a lo largo de
 * la ejecucion de la funcion), con el texto a mostrar como menu. En el caso de este juego, lo enviamos como
 * parametro a traves de la utilizacion de una macro (MENSAJE_DE_OPCIONES).
 * const char * opcionesValidas: puntero a cadena de tipo const (ya que no se va a modificar a lo largo de
 * la ejecucion de la funcion), con el texto de las opciones validas a verificar en la funcion. En el caso
 * de este juego, lo enviamos como parametro a traves de la utilizacion de una macro (CADENA_DE_OPCIONES).
 * Este parametro se utilizara para validar el corte del ciclo do - while de la funcion (el que determina
 * hasta cuando voy a pedirle al usuario que ingrese una opcion valida).
 *
 * Valor De Retorno: no tiene, es una funcion de tipo void.
 */
char mostrarMenu(const char * mensajeDeOpciones, const char * opcionesValidas);

#endif // MENU_H_INCLUDED
