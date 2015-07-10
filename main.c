/* CHAUDHRY Hussam
   4A AL2 */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

struct client {
    int id;
    int etat;
};

void print_clients(struct client clients[], int size);

int main(int argc, char *argv[]){

    int udpSocket, nBytes;
    char buffer[1024];
    struct sockaddr_in serverAddr, clientAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size, client_addr_size;

    int nb_clients = 0;
    struct client clients[255] = {{0, 0}};

    int trace = 0;
    char *s;

    // Récupération du paramètre de la trace
    if ( argc == 2 ) {
        // char* -> int
        trace = atoi(argv[1]);
    }


    /* Création socket udp */
    udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

    /* Configuration de la struct pour l'adresse */
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891); // Port
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*Bind du socket avec la struct de l'adresse */
    bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

     /* Initialisation de la variable de taille */
    addr_size = sizeof serverStorage;

    /* Boucle pour l'attente d'un message */
    while(1) {
        /* Réception d'un message */
        nBytes = recvfrom(udpSocket, buffer, 1024, 0, (struct sockaddr *)&serverStorage, &addr_size);

        s = strstr(buffer, "deco");
        if (s != NULL) {
            char *received;
            received = strtok(buffer, "d");

            int client_gone = atoi(received);
            clients[client_gone].id = 0;
            clients[client_gone].etat = 0;
        }

        s = strstr(buffer, "Connect");
        if (s != NULL) {
            // Nouveau client
            nb_clients = nb_clients + 1;
            // Ajout nouveau client dans le tableau des clients
            clients[nb_clients].id = nb_clients;
            clients[nb_clients].etat = 1;

            printf("Client numéro %d, état %d\n", clients[nb_clients].id, clients[nb_clients].etat);

            snprintf(buffer, sizeof(buffer), "%d", clients[nb_clients].id);
        } else {
            // Changement d'état
            // Char -> int
            char etat_c = buffer[strlen(buffer)-1];
            printf("Buffer %s\n", buffer);
            int etat = etat_c - '0';

            char client_id_c = buffer[strlen(buffer)-1] = '\0';
            int client_id = client_id_c - '0';

            clients[client_id].etat = etat;

            snprintf(buffer, sizeof(buffer), "%d", clients[client_id].etat);
        }

        /* Renvoi du message*/
        sendto(udpSocket, buffer, nBytes, 0, (struct sockaddr *)&serverStorage, addr_size);

        if (trace == 1) {
            print_clients(clients, 255);
        }
    }

    return 0;
}

void print_clients(struct client clients[], int size) {
    int i;
    printf("id : etat\n");
    for (i = 1; i < size; i++)
    {
        if (clients[i].id != 0) {
            printf("%d : %d\n", clients[i].id, clients[i].etat);
        }
    }
    printf("------\n");
}
