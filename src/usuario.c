#include "../include/usuario.h"

int nUsr = 0;

Usuario_t* crearUsuario(int* socket){
	Usuario_t* usr = NULL;

	if (socket == NULL || *socket <= 0)
		return NULL;
	usr = malloc(sizeof(Usuario_t));
	usr->id = nUsr++;
	usr->nick = malloc(NICK_MAX_LEN + 1);
	usr->socket = malloc(sizeof(int));
	*usr->socket = *socket;
	setCurrentPingt(usr);
	return usr;
}

int liberarUsuario(Usuario_t* usr){
	if(usr == NULL)
		return 1;
	if(!(usr->nick == NULL))
		free(usr->nick);
	if(!(usr->socket == NULL))
		free(usr->socket);
	free(usr);
	return 0; 
}

int setNick(Usuario_t* usr ,char* nick){
	int len = 0;
	if(nick == NULL)
		return -1;
	
	if(usr->nick == NULL)
		return -2;
	
	len = strlen(nick);
	
	if (len > NICK_MAX_LEN || len ==0)
		return 1;

	strcpy(usr->nick, nick);
	return 0;
}

char* getNick(Usuario_t* usr){
	if (usr == NULL)
		return NULL;
	return usr->nick;
}

int setId(Usuario_t* usr, int id){
	if(usr == NULL)
		return -1;
	usr->id = id;
	return 0;

}
int getId(Usuario_t* usr){
	if(usr == NULL)
		return -1;
	return usr->id;

}


int setCurrentPingt(Usuario_t* usr){
	if(usr == NULL)
		return -1;
	usr->pingt= (int) time(NULL);
	return 0;
}

int getPingt(Usuario_t* usr){
	if(usr == NULL)
		return -1;
	return usr->pingt;
}


int compareUsr(const void* data1, const void* data2){
	Usuario_t* usr1, * usr2;
	if(data1 == NULL || data2 == NULL)
		return -1;
	
	usr1= (Usuario_t*) data1;
	usr2= (Usuario_t*) data2;
	if (usr1->id == usr2->id)
		return 0;
	else
		return 1;
}
