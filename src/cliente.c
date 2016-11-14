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

int scheme = 0; 
unsigned char* skey, *iv;
EVP_CIPHER_CTX* ctx;
groupsig_key_t *grpkey = NULL, *memkey = NULL;

void* hiloEscritura(void* args){
	char* msg = NULL;
	int end = 0;
	if(sockfd ==-1)
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
				sendNick(sockfd, msg, strlen(msg)+1);
			break;
			case MSG2:
				sendMsg(sockfd, msg, strlen(msg)+1);
			break;
			case DISCONNECT:
				end = sendDisconnect(sockfd);
			break;
			case PING:
				sendPing(sockfd);
			break;
			case PONG:
				sendPong(sockfd);
			break;

			default:
			break;
		}
		free(msg);
		msg = NULL;
	}

	pthread_cancel(h2);
	return NULL;
}


void* hiloLectura(void* args){
	char* msg = NULL;
	int end = 0, ret = 0;
	if(sockfd ==-1)
		return NULL;
	while(!end){
		if((ret = reciveClientCiphMsg(sockfd, ctx, (unsigned char*)skey, (unsigned char*)iv, &msg)) <1)
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
				recvPing(sockfd);
			break;
			case PONG:
				recvPong();
			break;
		}
	}
	pthread_cancel(h1);
	return NULL;
}

int identificacion(char* nick){
	return sendNick(sockfd, nick, strlen(nick)+1);
}

int main(int argc , char *argv[]){
	struct addrinfo hints, *res;
	char opt;
	int long_index=0;
	char * ip = NULL;
	char * port = NULL;

	EVP_PKEY* pubKeyRSA = NULL;
	
	
	int key_format;

	char s_grpkey[] = ".fg/group/grp.key";
	char s_memkey[] = ".fg/members/0.key";
	char s_pubKey[] = "pubkey.pub";

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
				if (strlen(optarg) > NICK_MAX_LEN){
					printf("La longuitud del nick debe ser menor de %d caracteres\n", NICK_MAX_LEN);
					return 0;
				}
				nick = malloc(sizeof(char) * NICK_MAX_LEN + 1);
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

	RSAfileToPubKey(&pubKeyRSA, s_pubKey);
	if(pubKeyRSA == NULL){
		printf("ERR\n");
		return -1;
	}
	if(!(grpkey = groupsig_grp_key_import(scheme, key_format, s_grpkey))) {
		fprintf(stderr, "Error: invalid group key %s.\n", s_grpkey);
		return IERROR;
	}

	if(!(memkey = groupsig_mem_key_import(scheme,	key_format, s_memkey))) {
		fprintf(stderr, "Error: invalid member key %s.\n", s_memkey);
		return IERROR;
	}
	if(!clientInitSConexion(sockfd, pubKeyRSA , grpkey, memkey,
						scheme, &skey, &iv))
		return 0;
	

	/*Conexion chat*/
	
	ctx = create_ctx();
	identificacion(nick);
	/*Creacion de los hilos*/
	//pthread_create(&h1,NULL, hiloEscritura, (void *)NULL );
	pthread_create(&h2,NULL, hiloLectura, (void *)NULL );
	hiloEscritura(NULL);
	destroyClientUI();
	close(sockfd);
	free(nick);
	
	return 0;
 
}

	 