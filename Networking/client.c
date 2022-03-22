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

int main(int argc, char *argv[])
{
    int errno;
    int PORT;                  // portul de conectare la server
    int sd;                    // descriptorul de socket
    struct sockaddr_in client; // structura folosita pentru conectare
    char msg[100];             // mesajul trimis
    int oldCell, newCell;

    /* exista toate argumentele in linia de comanda? */
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


    //aici trebuie pus startupScreen(); si sa continuie codul dupa ce se apeleaza find match




    /* ne conectam la server */
    if (connect(sd, (struct sockaddr *)&client, sizeof(struct sockaddr)) == -1)
    {
        perror("[client]Eroare la connect().\n");
        return errno;
    }


    if (read(sd, msg, 100) < 0)
    {
        perror("[client]Eroare la read() de la server.\n");
        return errno;
    }
    /* afisam mesajul primit */
    printf("[client]Acesta este jucatorul numarul %s\n", msg);

    while(1){

        if(strcmp( msg, "1" ) == 0){ //daca acesta e primul jucator

            printf("[client] Introduceti vechiul cell: ");
            scanf ("%d", &oldCell);
            printf("[client] Introduceti noul cell: ");
            scanf ("%d", &newCell);
            if (write(sd, &oldCell, sizeof(oldCell)) <= 0)
    	    {	
        	perror("[client]Eroare la write() spre server.\n");
        	return errno;
    	    }
            if (write(sd, &newCell, sizeof(newCell)) <= 0)
    	    {	
        	perror("[client]Eroare la write() spre server.\n");
        	return errno;
    	    }
            if(oldCell == 65 || newCell == 65){
                break;
            }
            //update the table



            if (read(sd, &oldCell, sizeof(oldCell)) < 0)
            {
            perror("[client]Eroare la read() de la server.\n");
            return errno;
            }
            if (read(sd, &newCell, sizeof(newCell)) < 0)
            {
            perror("[client]Eroare la read() de la server.\n");
            return errno;
            }
            printf("[client] Oponentul a mutat piesa de pe %d pe %d \n", oldCell, newCell);
            if(oldCell == 65 || newCell == 65){
                break;
            }
            //update the table

        } else if (strcmp( msg, "2" ) == 0){

            if (read(sd, &oldCell, sizeof(oldCell)) < 0)
            {
            perror("[client]Eroare la read() de la server.\n");
            return errno;
            }
            if (read(sd, &newCell, sizeof(newCell)) < 0)
            {
            perror("[client]Eroare la read() de la server.\n");
            return errno;
            }
            if(oldCell == 65 || newCell == 65){
                break;
            }
            printf("[client] Oponentul a mutat piesa de pe %d pe %d \n", oldCell, newCell);
            //update the table



            printf("[client] Introduceti vechiul cell: ");
            scanf ("%d", &oldCell);
            printf("[client] Introduceti noul cell: ");
            scanf ("%d", &newCell);
            if(newCell == 65){
                break;
            }
            if (write(sd, &oldCell, sizeof(oldCell)) <= 0)
    	    {	
        	perror("[client]Eroare la write() spre server.\n");
        	return errno;
    	    }
            if (write(sd, &newCell, sizeof(newCell)) <= 0)
    	    {	
        	perror("[client]Eroare la write() spre server.\n");
        	return errno;
    	    }
            if(oldCell == 65 || newCell == 65){
                break;
            }
            //update the table

        }

    }
    close(sd);
}
