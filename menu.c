#include "menu.h"

char mostrarMenu(const char * mensajeDeOpciones, const char * opcionesValidas)
{
    char opcionElegida;
    int primerMuestra = 1;

    do
    {
        printf("%s%s",
               primerMuestra ? primerMuestra=0, "": "\n\nOpcion no valida, Vuelve a intentarlo:\n",
               mensajeDeOpciones);
        fflush(stdin);
        scanf("%c",&opcionElegida);
    } while(strchr(opcionesValidas,opcionElegida)==NULL);

    return opcionElegida;
}
