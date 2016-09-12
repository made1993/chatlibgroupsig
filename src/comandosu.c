#include "../include/comandosu.h"

int sendNick(int socket, char* msg){
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

int sendMsg(int socket, char* msg){
	char* buff= NULL; 
	int ret = 0;
	if(socket <= 0)
		return -1;
	if(nick == NULL || strlen(nick) > 7 || msg == NULL)
		return -1;

	buff = malloc(10 + strlen(nick) + strlen(msg));
	buff= strcat(buff, CMSG);
	buff= strcat(buff, " ");
	buff= strcat(buff, nick);
	buff= strcat(buff, " ");
	buff= strcat(buff, msg);
	ret = escribir(socket, buff);
	free(buff);
	return ret;

}


int sendDisconnect(int socket){
	return escribir(socket, CDISCONNECT);
}

int sendPing(int socket){
	return escribir(socket, CPING);
}

int sendPong(int socket){
	return escribir(socket, CPONG);
}

int recvNick(){
	return 0;
}

int recvMsg(char* msg){
	char* nick = NULL;
	char* content = NULL;
	char* pch = NULL;
	char* str= NULL;
	int nlen = 0;
	msg += strlen(CNICK);
	pch = strchr(msg, ' ');
	nlen = pch - msg;
	nick = malloc(sizeof(char) * (nlen + 1));
	strncpy(nick, msg, nlen);

	msg += nlen + 1;
	
	content = malloc(sizeof(char) * (strlen(msg) + 1));
	strcpy(content, msg);
	str = malloc(sizeof(char) * (strlen(nick) + strlen(content)) + 1 );
	sprintf(str,"%s:%s", nick, content);
	printMsg(str);
	free(str);
	free(nick);
	free(content);
	return 0;
}

int recvPing(int socket){
	return sendPong(socket);
}
int recvPong(){
	
	return 0;
}	
int recvDisconnect(int socket){
	return 0;
}


