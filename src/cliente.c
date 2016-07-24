#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include "../include/conexion.h"
 
int main(int argc , char *argv[]){
    int socket =-1;
    struct addrinfo hints, *res;
    socket = abrirScoketTCP();

    if(0!=getaddrinfo("192.168.1.42", 8080, &hints, &res)){
        error_text(current_page(),"Error al obtener informacion del servidor");
        return;
    }

    if(-1==abrirConnect(sockfd, *(res->ai_addr))){
        error_text(current_page(),"Error al conectar"); 
        return;
    }

}
