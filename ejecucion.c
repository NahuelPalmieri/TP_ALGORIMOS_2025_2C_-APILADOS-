#include "ejecucion.h"

void ejecutarLaberintosYFantasmas()
{
    tTablero tablero;
    tEstadisticas estadisticas;
    char opcionElegida;
    tCola colaMovJugador;
    tCola colaMovMaquina;

    crearCola(&colaMovJugador);
    crearCola(&colaMovMaquina);

    do
    {
        opcionElegida = mostrarMenu(MENSAJE_DE_OPCIONES,CADENA_DE_OPCIONES);
        opcionElegida = toupper(opcionElegida);

        switch(opcionElegida)
        {
            case 'R':
                //ESTO NO VA A ACA//
                //ESTO NO VA A ACA//
                //ESTO NO VA A ACA//
                mostrarMovimientosEncoladosDeJugador(&colaMovJugador);
                mostrarMovimientosEncoladosDeFantasmas(&colaMovMaquina);
                //ESTO NO VA A ACA//
                //ESTO NO VA A ACA//
                //ESTO NO VA A ACA//
                printf("Mostrando Ranking De Jugadores!!!");
                break;
            case 'J':
                prepararElEntorno(ARCHIVO_DE_TEXTO_CONFIGURACION,&tablero,&estadisticas);
                creadorMapaDeJuegoAleatorio(&tablero,&estadisticas);

                mostrarMapa(&tablero);
                mostrarEstadisticas(&estadisticas);

                moversePorElMapa(&tablero,&estadisticas,&colaMovJugador,&colaMovMaquina);
                break;
            case 'F':
                printf("Saliendo del juego...");
                destruirMatrizEntera(tablero.posFantasmas.posItems,tablero.posFantasmas.cantItems);
                destruirMatrizEntera(tablero.posPremios.posItems,tablero.posPremios.cantItems);
                destruirMatrizEntera(tablero.posVidasExtra.posItems,tablero.posVidasExtra.cantItems);
                destruirMatrizDeCaracteres(tablero.mapa,tablero.cantidadDeFilas);
                break;
        }
    } while(opcionElegida != 'F');
}
