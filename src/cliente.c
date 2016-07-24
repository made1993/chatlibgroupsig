#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include "../include/conexion.h"
 

int main(int argc , char *argv[]){
    char* msg= malloc(8096);
    int sockfd;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    /*Comenzamos la conexion TCP*/
    printf("se obtiene informacion\n");
    if(0!=getaddrinfo("192.168.1.42", "8080", &hints, &res)){
        return 0;
    }
    printf("socket\n");
    sockfd=abrirSocketTCP();
    if(sockfd==-1){
        return 0;
    }
    printf("connect\n");
    if(-1==abrirConnect(sockfd, *(res->ai_addr))){
        
        return 0;
    }

    scanf("%s", msg);
    escribir(sockfd, msg);

    return 0;


    
}

    