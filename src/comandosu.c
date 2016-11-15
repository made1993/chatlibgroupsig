#include "../include/comandosu.h"

int sendNick(Sconexion_t * scnx, char* msg, int msglen){
	char* buff= NULL, *pch1 =  NULL, *pch2 =  NULL;
	int ret = 0, nicklen = 0;
	if(scnx == NULL || msg == NULL || msglen < 1){
		printMsg("NICK ERROR");
		return -1;
	}
	if(strncmp(CNICK, msg, strlen(CNICK))!=0){
		buff = malloc(sizeof(char) * (strlen(CNICK) + 2 + strlen(msg)));
		sprintf(buff, "%s %s", CNICK, msg);

		ret = sendClientCiphMsg(scnx, (const unsigned char*)buff, msglen + strlen(CNICK) + 1);
		//ret = escribir(socket, buff, msglen + strlen(CNICK) + 1);
		free(buff);
		return ret;	
	}
	pch1 = strchr(msg, ' ');
	if (pch1 == NULL){
		printMsg("NICK ERROR");
		return -1;
	}
	pch1++;
	pch2= strchr(pch1, ' ');
	if(pch2 != NULL){
		printMsg("NICK ERROR");
		return -1;
	}
	nicklen =  strlen(pch1)+1;
	if(nicklen < 2 || nicklen > NICK_MAX_LEN){
		printMsg("La longuitud del NICK es incorrecta");
		return -1;
	}
	buff = malloc(strlen(CNICK) + 1 + nicklen );
	sprintf(buff, "%s %s", CNICK, pch1);
	
	ret = sendClientCiphMsg(scnx, (const unsigned char*)buff, nicklen + strlen(CNICK) + 1);
	//ret = escribir(socket, buff, nicklen + strlen(CNICK) + 1);
	free(buff);

	return ret;
}

int sendMsg(Sconexion_t* scnx, char* msg, int msglen){
	char* buff= NULL; 
	int ret = 0, bufflen = 0;
	if(scnx == NULL){
		printMsg("MSG ERROR");
		return -1;
	}
	if(nick == NULL || strlen(nick) > NICK_MAX_LEN || msg == NULL){
		printMsg("MSG ERROR");
		return -1;
	}
	bufflen = strlen(CMSG) + strlen(nick) + msglen + 2;
	buff = malloc(bufflen);
	buff= strcpy(buff, (char*)CMSG);
	buff= strcat(buff, " ");
	buff= strcat(buff, nick);
	buff= strcat(buff, " ");
	buff= strcat(buff, msg);
	ret = sendClientCiphMsg(scnx, (const unsigned char*)buff, bufflen);
	//ret = escribir(socket, buff, bufflen);
	free(buff);
	buff = NULL;
	return ret;

}


int sendDisconnect(Sconexion_t* scnx){
	if(scnx == NULL)
		return 0;
	return sendClientCiphMsg(scnx, (const unsigned char*)CDISCONNECT, strlen(CDISCONNECT)+1);
	//return escribir(socket, CDISCONNECT, strlen(CDISCONNECT)+1);
}

int sendPing(Sconexion_t* scnx){
	if(scnx == NULL)
		return 0;
	return sendClientCiphMsg(scnx,(const unsigned char*)CPING, strlen(CPING)+1);
	//return escribir(socket, CPING, strlen(CPING)+1);
}

int sendPong(Sconexion_t* scnx){
	if(scnx == NULL)
		return 0;
	return sendClientCiphMsg(scnx,(const unsigned char*)CPONG, strlen(CPONG)+1);
	//return escribir(socket, CPONG, strlen(CPONG)+1);
}

int recvNick(char* msg){
	char* nick1 = NULL, *nick2 = NULL;
	char* str;
	parseNick(msg, &nick1, &nick2);
	if(nick1 == NULL || nick2 == NULL){
		printMsg("NICK ERROR");
		return -1;
	}

	if(strcmp(nick1, nick) == 0)
		strcpy(nick, nick2);
	str =  malloc(strlen(nick1) + strlen(nick2) + strlen((char*)"ahora es") + 3);
	sprintf(str, "%s ahora es %s",nick1, nick2);
	printMsg(str);
	free(str);
	free(nick1);
	free(nick2);
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
	sprintf(str, "%s: %s", nick, content);
	printMsg(str);
	free(str);
	free(nick);
	free(content);
	fclose(f);
	return 0;
}

int recvPing(Sconexion_t* scnx){
	if(scnx == NULL){
		printMsg("PING ERROR");
		return -1;
	}
	return sendPong(scnx);
}
int recvPong(){
	
	return 0;
}	
int recvDisconnect(char* msg){
	char* pch1 =  NULL, *pch2 = NULL;
	char * buff;
	if(strcmp(msg, CDISCONNECT) == 0)
		return 1;
	pch1 =  strchr(msg, ' ');
	if(pch1 == NULL)
		return 0;
	pch1++;
	pch2 =  strchr(pch1, ' ');
	if(pch2 == NULL)
		return 0;
	if(strcmp(nick, pch1)== 0){
		printMsg("Has sido desconectado del servidor");
		return 1;
	}
	buff = malloc(strlen("Se ha desconectado") + 2 + strlen(pch1));
	sprintf(buff, "Se ha desconectado %s", pch1);
	printMsg(buff);

	return 0;
}



