#ifndef COMANDOSU_H
#define COMANDOSU_H

#include "usuario.h"
#include "conexion.h"
#include "comandos.h"
#include "clientui.h"

extern char* nick;

int sendNick(int socket, char* nick, int nicklen);
int sendMsg(int socket, char* str, int msglen);
int sendDisconnect(int socket);
int sendPing(int socket);
int sendPong(int socket);


int recvNick();
int recvMsg(char* msg);
int recvDisconnect(int socket);
int recvPing(int socket);
int recvPong();

#endif