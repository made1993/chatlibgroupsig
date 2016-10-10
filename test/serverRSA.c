#include "../include/funcionesRSA.h"

int main(){
	EVP_PKEY* privKey,* pubKey;
	const unsigned char msg[] = "hello friend"; 
	unsigned char* sig = NULL;	 
	size_t slen;
	char buff[8096];
	int sockfd;
	struct sockaddr_in ip4addr;
	int socketcli;
	
	OpenSSL_add_all_algorithms();
	generateKeysRSA(&privKey, &pubKey);
	signMsgRSA(privKey, msg, &sig, &slen);
	

	sockfd = abrirSocketTCP();
	abrirBind(sockfd, 8080);
	abrirListen(sockfd);
	printf("ESPERNAO CLIENTE\n");
	socketcli=aceptar(sockfd, ip4addr);
	recibir(socketcli, buff);
	printf("%s\n", buff);
	
	sendRSAkey(socketcli, pubKey);
	printf("CLAVE RSA enviada\n");
	sendRSAsign(socketcli, privKey, msg, strlen((char*) msg));
	printf("firma enviada\n");
	return 0;
}