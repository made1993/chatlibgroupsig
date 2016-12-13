#include "../include/usuario.h"

int nUsr = 0;

Usuario_t* crearUsuario(){
	Usuario_t* usr = NULL;
	
	usr = malloc(sizeof(Usuario_t));
	usr->id = nUsr++;
	usr->nick = malloc(NICK_MAX_LEN + 1);
	usr->nick[0] = '\0';
	setCurrentPingt(usr);
	return usr;
}

int initUser(Usuario_t* usr, int socket, groupsig_key_t *grpkey, groupsig_key_t *memkey, int scheme, EVP_PKEY* keyRSA){
	if(usr == NULL)
		return 0;
	usr->scnx = initSconexion(socket, grpkey, memkey, scheme, keyRSA);
	if(usr->scnx ==  NULL)
		return 0;
	return 1;
}

int liberarUsuario(Usuario_t* usr){
	if(usr == NULL)
		return 1;
	if(!(usr->nick == NULL)){
		free(usr->nick);
		usr->nick = NULL;
	}
	freeSconexion(usr->scnx); 
	free(usr->scnx); usr->scnx = NULL;
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


int setKey(Usuario_t* usr, char** key){
	if(usr == NULL || key == NULL || *key == NULL || usr->scnx == NULL)
		return 0;

	usr->scnx->key = (unsigned char*)*key;
	return 1;
}
char* getKey(Usuario_t* usr){
	if(usr == NULL)
		return NULL;

	return (char*)usr->scnx->key;
}

int setIv(Usuario_t* usr, char** iv){
	if(usr == NULL || iv == NULL || *iv == NULL || usr->scnx == NULL)
		return 0;

	usr->scnx->iv = (unsigned char*)*iv;
	return 1;
}
char* getIv(Usuario_t* usr){
	if(usr == NULL)
		return NULL;

	return (char*)usr->scnx->iv;
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
