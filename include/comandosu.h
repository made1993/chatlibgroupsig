#ifndef COMANDOSU_H
#define COMANDOSU_H

#include "usuario.h"
#include "conexion.h"
#include "comandos.h"

int sendPing(int socket);
int sendPong(int socket);
int sendNick(int socket, char* nick);
int sendDisconnect(int socket);


int recvPing(int socket);
int recvPong(int socket);
int recvNick(int socket, char* nick);
int recvDisconnect(int socket);

#endif