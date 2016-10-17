#include "../include/funcionesRSA.h"

int main(){
	EVP_PKEY* privKey;
	RSA* rsaPub;
	const unsigned char msg[] = "hello friend"; 
	unsigned char* sig = NULL;	 
	size_t slen;
	char* buff;
	int sockfd;
	struct sockaddr_in ip4addr;
	int socketcli;
	
	OpenSSL_add_all_algorithms();
	generateKeysRSA(&privKey, &rsaPub);
	signMsgRSA(privKey, msg, &sig, &slen);
	

	sockfd = abrirSocketTCP();
	abrirBind(sockfd, 8080);
	abrirListen(sockfd);
	printf("ESPERNAO CLIENTE\n");
	socketcli=aceptar(sockfd, ip4addr);
	printf ("%d\n", recibir(socketcli, &buff));
	printf("%s\n", buff);
	
	//sendRSAkey(socketcli, rsaPub);
	//printf("CLAVE RSA enviada\n");
	//sendRSAsign(socketcli, privKey, msg, strlen((char*) msg));
	//printf("firma enviada\n");
	return 0;
}