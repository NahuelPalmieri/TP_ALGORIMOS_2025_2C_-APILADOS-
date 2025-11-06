#include "servidor_lib.h"

int init_winsock()
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET create_server_socket()
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) return INVALID_SOCKET;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        closesocket(s);
        return INVALID_SOCKET;
    }

    if (listen(s, 1) == SOCKET_ERROR)
    {
        closesocket(s);
        return INVALID_SOCKET;
    }

    return s;
}

void process_request(const char *request, char *response,tArbol * arbol)
{
    //char operation[16], text[BUFFER_SIZE];
    char * cad = malloc(1024);
    char respuesta[]="\nLo abri bien\n";
    char respuestaMAL[]="\nLo abri MAL\n";
    int rtdo;
    char text[BUFFER_SIZE];
    sscanf(request,"%[^\n]", text);
    if(strcmp(text,"RANKING") != 0)
    {
        rtdo = manejarArchUsuarios(NOMB_ARCH_USU,NOMB_ARCH_PARTIDAS,NOMB_ARCH_IND,text,arbol);
        if(rtdo == 1)
        {
            printf("\n\nTU ARCHIVO USUARIO:\n");
            leerArchBinario(NOMB_ARCH_USU);
            printf("\n\nTU ARCHIVO PARTIDAS:\n");
            leerArchBinarioPartida(NOMB_ARCH_PARTIDAS);
            printf("\n\nTU ARCHIVO INDICE:\n");
            leerArchBinarioIndice(NOMB_ARCH_IND);
            printf("\narbolBBB\n");
            arbolInOrden(arbol);
            strcpy(response,respuesta);
        }
        else
        {
            strcpy(response,respuestaMAL);
        }
    }
    else
    {
        //RANKING
         if(!cad)
         {
            return;
         }
            strcpy(response,procesarRanking(NOMB_ARCH_USU, cad));
         if(!response)
         {
             strcpy(response,respuestaMAL);
         }
         free(cad);
    }
}

void run_server()
{
    tArbol ArbolIndice;
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int bytes_received;

    struct sockaddr_in client_addr;
    int client_addr_size;
    int err;
    char peticion[BUFFER_SIZE];

    tCola cola;

    if (init_winsock() != 0)
    {
        printf("Error al inicializar Winsock\n");
        return;
    }

    SOCKET server_socket = create_server_socket();
    if (server_socket == INVALID_SOCKET)
    {
        printf("Error al crear socket del servidor\n");
        WSACleanup();
        return;
    }

    printf("Servidor escuchando en puertoA %d...\n", PORT);
    client_addr_size = sizeof(client_addr);

    SOCKET client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == INVALID_SOCKET)
    {
        printf("Error en accept()\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    printf("Cliente conectadoA.\n");
    crearCola(&cola);

    // Bucle principal
    while (1)
    {
        // Intentar recibir un mensaje
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';
            printf("[RX] Recibido: %s\n", buffer);

            // Encolamos el mensaje recibido
            ponerEnCola(&cola, buffer, BUFFER_SIZE);
        }
        else if (bytes_received == 0)
        {
            // El cliente ha cerrado la conexión de forma ordenada
            printf("Cliente desconectado (recv == 0). Saliendo del bucle.\n");
            break;
        }
        else /* bytes_received == SOCKET_ERROR */
        {
            err = WSAGetLastError();        // Ver que es esto
            if (err == WSAEWOULDBLOCK)
            {
                // No hay datos en modo no-bloqueante.
                // Simplemente seguimos el bucle y procesamos la cola.
            }
            else
            {
                // Error serio, salimos
                printf("Error en recv(): %d. Saliendo.\n", err);
                break;
            }
        }

        // Procesar mensajes pendientes en la cola
        while (!colaVacia(&cola))
        {
            // Desencolar la peticion
            sacarDeCola(&cola, peticion, BUFFER_SIZE);

            // Procesar la peticion
            printf("[PROC] Procesando: %s\n", peticion);
            process_request(peticion, response, &ArbolIndice);

            // Enviar la respuesta al cliente (si es necesario)
            send(client_socket, response, strlen(response), 0);
            printf("[TX] Enviado: %s\n", response);
        }

        // Evita usar de manera intenciva la CPU
        Sleep(100);
    }

    vaciarCola(&cola);
    printf("Conexion cerrada.\n");
    closesocket(client_socket);
    closesocket(server_socket);
    destruirArbol(&ArbolIndice);
    WSACleanup();
}
