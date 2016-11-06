#ifndef COMANDOSS_H
#define COMANDOSS_H

#include "conexion.h"
#include "comandos.h"
#include "usuario.h"
#include "linkedList.h"
extern LinkedList* listaUsuarios;

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


int recvNick(Usuario_t*, char* msg);
int recvMsg(char* msg, int msglen);
int recvDisconnect(Usuario_t*);
int recvPing(Usuario_t*);
int recvPong(Usuario_t*);

int broadcastMsg(char * msg, int msglen);


#endif