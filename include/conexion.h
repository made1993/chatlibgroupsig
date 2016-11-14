#ifndef _SOCKET_H
#define _SOCKET_H

#include <string.h>
#include <linux/tcp.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/ether.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <inttypes.h>
#include "funcionesRSA.h"
#include "funcionesGS.h"
#include "funcionesDH.h"
#include "funcionesAES.h"

#define MAX_MSG_LEN 8096
#define LAST_MSG 0
#define MORE_MSG 1
typedef struct argumentos{
	char direccion[20];
	char strPuerto[20];
	char *archivo;
	int intPuerto;
}ARGUMENTOS;

struct argumentos args;
int abrirSocketTCP();
int abrirSocketUDP();
int abrirBind(int sockfd,int puerto);
int aceptar(int sockfd, struct sockaddr_in ip4addr);
int abrirConnect(int sockfd, struct sockaddr ip4addr);
int abrirListen(int sockfd);
int recibir(int sockfd,char **buf);
int escribir(int sockfd,char *msg, int mlen);
char* atoIp(char* str);
uint8_t obtenerIPInterface(char * interface, uint8_t* retorno);

int sendClientCiphMsg(int socket, EVP_CIPHER_CTX* ctx, unsigned char* key, unsigned char* iv, 
						const unsigned char* text, int textlen, groupsig_key_t *grpkey, 
						groupsig_key_t *memkey,	int scheme);

int reciveServerCiphMsg(int socket, EVP_CIPHER_CTX* ctx, unsigned char* key, unsigned char* iv, 
						groupsig_key_t *grpkey, char** msg);


int sendServerCiphMsg(int socket, EVP_CIPHER_CTX* ctx, unsigned char* key, unsigned char* iv, 
						const unsigned char* text, int textlen);

int reciveClientCiphMsg(int socket, EVP_CIPHER_CTX* ctx, unsigned char* key, unsigned char* iv, 
						char** msg);

int clientInitSConexion(int socket, EVP_PKEY* pubKeyRSA , groupsig_key_t *grpkey, groupsig_key_t *memkey,
						int scheme, unsigned char** skey, unsigned char** iv);

int serverInitSConexion(int socket, EVP_PKEY* privKeyRSA , groupsig_key_t *grpkey, unsigned char** skey,
						unsigned char** iv);
#endif
