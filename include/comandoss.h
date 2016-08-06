#ifndef COMANDOSS_H
#define COMANDOSS_H

#include "conexion.h"
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
int sendMsg(Usuario_t*, char* msg);
int sendNick(Usuario_t*, char* nick);

int recvPing(Usuario_t*);
int recvPong(Usuario_t*);
int recvMsg(Usuario_t*, char* msg);
int recvNick(Usuario_t*, char* nick);


#endif