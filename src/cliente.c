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

pthread_t h1, h2;
char* nick;

Sconexion_t* scnx = NULL;

void* hiloEscritura(void* args){
	char* msg = NULL;
	int end = 0;
	if(scnx == NULL)
		return NULL;
	while (!end){

		msg = scanMsg();
		if(strlen(msg) == 0){
			free(msg);
			msg = NULL;
			continue;
		}
		switch(comando(msg)){
			case NICK:
				sendNick(scnx, msg, strlen(msg)+1);
			break;
			case MSG2:
				sendMsg(scnx, msg, strlen(msg)+1);
			break;
			case DISCONNECT:
				end = sendDisconnect(scnx);
				destroyClientUI();
			break;
			case PING:
				sendPing(scnx);
			break;
			case PONG:
				sendPong(scnx);
			break;

			default:
			break;
		}
		free(msg);
		msg = NULL;
	}
	printf("hilo escritura termina\n");
	pthread_cancel(h2);
	return NULL;
}


void* hiloLectura(void* args){
	char* msg = NULL;
	int end = 0, ret = 0;
	if(scnx == NULL)
		return NULL;
	while(!end){
		if((ret = reciveClientCiphMsg(scnx, &msg)) <1)
			break;
		
		switch(comando(msg)){
			case NICK:
				recvNick(msg);
			break;
			case MSG:
				recvMsg(msg);
			break;
			case DISCONNECT:
				recvDisconnect();
				end = 1;
			break;
			case PING:
				recvPing(scnx);
			break;
			case PONG:
				recvPong();
			break;
		}
		free(msg);
	}

	destroyClientUI();
	printf("Se peridio la conexion con el servidor\n");
	pthread_cancel(h1);
	return NULL;
}

int identificacion(char* nick){
	return sendNick(scnx, nick, strlen(nick)+1);
}

int main(int argc , char *argv[]){
	struct addrinfo hints, *res;
	char opt;
	int long_index=0;
	char * ip = NULL;
	char * port = NULL;
	int sockfd = -1;
	void* thstatus;

	int scheme = GROUPSIG_CPY06_CODE; 
	groupsig_key_t *grpkey = NULL, *memkey = NULL;
	EVP_PKEY* pubKeyRSA = NULL;
	
	
	int key_format;

	char s_grpkey[] = ".fg/group/grp.key";
	char s_pubKey[] = "pubkey.pub";
	char* s_memkey = NULL;

	static struct option options[] = {
        {"ip",required_argument,0, 1},
        {"port",required_argument,0, 2},
        {"nick",required_argument,0, 3},
        {"memkey",required_argument,0, 4},

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
				if (strlen(optarg) > NICK_MAX_LEN){
					printf("La longuitud del nick debe ser menor de %d caracteres\n", NICK_MAX_LEN);
					return 0;
				}
				nick = malloc(sizeof(char) * NICK_MAX_LEN + 1);
				strcpy(nick, optarg);

			break;
			case 4:
				s_memkey = malloc(sizeof(char) * strlen(optarg) + 1);
				strcpy(s_memkey, optarg);
			break;
		}
	}
	/*Chequeo de que los argumentos esten bien*/
	if(ip == NULL){
		printf("No se especifico la direccion destino\n");
		printf("-ip arg -port arg -nick arg -memkey arg\n");
		return 0;
	}
	if(port == NULL){

		printf("No se especifico la puerto destino\n");
		printf("-ip arg -port arg -nick arg -memkey arg\n");
		return 0;	
	}
	if(nick == NULL){

		printf("No se especifico el nick\n");
		printf("-ip arg -port arg -nick arg -memkey arg\n");
		return 0;
	}

	if(s_memkey == NULL){

		printf("No se especifico la clave de miembro\n");
		printf("-ip arg -port arg -nick arg -memkey arg\n");
		return 0;
	}
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	

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

	/*CONEXION SEGURA*/
	switch(scheme) {
	case GROUPSIG_KTY04_CODE:
		key_format = GROUPSIG_KEY_FORMAT_FILE_NULL_B64;
		break;
	case GROUPSIG_BBS04_CODE:

	case GROUPSIG_CPY06_CODE:

		key_format = GROUPSIG_KEY_FORMAT_FILE_NULL;
		break;
	default:
		fprintf(stderr, "Error: unknown scheme.\n");
		return IERROR;
	}
	groupsig_init(time(NULL));
	RSAfileToPubKey(&pubKeyRSA, s_pubKey);
	if(pubKeyRSA == NULL){
		printf("ERR\n");
		return -1;
	}

	if(!(grpkey = groupsig_grp_key_import(scheme, key_format, s_grpkey))) {
		fprintf(stderr, "Error: invalid group key %s.\n", s_grpkey);
		return IERROR;
	}

	if(!(memkey = groupsig_mem_key_import(scheme, key_format, s_memkey))) {
		fprintf(stderr, "Error: invalid member key %s.\n", s_memkey);
		return IERROR;
	}

	scnx = initSconexion(sockfd, grpkey, memkey, scheme, pubKeyRSA);
	if(scnx == NULL || !clientInitSConexion(scnx))
		return 0;
	

	/*Conexion chat*/
	identificacion(nick);

	/*Creacion de la UI*/
	createClientUI();

	/*Creacion de los hilos*/
	pthread_create(&h1,NULL, hiloEscritura, (void *)NULL );
	pthread_create(&h2,NULL, hiloLectura, (void *)NULL );
	pthread_join(h2, &thstatus);
	pthread_join(h1, &thstatus);
	close(sockfd);
	free(nick);
	
	return 0;
 
}

	 