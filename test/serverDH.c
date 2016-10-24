#include "../include/funcionesRSA.h"

int main(){
	EVP_PKEY* dhKey, * params;
	const unsigned char msg[] = "hello friend"; 
	unsigned char* sig = NULL;	 
	size_t slen;
	char* buff;
	int sockfd;
	struct sockaddr_in ip4addr;
	int socketcli;	
	getParamsIniDH(&params);


	sockfd = abrirSocketTCP();
	abrirBind(sockfd, 8080);
	abrirListen(sockfd);
	printf("ESPERANDO CLIENTE\n");
	socketcli=aceptar(sockfd, ip4addr);
	

	
	close(socketcli);
	close(sockfd);
	return 0;
}