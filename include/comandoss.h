#ifndef COMANDOSS_H
#define COMANDOSS_H

#include "conexion.h"
#include "comandos.h"
#include "usuario.h"


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
int sendMsg(Usuario_t*,  char* msg);
int sendNick(Usuario_t*, char* newNick);


int recvNick(Usuario_t*, char* msg);
int recvMsg(Usuario_t*, char* msg);
int recvDisconnect(Usuario_t*);
int recvPing(Usuario_t*);
int recvPong(Usuario_t*);

int broadcastMsg(char * msg);


#endif