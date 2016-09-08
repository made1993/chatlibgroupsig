#include <stdio.h>
#include <string.h>	//strlen
#include <arpa/inet.h> //inet_addr
#include <unistd.h>	//write
#include <pthread.h>

#include "../include/comandoss.h"

pthread_t* hilos;

LinkedList* listaUsuarios;

void* verificarCliente(void* args){
	int* socket = NULL;
	char * buff = malloc(8096);
	socket = (int*) args;
	Usuario_t* usr = NULL;
	usr = crearUsuario(socket);
	while(1){

		if(recibir(*socket, buff) == -1){
			close(*socket);
			return NULL;
		}
		if(strlen(buff) == 0){
			close(*socket);
			return NULL;
		}

		if(comando(buff)== NICK){
			recvNick(usr, buff);
			printf("Se registro %s\n", usr->nick);
			escribir(*usr->socket, "/MSG server bienvenido");
			break;
		}

	}
	insert_list(listaUsuarios, usr);
	return usr;
}
void* lecturaUsuario(void* args){
	char * buff = malloc(8096);
	Usuario_t* usr = verificarCliente(args);
	while(1){

		if(recibir(*usr->socket, buff) == -1)
			break;
		if(strlen(buff) == 0)
			break;
		printf("%s\n", buff);
		switch (comando(buff)){
			case NICK:
				recvNick(usr, buff);
			break;

			case MSG:
				recvMsg(buff);
			break;

			case DISCONNECT:
				recvDisconnect(usr);
			break;

			case PING:
				recvPing(usr);
			break;

			case PONG:
				recvPong(usr);
			break;

			default:

			break;



		}
		printf("[%s]\n", buff);
	}
	printf("Usuario desconectado\n");
	delete_elem_list( listaUsuarios,(void*) usr);
	liberarUsuario(usr);
	free(buff);
	return NULL;
}


int main(){
	int socket;
	int* socketcli;
	int usuarios = 0;
	
	struct sockaddr_in ip4addr;

	socket = abrirSocketTCP();

	abrirBind(socket, 8080);

	abrirListen(socket);

	create_list(compareUsr);

	while(1){
		socketcli= malloc(sizeof(int));
		*socketcli=aceptar(socket, ip4addr);
		usuarios++;
		hilos= realloc(hilos, sizeof(pthread_t)*usuarios);

		pthread_create(&hilos[usuarios-1],NULL, lecturaUsuario, (void*) socketcli );
		
	}
	return 0;
}

