#include <stdio.h>
#include <string.h>	//strlen
#include <arpa/inet.h> //inet_addr
#include <unistd.h>	//write
#include <pthread.h>

#include "../include/comandoss.h"
#define PING_SLEEP 30
#define PING_MAX 60
#define PING_TIME 30


pthread_t* hilos;

LinkedList* listaUsuarios;

void* verificarCliente(void* args){
	int* socket = NULL;
	char * buff = NULL;
	int bufflen, scheme = 0;
	int key_format = -1;

	groupsig_key_t *grpkey;
	EVP_PKEY* privKeyRSA = NULL;

	char s_grpkey[] = ".fg/group/grp.key";
	char s_rsaPrivKey[] = "privkey.pem";

	socket = (int*) args;
	Usuario_t* usr = NULL;
	usr = crearUsuario();
	if(usr == NULL)
		return NULL;

	RSAfileToPrivKey(&privKeyRSA, s_rsaPrivKey);
	if(privKeyRSA == NULL){
		liberarUsuario(usr);
		free(usr);
		return NULL;
	}
	switch(scheme) {
	case GROUPSIG_KTY04_CODE:
		key_format = GROUPSIG_KEY_FORMAT_FILE_NULL_B64;
		break;
	case GROUPSIG_BBS04_CODE:

	case GROUPSIG_CPY06_CODE:

		key_format = GROUPSIG_KEY_FORMAT_FILE_NULL;
		break;
	default:
		fprintf(stderr, "Error: unknown scheme.\n");
		liberarUsuario(usr);
		free(usr);
		return NULL;
	}

	groupsig_init(time(NULL));

	if(!(grpkey = groupsig_grp_key_import(scheme, key_format, s_grpkey))) {
		fprintf(stderr, "Error: invalid group key %s.\n", s_grpkey);
		liberarUsuario(usr);
		free(usr);
		return NULL;
	}
	if(!initUser(usr, *socket, grpkey, NULL, scheme, privKeyRSA)){
		liberarUsuario(usr);
		free(usr);
		return NULL;
	}
	if(!serverInitSConexion(usr->scnx)){
		liberarUsuario(usr);
		free(usr);
		return NULL;
	}
	while(1){

		bufflen = reciveServerCiphMsg(usr->scnx, &buff);
		if(bufflen < 1){
			delete_elem_list(listaUsuarios, (void*) usr);
			liberarUsuario(usr);
			free(usr);
			return NULL;
		}

		if(comando(buff)== NICK){
			recvNick(usr, buff);
			printf("Se registro %s\n", usr->nick);
			sendServerCiphMsg(usr->scnx, (const unsigned char*)"/MSG server bienvenido",
								 strlen((char*)"/MSG server bienvenido")+1);
			//escribir(*usr->socket, "/MSG server bienvenido", strlen((char*)"/MSG server bienvenido")+1);
			break;
		}
		else{
			sendServerCiphMsg(usr->scnx, (const unsigned char*)CDISCONNECT,
							 strlen((char*)CDISCONNECT)+1);
			//escribir(*usr->socket, CDISCONNECT, strlen((char*)CDISCONNECT)+1);

		}

	}
	insert_list(listaUsuarios, usr);
	return usr;
}

void* controlDeConexion(void* args){
	Node* nd = NULL;
	Usuario_t* usr = NULL;
	unsigned int pingt;
	while(1){
		nd = listaUsuarios->first;
		while (nd != NULL){
			usr = (Usuario_t*) nd->data;
			pingt = getPingt(usr);
			if((unsigned)time(NULL)- pingt > PING_TIME){
				sendPing(usr);
			}
			else if((unsigned)time(NULL) - pingt > PING_MAX){
				sendDisconnect(usr);
				delete_elem_list(listaUsuarios, (void*) usr);
				liberarUsuario(usr);

			}
			nd= nd->next;
		}
		sleep(PING_SLEEP);
	}
}

void* lecturaUsuario(void* args){
	char * buff = NULL;
	int bufflen, end = 0;
	Usuario_t* usr = verificarCliente(args);
	if(usr == NULL){
		printf("ERR: fallo al verificar cliente\n");
		return NULL;
	}
	while(!end){
		bufflen = reciveServerCiphMsg(usr->scnx, &buff);
		if(bufflen  < 1){
			printf( "%d\n", delete_elem_list(listaUsuarios, (void*) usr));
			liberarUsuario(usr);
			free(usr);
			end = 1;
			printf("cosas extrañas pueden pasar\n");
			break;
		}
		fwrite(buff, 1, bufflen, stdout);
		printf("bufflen: [%d]\n", bufflen);
		switch (comando(buff)){
			case NICK:
				printf("NICK\n");
				recvNick(usr, buff);
			break;

			case MSG:
				printf("MSG\n");
				recvMsg(buff, bufflen);
			break;

			case DISCONNECT:
				printf("DISCONNECT\n");
				recvDisconnect(usr);
				end = 1;
			break;

			case PING:
				printf("PING\n");
				recvPing(usr);
			break;

			case PONG:			
				printf("PONG\n");
				recvPong(usr);
			break;

			default:
				printf("default\n");

			break;



		}
		free(buff);
	}
	printf("Usuario desconectado\n");
	return NULL;
}


int main(){
	int socket;
	int* socketcli;
	int usuarios = 0;
	pthread_t hiloPing;
	struct sockaddr_in ip4addr;

	socket = abrirSocketTCP();
	if(socket < 1)
		return 0;

	if(abrirBind(socket, 8080) < 0)
		return 0;

	if(abrirListen(socket) == -1)
		return 0;

	listaUsuarios = create_list(compareUsr);
	pthread_create(&hiloPing,NULL, controlDeConexion, NULL );

	while(1){
		socketcli = malloc(sizeof(int));
		*socketcli = aceptar(socket, ip4addr);

		printf("nuevo cliente\n");
		usuarios++;
		hilos= realloc(hilos, sizeof(pthread_t)*usuarios);

		pthread_create(&hilos[usuarios-1],NULL, lecturaUsuario, (void*) socketcli);
		
	}
	printf("fin\n");
	return 0;
}

