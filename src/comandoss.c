#include "../include/comandoss.h"
#include <time.h>

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
	return escribir(*usr->socket, CPING);

}
int sendPong(Usuario_t* usr){
	if(usr==NULL || usr->socket == NULL
		|| *usr->socket <= 0)
		return -1;
	return escribir(*usr->socket, CPONG);

}

int sendMsg(Usuario_t* usr, char* msg){
	
	char* buff= NULL; 
	int ret = 0;
	char* nick= usr->nick;
	if(usr==NULL || usr->socket == NULL
		|| *usr->socket <= 0)
		return -1;
	if(nick == NULL || strlen(nick) > 7 || msg == NULL)
		return -1;
	buff = malloc(8+ strlen(nick)+strlen(msg));
	buff= strcat(CMSG, " ");
	buff= strcat(buff, nick);
	buff= strcat(buff, " ");
	buff= strcat(buff, msg);
	ret = escribir(*usr->socket, buff);
	free(buff);
	return ret;	
}




int recvNick(Usuario_t* usr, char* msg){
	char* nick= NULL;
	if (usr == NULL || msg== NULL)
		return -1;
	nick = msg + strlen(CNICK)+1;
	setNick(usr, nick);


	return 0;

}

int recvPing(Usuario_t* usr){
	if(usr == NULL || usr->socket == NULL)
		return -1;
	return escribir(*usr->socket, CPONG);
}

int recvPong(Usuario_t* usr){
	if(usr == NULL || usr->socket == NULL)
		return -1;
	setCurrentPingt(usr);
	return 0;
}


int broadcastMsg(char * msg){

	return 0;
}
