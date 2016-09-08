#include "../include/comandos.h"
#include <string.h>
int comando(char* msg){
	if(strncmp(msg, CNICK, strlen(CNICK))==0) 
		return NICK;
	else if(strncmp(msg, CMSG, strlen(CMSG))==0) 
		return MSG;
	else if(strncmp(msg, CDISCONNECT, strlen(CDISCONNECT))==0)
		return DISCONNECT;
	else if(strncmp(msg, CPING, strlen(CPING))==0)
		return PING;
	else if(strncmp(msg, CPONG, strlen(CPONG))==0)
		return PONG;
	return MSG2;
}
