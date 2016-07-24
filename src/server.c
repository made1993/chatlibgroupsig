#include <stdio.h>
#include <string.h>	//strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>	//write
#include "../include/conexion.h"
 

int main(){
	int socket;
	int socketcli;
	char * buff = malloc(8096);
	struct sockaddr_in ip4addr;

	printf("abrirSocketTCP\n");
	socket = abrirSocketTCP();

	printf("bind\n");
	abrirBind(socket, 8080);

	printf("Listen\n");
	abrirListen(socket);



	socketcli=aceptar(socket, ip4addr);
	recibir(socketcli, buff);
	printf("%s\n", buff);
}

