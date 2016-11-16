#include "../include/comandoss.h"
#include <time.h>


int sendPing(Usuario_t* usr){
	if(usr==NULL || usr->scnx == NULL)
		return -1;

	return sendServerCiphMsg(usr->scnx, (const unsigned char*)CPING, strlen(CPING)+1);
	//return escribir(*usr->socket, CPING, strlen(CPING)+1);

}
int sendPong(Usuario_t* usr){
	if(usr==NULL || usr->scnx == NULL)
		return -1;
	return sendServerCiphMsg(usr->scnx, (const unsigned char*)CPONG, strlen(CPONG)+1);
	//return escribir(*usr->socket, CPONG, strlen(CPONG)+1);

}

int sendMsg(Usuario_t* usr, char* msg, int msglen){
	
	char* buff= NULL; 
	int ret = 0;
	char* nick= usr->nick;
	if(usr==NULL || usr->scnx == NULL)
		return -1;
	if(nick == NULL || strlen(nick) > NICK_MAX_LEN || msg == NULL)
		return -1;
	buff = malloc(10 + strlen(nick) + strlen(msg));

	buff= strcat(buff, CMSG);
	buff= strcat(buff, " ");
	buff= strcat(buff, nick);
	buff= strcat(buff, " ");
	buff= strcat(buff, msg);
	ret = sendServerCiphMsg(usr->scnx, (const unsigned char*)buff, msglen);
	//ret = escribir(*usr->socket, buff, msglen);
	free(buff);
	return ret;	
}

int sendNick(Usuario_t* usr, char* nick){
	char* msg =  NULL;
	if(usr == NULL || nick == NULL || strlen(nick) < 1)
		return -1;
	msg =  malloc(strlen(CNICK) + strlen(usr->nick) + strlen(nick) +3);
	sprintf(msg, "%s %s %s", CNICK, usr->nick, nick);
	broadcastMsg(msg, strlen(msg)+1);
	return 0;
}
int sendDisconnect(Usuario_t* usr){
	if(usr==NULL || usr->scnx == NULL)
		return -1;
	sendServerCiphMsg(usr->scnx, (const unsigned char*)CDISCONNECT, strlen(CDISCONNECT)+1);
	//escribir(*usr->socket, CDISCONNECT, strlen(CDISCONNECT)+1);
	delete_elem_list(listaUsuarios, (void*) usr);
	liberarUsuario(usr);
	return 0;
}



int recvNick(Usuario_t* usr, char* msg){
	char* nick1 = NULL, * nick2 = NULL;
	
	if (usr == NULL || msg== NULL)
		return -1;

	parseNick(msg, &nick1, &nick2);
	if(nick2 != NULL)
		return -1;
	sendNick(usr, nick1);

	setNick(usr, nick1);


	return 0;

}
int recvMsg(char * msg, int msglen){
	if(listaUsuarios == NULL || msg == NULL || msglen < 1)
		return -1;
	broadcastMsg(msg, msglen);

	return 0;
}

int recvDisconnect(Usuario_t* usr){
	char* buff;

	buff =  malloc(strlen("/MSG server se ha desconectado") + strlen(usr->nick) +2);
	sprintf(buff, "/MSG server %s se ha desconectado", usr->nick);
	broadcastMsg(buff, strlen(buff)+1);
	
	delete_elem_list(listaUsuarios, (void*) usr);
	liberarUsuario(usr);
	free(buff);
	return 0;
}

int recvPing(Usuario_t* usr){
	if(usr == NULL || usr->scnx == NULL)
		return -1;

	return sendServerCiphMsg(usr->scnx, (const unsigned char*)CPONG, strlen(CPONG)+1);
	//return escribir(*usr->socket, CPONG, strlen(CPONG)+1);
}

int recvPong(Usuario_t* usr){
	if(usr == NULL || usr->scnx == NULL)
		return -1;
	printf("se recibe pong\n");
	setCurrentPingt(usr);
	return 0;
}


int broadcastMsg(char * msg, int msglen){
	Node* nd = NULL;
	Usuario_t* usr = NULL;
	if(listaUsuarios == NULL || msg == NULL)
		return -1;
	nd = listaUsuarios->first;
	while (nd != NULL){
		usr = (Usuario_t*) nd->data;
		if(usr->scnx != NULL)
			sendServerCiphMsg(usr->scnx, (const unsigned char*)msg, msglen);
			//escribir(*(usr->socket), msg, msglen);
		nd= nd->next;
	}


	return 0;
}
