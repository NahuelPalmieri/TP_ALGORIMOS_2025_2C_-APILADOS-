#include "Pacman.h"
#define CONFIGURACIONES "configuracion.txt"

#define TAM_NOMBRE 11
#define CANT_RANK 5
typedef struct {
    int id;
    char nombre[TAM_NOMBRE];
    int puntaje;
} tMostrarRank;

int cargarDatos(tMostrarRank * vec, char * cad);




int main(int argc, char* argv[]) {

    //char* archivoConfig = argv[1]; --> por esto no funcionaba.
    char* archivoConfig = CONFIGURACIONES;
    char linea[64];
    int opcion, cond, i;
    char cadRank[]="RANKING";
    tUsuario usuarioActual;
    tMostrarRank auxMostrarRank[CANT_RANK];
    char * cadena = malloc(sizeof(tUsuario));
    ///
     if (init_winsock() != 0) {
        printf("Error al inicializar Winsock\n");
        return 1;
    }

    SOCKET sock = connect_to_server(SERVER_IP, PORT);
    if (sock == INVALID_SOCKET) {
        printf("No se pudo conectar al servidor\nJuego Offline:\n");
///OFFLINE
        jugarPartida(archivoConfig);
        printf("Desea jugar otra partida OFFLINE(1 - si, 0 - no)? :");
        scanf("%d",&opcion);
        while(opcion == 1)
        {
            getchar();
            jugarPartida(archivoConfig);
            printf("Desea jugar otra partida OFFLINE(1 - SI, 0 - NO)? :");
            scanf("%d",&opcion);
        }
        WSACleanup();
        return 0;
    }

    printf("Conectado al servidorA.\n");

    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    do {
        mostrarMenu();
        printf("Opcion: ");
        // leer toda la línea (por si el usuario mete letras o basura)
        if(!fgets(linea, sizeof(linea), stdin)) {
            printf("Error de entrada.\n");
            return 1;
        }

        // convertir texto a número
        opcion = (int)strtol(linea, NULL, 10);
        switch (opcion) {
            case 1:
///JUEGO
                usuarioActual = jugarPartida(archivoConfig);
                sprintf(cadena,"%d|%d|%s",usuarioActual.puntuacion,usuarioActual.cantidadMovimientos,usuarioActual.nombre);

                cond=1;
                while (cond==1) {
                        cond=0;
                    printf("> ");
                        if (strncpy(buffer,cadena,BUFFER_SIZE) == NULL) break;

                        buffer[strcspn(buffer, "\n")] = '\0'; // quitar salto de línea
                        if (strlen(buffer) == 0) continue;

                        if (send_request(sock, buffer, response) == 0) {
                        printf("Respuesta: %s\n", response);
                        } else {
                                printf("Error al enviar o recibir datos\n");
                                break;
                                }
                }
                break;
            case 2:
                strcpy(cadena,cadRank);
///
                cond=1;
                while (cond==1)
                {
                    cond=0;
                    printf("> ");
                    if (strncpy(buffer,cadena,BUFFER_SIZE) == NULL)
                        break;

                    buffer[strcspn(buffer, "\n")] = '\0'; // quitar salto de línea
                    if (strlen(buffer) == 0)
                        continue;

                    if (send_request(sock, buffer, response) == 0)
                    {
                        printf("Mostrando Ranking:\n");
                        printf("ID:| Nombre:     |Puntaje:\n");
                        cargarDatos(auxMostrarRank,response);
                        for(i = 0; i < CANT_RANK; i++)
                        {
                            printf("%-2d | %-11s | %d\n",auxMostrarRank[i].id,auxMostrarRank[i].nombre,auxMostrarRank[i].puntaje);
                        }
                    }
                    else
                    {
                        printf("Error al enviar o recibir datos\n");
                        break;
                    }
                }
                break;
            case 3:
                printf("Saliendo del juego...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    }while(opcion != 3);
///
    free(cadena);
    close_connection(sock);
///
    return 0;
}

int cargarDatos(tMostrarRank * vec, char * cad)
{
    int i, cont = 0;
    char aux[CANT_RANK][25];
    sscanf(cad,"%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|",aux[0],aux[1],aux[2],aux[3],aux[4]);

    for(i = 0; i < CANT_RANK; i++)
    {
        if(strlen(aux[i])>5)
        {
            sscanf(aux[i],"%d;%[^;];%d",&vec[i].id,vec[i].nombre,&vec[i].puntaje);
            cont++;
        } else
        {
            vec[i].id = -1;
            strcpy(vec[i].nombre,"N/A");
            vec[i].puntaje = 0;
        }
    }
    return cont;
}
