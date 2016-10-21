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
	signMsgRSA(privKey, msg, &sig, &slen, strlen( (char*)msg));
	

	sockfd = abrirSocketTCP();
	abrirBind(sockfd, 8080);
	abrirListen(sockfd);
	printf("ESPERANDO CLIENTE\n");
	
	
	close(socketcli);
	close(sockfd);
	return 0;
}