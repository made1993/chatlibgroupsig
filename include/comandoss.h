#ifndef COMANDOSS_H
#define COMANDOSS_H

#include "comandos.h"
#include "usuario.h"
#include "linkedList.h"
#include "parser.h"

extern LinkedList* listaUsuarios;

extern EVP_CIPHER_CTX* ctx;
extern groupsig_key_t *grpkey;
/**
*
*	Funciones encargadas de los distintos comandos 
*	del lado del servidor.
*
*
**/

void* hiloPing(void* args);
int sendPing(Usuario_t*);
int sendPong(Usuario_t*);
int sendMsg(Usuario_t* usr, char* msg, int msglen);
int sendNick(Usuario_t*, char* newNick);
int sendDisconnect(Usuario_t* usr);

int recvNick(Usuario_t*, char* msg);
int recvMsg(char* msg, int msglen);
int recvDisconnect(Usuario_t*);
int recvPing(Usuario_t*);
int recvPong(Usuario_t*);

int broadcastMsg(char * msg, int msglen);


#endif