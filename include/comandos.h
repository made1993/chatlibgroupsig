#ifndef COMANDOS_H
#define COMANDOS_H


#define NICK 		0xFFFF0000
#define MSG 		0xFFFF0001
#define DISCONNECT 	0xFFFF0002
#define PING 		0xFFFF0003
#define PONG 		0xFFFF0004



#define CNICK 		"/NICK"
#define CMSG 		"/MSG"
#define CDISCONNECT "/DISCONNECT"
#define CPING 		"/PING"
#define CPONG 		"/PONG"

int comando(char* msg);

#endif	