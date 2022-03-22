#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

struct clients
{
    int connectedClients;
    int client;
    int client2;
} clients;

/* codul de eroare returnat de anumite apeluri */

void recover_kids()
{ //recovers zombie processes
    while (waitpid(-1, NULL, WNOHANG) != 0)
        ;
}

int main()
{
    int errno;                 //va returna numarul erorii
    const int PORT = 1818;     //declaram de tip const pentru ca valoarea portului nu se schimba pe parcurs
    struct sockaddr_in server; // structura folosita de server
    struct sockaddr_in from;
    char msg[100];          //mesajul primit de la client
    char msgrasp[100] = ""; //mesaj de raspuns pentru client
    int sd;                 //descriptorul de socket
    int oldCell, newCell;

    /* crearea unui socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[server]Eroare la socket().\n");
        return errno;
    }

    /* pregatirea structurilor de date */
    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
    /* acceptam orice adresa */
    //host to network long
    server.sin_addr.s_addr = htonl(INADDR_ANY); //se accepta requesturi de la orice interfata
    /* utilizam un port utilizator */
    //host to network short
    server.sin_port = htons(PORT);

    /* atasam socketul */
    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server]Eroare la bind().\n");
        return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen(sd, 5) == -1)
    {
        perror("[server]Eroare la listen().\n");
        return errno;
    }

    if (signal(SIGCHLD, recover_kids) == SIG_ERR)
    {
        perror("[server]Eroare la atasarea semnalului.\n");
        return errno;
    }
    // int connectedClients = 0;
    /* servim in mod iterativ clientii... */
    clients.connectedClients = 0;
    while (1)
    {
        printf("waiting for connections \n");
        // int client;
        int length = sizeof(from);
        printf("[server]Asteptam la portul %d un client \n", PORT);
        fflush(stdout);
        /* acceptam un client (stare blocanta pana la realizarea conexiunii) */
        if (clients.connectedClients == 0)
        {
            clients.client = accept(sd, (struct sockaddr *)&from, &length);
            if (clients.client < 0)
            {
                perror("[server]Eroare la accept().\n");
                continue;
            }
            printf("[server] S-a conectat primul client \n");
            clients.connectedClients++;
            printf("acum dupa conectare avem %d clienti conectati\n", clients.connectedClients);
        }
        else if (clients.connectedClients == 1)
        {
            clients.client2 = accept(sd, (struct sockaddr *)&from, &length);
            if (clients.client2 < 0)
            {
                perror("[server]Eroare la accept().\n");
                continue;
            }
            printf("[server] S-a conectat al doilea client \n");
            clients.connectedClients++;
            printf("acum dupa conectare avem %d clienti conectati\n", clients.connectedClients);
        }

        /* eroare la acceptarea conexiunii de la un client */

        // //asteptam al doilea client acum
        // int client2;
        // int length2 = sizeof(from);
        // printf("[server]Asteptam la portul %d al doilea client \n", PORT);
        // fflush(stdout);
        // /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
        // client2 = accept(sd, (struct sockaddr *)&from, &length);
        // /* eroare la acceptarea conexiunii de la un client */
        // if (client2 < 0)
        // {
        //     perror("[server]Eroare la accept().\n");
        //     continue;
        // }
        // printf("[server] S-a conectat al doilea client \n");
        // connectedClients++;

        if (clients.connectedClients == 2)
        {
            clients.connectedClients = 0;
            printf("[server] Doi clienti s-au conectat pentru a juca \n");
            printf("[server] Vom face un fork pentru a coordona jocul celor doi \n");

            int f = fork();
            if (f == -1)
            {
                perror("[server]Eroare la crearea fork-ului().\n");
                close(clients.client); /* inchidem conexiunea cu clientul */
                close(clients.client2);
                continue; /* continuam sa ascultam */
            }
            else if (f == 0) //suntem in copil
            {
                printf("suntem in copil ;) \n");
                //sending the players the order number
                strcat(msgrasp, "1");
                if (write(clients.client, msgrasp, 100) <= 0)
                {
                    perror("[server]Eroare la write() catre clientul 1 inainte de fork.\n");
                    continue; /* continuam sa ascultam */
                }
                memset(msgrasp, 0, strlen(msgrasp));
                strcat(msgrasp, "2");
                if (write(clients.client2, msgrasp, 100) <= 0)
                {
                    perror("[server]Eroare la write() catre clientul 2 inainte de fork.\n");
                    continue; /* continuam sa ascultam */
                }
                memset(msgrasp, 0, strlen(msgrasp));
                int ok = 1;
                while (1)
                {

                    printf("[server]Asteptam mutarea primului client...\n");
                    /* citirea fostei pozitii a piesei */
                    if (read(clients.client, &oldCell, sizeof(oldCell)) <= 0)
                    {
                        perror("[server]Eroare la read() de la client.\n");
                        continue; /* continuam sa ascultam */
                    }

                    /* citirea noii pozitii a piesei */
                    if (read(clients.client, &newCell, sizeof(newCell)) <= 0)
                    {
                        perror("[server]Eroare la read() de la client.\n");
                        continue; /* continuam sa ascultam */
                    }

                    printf("[server]Mutarea primului client a fost receptionata...\n");
                    printf("[server]Piesa cu de pe patratica %d va fi mutata pe %d.\n", oldCell, newCell);

                    //trimitem raspunsul de la clientul 1 clientului 2
                    if (write(clients.client2, &oldCell, sizeof(oldCell)) <= 0)
                    {
                        perror("[server]Eroare la write() catre client.\n");
                        continue; /* continuam sa ascultam */
                    }

                    if (write(clients.client2, &newCell, sizeof(newCell)) <= 0)
                    {
                        perror("[server]Eroare la write() catre client.\n");
                        continue; /* continuam sa ascultam */
                    }

                    if (oldCell == 65 || newCell == 65)
                    { //in caz ca celalalt player a dat quit
                        close(clients.client);
                        close(clients.client2);
                        printf("acum executam exit-ul \n");
                        exit(0);
                        // break;
                    }

                    printf("[server] Trimitem celui de-al doilea jucator mutarea primului. \n");
                    printf("[server] Asteptam raspunsul jucatorului 2. \n");

                    //asteptam in stare blocanta mutarea celui de-al doilea client
                    /* citirea fostei pozitii a piesei */
                    if (read(clients.client2, &oldCell, sizeof(oldCell)) <= 0)
                    {
                        perror("[server]Eroare la read() de la client.\n");
                        continue; /* continuam sa ascultam */
                    }

                    /* citirea noii pozitii a piesei */
                    if (read(clients.client2, &newCell, sizeof(newCell)) <= 0)
                    {
                        perror("[server]Eroare la read() de la client.\n");
                        continue; /* continuam sa ascultam */
                    }

                    printf("[server] Mutarea celui de-al doilea client a fost receptionata...\n");
                    printf("[server] Piesa cu de pe patratica %d va fi mutata pe %d.\n", oldCell, newCell);

                    //trimitem raspunsul de la clientul 1 clientului 2
                    if (write(clients.client, &oldCell, sizeof(oldCell)) <= 0)
                    {
                        perror("[server]Eroare la write() catre client.\n");
                        continue; /* continuam sa ascultam */
                    }

                    if (write(clients.client, &newCell, sizeof(newCell)) <= 0)
                    {
                        perror("[server]Eroare la write() catre client.\n");
                        continue; /* continuam sa ascultam */
                    }

                    if (oldCell == 65 || newCell == 65)
                    {
                        close(clients.client);
                        close(clients.client2);
                        printf("acum executam exit-ul \n");
                        exit(0);
                        // break;
                    }

                    printf("[server] Trimitem primului jucator mutarea celui de-al doilea. \n");
                }
                // close(clients.client);
                // close(clients.client2);
                printf("acum executam exit-ul din nou??? \n");
                exit(0);
            }
            else
            {
                clients.connectedClients = 0;
            }
            // close(client);
            // close(client2);
            // connectedClients = 0;
            printf("am iesit din iful ala mare iar connected clients este %d\n", clients.connectedClients);
        }
        printf("am iesit din if clients == 2 \n");
        // clients.connectedClients = 0;
        // close(client);
        // close(client2);
    } /* while */
} /* main */
