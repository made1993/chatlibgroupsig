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
#include <pthread.h>
#include <getopt.h>
#include "../include/comandosu.h"
#include "../include/clientui.h" 

int sockfd = -1;
pthread_t h1, h2;
char* nick;
void* hiloEscritura(void* args){
	char* msg = NULL;
	if(sockfd ==-1)
		return NULL;
	while (1){

		msg = scanMsg();
		printMsg(msg);
		switch(comando(msg)){
			case NICK:
				sendNick(socket);
			break;
			case MSG:
				sendMsg(msg);
			break;
			case DISCONNECT:
				sendDisconnect(sockfd);
			break;
			case PING:
				sendPing(sockfd);
			break;
			case PONG:
				sendPong();
			break;
		}
		free(msg);
	}
	free(msg);
	return NULL;
}


void* hiloLectura(void* args){
	char* msg= malloc(8096);
	if(sockfd ==-1)
		return NULL;
	while(1){
		recibir(sockfd, msg);
		switch(comando(msg)){
			case NICK:
				recvNick();
			break;
			case MSG:
				recvMsg(msg);
			break;
			case DISCONNECT:
				recvDisconnect(sockfd);
			break;
			case PING:
				recvPing(sockfd);
			break;
			case PONG:
				recvPong();
			break;
		}
	}
	return NULL;
}

int identificacion(char* nick){
	return sendNick(sockfd, nick);
}

int main(int argc , char *argv[]){
	struct addrinfo hints, *res;
	char opt;
	int long_index=0;
	char * ip = NULL;
	char * port = NULL;
	static struct option options[] = {
        {"ip",required_argument,0, 1},
        {"port",required_argument,0, 2},
        {"nick",required_argument,0, 3},

        {0,0,0,0}
    };
	while ((opt = getopt_long_only(argc, argv,"1:", options, &long_index )) != -1){
		switch(opt){
			case 1:
				
				ip = malloc(sizeof(char) * strlen(optarg) + 1);
				strcpy(ip, optarg);				
			break;
			case 2:

				port = malloc(sizeof(char) * strlen(optarg) + 1);
				strcpy(port, optarg);

			break;
			case 3:
				if (strlen(optarg)>7){
					printf("La longuitud del nick debe ser menor de 7 caracteres\n");
				}
				nick = malloc(sizeof(char)*8);
				strcpy(nick, optarg);

			break;
		}
	}
	/*Chequeo de que los argumentos esten bien*/
	if(ip == NULL){
		printf("No se especifico la direccion destino\n");
		printf("-ip arg -port arg -nick arg\n");
		return 0;
	}
	if(port == NULL){

		printf("No se especifico la puerto destino\n");
		printf("-ip arg -port arg -nick arg\n");
		return 0;	
	}
	if(nick == NULL){

		printf("No se especifico el nick\n");
		printf("-ip arg -port arg -nick arg\n");
		return 0;
	}
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	/*Creacion de la UI*/
	createClientUI();

	/*Comenzamos la conexion TCP*/
	if(0!=getaddrinfo(ip, port, &hints, &res)){
		printf("No se pudo conectar con el servidor\n");
		return 0;
	}
	sockfd=abrirSocketTCP();
	if(sockfd==-1){
		return 0;
	}
	if(-1==abrirConnect(sockfd, *(res->ai_addr))){
		  
		return 0;
	}

	/*Conexion chat*/
	identificacion(nick);

	/*Creacion de los hilos*/
	pthread_create(&h1,NULL, hiloEscritura, (void *)NULL );

	hiloLectura(NULL);
	
	return 0;
 
}

	 