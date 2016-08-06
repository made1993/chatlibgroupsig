#include "../include/comandoss.h"


void* hiloPing(void* args){
	Usuario_t* usr = NULL;
	if (args == NULL)
		return NULL;
	usr = (Usuario_t*)args;

	while (1){
		if(usr->pingt >= 30){
			sendPing(usr);
		}
		else{
			sleep(30);
		}
	}
	return NULL;
}

int sendPing(Usuario_t* usr){
	if(usr==NULL || usr->socket == NULL
		|| *usr->socket <= 0)
		return -1;
	return escribir(*usr->socket, "/ping");

}
int sendPong(Usuario_t* usr){
	if(usr==NULL || usr->socket == NULL
		|| *usr->socket <= 0)
		return -1;
	return escribir(*usr->socket, "/pong");

}

int sendMsg(Usuario_t*, char* nick, char* msg){
	
	char* buff= NULL; 
	int ret = 0;
	if(usr==NULL || usr->socket == NULL
		|| *usr->socket <= 0)
		return -1;
	if(nick == NULL || strlen(nick > 7) || msg == NULL)
		return -1;
	buff = malloc(8+ strlen(nick)+strlen(msg));
	buff= strcat("/msg ", nick);
	buff= strcat(buff, " ");
	buff= strcat(buff, msg);
	ret = escribir(*usr->socket, buff);
	free(buff);
	return ret;	
}

int sendNick(Usuario_t*, char* nick, char* newNick){
	
	if(usr==NULL || usr->socket == NULL
		|| *usr->socket <= 0)
		return -1;

	return escribir(*usr->socket, "/nick");	
}