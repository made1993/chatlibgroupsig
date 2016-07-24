#include <stdio.h>
#include <string.h>	//strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>	//write
#include "../include/conexion.h"
 

int main(){
	int socket;
	socket = abrirSocketTCP();

	abrirBind(socket, 8080);

	abrirListen(socket);
}

