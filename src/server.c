#include <stdio.h>
#include <string.h>	//strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>	//write
#include <pthread.h>
#include "../include/conexion.h"
 
pthread_t* hilos;

void* lecturaUsuario(void* args){
	int* socket = NULL;
	char * buff = malloc(8096);
	socket = (int*) args;
	while(1){

		if(recibir(*socket, buff) == -1)
			break;
		if(strlen(buff) == 0)
			break;
		printf("[%s]\n", buff);
	}
	printf("Usuario desconectado\n");
	free(socket);
	free(buff);
	return NULL;
}

int main(){
	int socket;
	int* socketcli;
	int usuarios = 0;
	
	struct sockaddr_in ip4addr;

	printf("abrirSocketTCP\n");
	socket = abrirSocketTCP();

	printf("bind\n");
	abrirBind(socket, 8080);

	printf("Listen\n");
	abrirListen(socket);


	while(1){
		socketcli= malloc(sizeof(int));
		*socketcli=aceptar(socket, ip4addr);
		usuarios++;
		hilos= realloc(hilos, sizeof(pthread_t)*usuarios);

		pthread_create(&hilos[usuarios-1],NULL, lecturaUsuario, (void*) socketcli );
		
	}
	return 0;
}

