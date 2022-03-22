#include "start.h"
#include "main.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

int errno;
int PORT;                  // portul de conectare la server
int sd;                    // descriptorul de socket
struct sockaddr_in client; // structura folosita pentru conectare
char msg[100];             // mesajul trimis
int oldCell, newCell;
int playerNumber;
int myTurn;
int auxiliarPiece;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Sintaxa: %s <adresa_server> <PORT>\n", argv[0]);
        return -1;
    }

    /* stabilim PORTul */
    PORT = atoi(argv[2]);

    /* cream socketul */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Eroare la socket().\n");
        return errno;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    client.sin_family = AF_INET;
    /* adresa IP a serverului */
    client.sin_addr.s_addr = inet_addr(argv[1]);
    /* PORTul de conectare */
    client.sin_port = htons(PORT);

    startupScreen();
    return 0;
}