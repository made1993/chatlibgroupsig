#include "../include/comandosu.h"


int ping(int socket){
	return escribir(socket, CPING);
}
int sendPong(int socket){
	return escribir(socket, CPONG);
}
int sendNick(int socket, char* nick){
	char* buff= NULL;
	int ret = 0;
	if(nick == NULL)
		return -1;
	buff = malloc(sizeof(char) * (strlen(CNICK) + 2 + strlen(nick)));
	sprintf(buff, "%s %s", CNICK, nick);
	ret = escribir(socket, buff);
	free(buff);
	return ret;
}
int sendDisconnect(int socket){
	return escribir(socket, CDISCONNECT);
}


int recvPing(int socket){
	return sendPong(socket);
}
int recvPong(int socket){
	return 0;
}

int recvMsg(int socket, char* msg){
	char* nick = NULL;
	char* content = NULL;
	char* pch = NULL;
	int nlen = 0;
	msg += strlen(CNICK) + 1;
	pch = strchr(msg, ' ');
	nlen = pch - msg;
	nick = malloc(sizeof(char) * (nlen + 1));
	strncpy(nick, msg, nlen);

	msg += nlen + 1;
	
	content = malloc(sizeof(char) * (strlen(msg) + 1));
	strcpy(content, msg);

	printf("%s:%s\n", nick, content);
	free(nick);
	free(content);
	return 0;
}
int recvDisconnect(int socket){
	return 0;
}


