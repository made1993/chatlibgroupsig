#ifndef COMANDOSU_H
#define COMANDOSU_H

#include "usuario.h"
#include "conexion.h"
#include "comandos.h"
#include "clientui.h"
#include "parser.h"

extern char* nick;
extern int scheme; 
extern unsigned char* skey, *iv;
extern EVP_CIPHER_CTX* ctx;
extern groupsig_key_t *grpkey, *memkey;

int sendNick(int socket, char* nick, int nicklen);
int sendMsg(int socket, char* str, int msglen);
int sendDisconnect(int socket);
int sendPing(int socket);
int sendPong(int socket);


int recvNick(char* msg);
int recvMsg(char* msg);
int recvPing(int socket);
int recvPong();
int recvDisconnect();

#endif