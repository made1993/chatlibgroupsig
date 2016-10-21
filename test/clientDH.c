#include "../include/funcionesDH.h"
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

int main(){
	struct addrinfo hints, *res;
	char* buff;
	
	EVP_PKEY* pubkey;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int sockfd, status;

	OpenSSL_add_all_algorithms();
	/*Comenzamos la conexion TCP*/
	if(0!=getaddrinfo("127.0.0.1", "8080", &hints, &res)){
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


	
	
	close(sockfd);

	return 1;
}