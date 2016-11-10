#include "../include/comandosu.h"

int sendNick(int socket, char* msg, int nicklen){
	char* buff= NULL;
	int ret = 0;
	if(nick == NULL)
		return -1;
	buff = malloc(sizeof(char) * (strlen(CNICK) + 2 + strlen(nick)));
	sprintf(buff, "%s %s", CNICK, nick);
	ret = escribir(socket, buff, nicklen + strlen(CNICK) + 1);
	free(buff);
	return ret;
}

int sendMsg(int socket, char* msg, int msglen){
	char* buff= NULL; 
	int ret = 0, bufflen = 0;
	if(socket <= 0)
		return -1;
	if(nick == NULL || strlen(nick) > NICK_MAX_LEN || msg == NULL)
		return -1;
	bufflen = strlen(CMSG) + strlen(nick) + msglen + 2;
	buff = malloc(bufflen);
	buff= strcpy(buff, (char*)CMSG);
	buff= strcat(buff, " ");
	buff= strcat(buff, nick);
	buff= strcat(buff, " ");
	buff= strcat(buff, msg);
	ret = escribir(socket, buff, bufflen);
	free(buff);
	buff = NULL;
	return ret;

}


int sendDisconnect(int socket){
	return escribir(socket, CDISCONNECT, strlen(CDISCONNECT)+1);
}

int sendPing(int socket){
	return escribir(socket, CPING, strlen(CPING)+1);
}

int sendPong(int socket){
	return escribir(socket, CPONG, strlen(CPONG)+1);
}

int recvNick(){
	return 0;
}

int recvMsg(char* msg){
	char* nick = NULL;
	char* content = NULL;
	char* str =  NULL;
	FILE* f;
	f = fopen("trash.txt", "a");
	fprintf(f, "%s\n", msg);
	
	parseMsg(msg, &nick, &content);
	str = malloc(strlen(nick) + strlen(content) +2);
	sprintf(str, "%s:%s", nick, content);
	printMsg(str);
	free(str);
	free(nick);
	free(content);
	fclose(f);
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


