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
	char * buff = NULL;
	int bufflen;
	socket = (int*) args;
	Usuario_t* usr = NULL;
	usr = crearUsuario(socket);
	while(1){

		if(recibir(*socket, &buff) == -1){
			close(*socket);
			return NULL;
		}
		printf("%s\n", buff);
		if((bufflen = strlen(buff)) < 1){
			close(*socket);
			return NULL;
		}

		if(comando(buff)== NICK){
			recvNick(usr, buff);
			printf("Se registro %s\n", usr->nick);
			escribir(*usr->socket, "/MSG server bienvenido", strlen((char*)"/MSG server bienvenido")+1);
			break;
		}

	}
	insert_list(listaUsuarios, usr);
	return usr;
}
void* lecturaUsuario(void* args){
	char * buff = NULL;
	int bufflen, end = 0;
	Usuario_t* usr = verificarCliente(args);
	while(!end){
		if((bufflen = recibir(*usr->socket, &buff)) < 1){
			printf( "%d\n", delete_elem_list(listaUsuarios, (void*) usr));
			printf("cosas extrañas pueden pasar\n");
			break;
		}
		fwrite(buff, 1, bufflen, stdout);
		printf("bufflen: [%d]\n", bufflen);
		switch (comando(buff)){
			case NICK:
				printf("NICK\n");
				recvNick(usr, buff);
			break;

			case MSG:
				printf("MSG\n");
				recvMsg(buff, bufflen);
			break;

			case DISCONNECT:
				printf("DISCONNECT\n");
				recvDisconnect(usr);
				end = 1;
			break;

			case PING:
				printf("PING\n");
				recvPing(usr);
			break;

			case PONG:			
				printf("PONG\n");
				recvPong(usr);
			break;

			default:
				printf("default\n");

			break;



		}
		free(buff);
	}
	printf("Usuario desconectado\n");
	return NULL;
}


int main(){
	int socket;
	int* socketcli;
	int usuarios = 0;
	
	struct sockaddr_in ip4addr;

	socket = abrirSocketTCP();
	if(socket < 1)
		return 0;

	if(abrirBind(socket, 8080) < 0)
		return 0;

	if(abrirListen(socket) == -1)
		return 0;

	listaUsuarios = create_list(compareUsr);

	while(1){
		printf("nuevo cliente\n");
		socketcli= malloc(sizeof(int));
		*socketcli=aceptar(socket, ip4addr);
		usuarios++;
		hilos= realloc(hilos, sizeof(pthread_t)*usuarios);

		pthread_create(&hilos[usuarios-1],NULL, lecturaUsuario, (void*) socketcli );
		
	}
	printf("fin\n");
	return 0;
}

