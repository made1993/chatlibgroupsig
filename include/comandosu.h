#ifndef COMANDOSU_H
#define COMANDOSU_H

#include "usuario.h"
#include "comandos.h"
#include "clientui.h"
#include "parser.h"
#include "sconexion.h"

extern char* nick;
extern int scheme; 
extern unsigned char* skey, *iv;
extern EVP_CIPHER_CTX* ctx;
extern groupsig_key_t *grpkey, *memkey;

int sendNick(Sconexion_t * scnx, char* nick, int nicklen);
int sendMsg(Sconexion_t * scnx, char* str, int msglen);
int sendDisconnect(Sconexion_t * scnx);
int sendPing(Sconexion_t * scnx);
int sendPong(Sconexion_t * scnx);


int recvNick(char* msg);
int recvMsg(char* msg);
int recvPing(Sconexion_t * scnx);
int recvPong();
int recvDisconnect();

#endif