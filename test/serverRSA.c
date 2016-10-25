#include "../include/funcionesRSA.h"

int main(){
	EVP_PKEY* privKey, * pubKey;
	const unsigned char msg[] = "hello friend"; 
	int sockfd;
	struct sockaddr_in ip4addr;
	int socketcli;
	
	OpenSSL_add_all_algorithms();
	generateKeysRSA(&privKey, &pubKey);
	

	sockfd = abrirSocketTCP();
	abrirBind(sockfd, 8080);
	abrirListen(sockfd);
	printf("ESPERANDO CLIENTE\n");
	socketcli=aceptar(sockfd, ip4addr);
	
	sendRSAkey(socketcli, pubKey);
	printf("CLAVE RSA enviada\n");
	sendRSAsign(socketcli, privKey, msg, strlen((char*) msg)+1);
	printf("firma enviada\n");
	close(socketcli);
	close(sockfd);
	return 0;
}