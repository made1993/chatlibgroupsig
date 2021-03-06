#include <stdio.h>
#include <string.h>	//strlen
#include <arpa/inet.h> //inet_addr
#include <unistd.h>	//write
#include <pthread.h>
#include <signal.h>

#include "../include/comandoss.h"

#ifdef TIMETEST
#define PING_SLEEP 10
#define PING_TIME 1
#else

#define PING_SLEEP 30
#define PING_TIME 30

#endif

#define PING_MAX 60
pthread_t* hilos;

LinkedList* listaUsuarios;

EVP_PKEY* privKeyRSA = NULL;
groupsig_key_t *grpkey = NULL;
groupsig_key_t *mgrkey = NULL;
crl_t* crl = NULL;
gml_t* gml = NULL;
char* s_crlf = NULL;
int scheme = GROUPSIG_CPY06_CODE;

#ifdef TIMETEST
FILE* f = NULL;
#endif

void sig_handler(int signo){
	if (signo == SIGINT || signo == SIGTERM){
		#ifdef TIMETEST
		fclose(f);
		#endif
    	printf("received end signal\n");
	}
	exit(0);
}


void* controlDeConexion(void* args){
	Node* nd = NULL;
	Usuario_t* usr = NULL;
	unsigned int pingt;
	#ifdef TIMETEST 
	clock_t ini, fin, tot;
	
	f =fopen("tmedioenv.dat", "w");
	#endif
	while(1){
		nd = listaUsuarios->first;
		while (nd != NULL){
			printf("PING\n");
			usr = (Usuario_t*) nd->data;
			pingt = getPingt(usr);
			if((unsigned)time(NULL)- pingt > PING_TIME){
				#ifdef TIMETEST
				ini = clock();
				sendPing(usr);
				fin = clock();
				tot = fin-ini;
				fprintf(f, "%ld\n", tot);
				#else
				sendPing(usr);
				#endif
			}
			else if((unsigned)time(NULL) - pingt > PING_MAX){
				sendDisconnect(usr);
				delete_elem_list(listaUsuarios, (void*) usr);
				liberarUsuario(usr);
				free(usr);
				usr = NULL;

			}
			nd= nd->next;
		}
		sleep(PING_SLEEP);
	}
}


void* verificarCliente(void* args){
	int* socket = NULL;
	char * buff = NULL;
	int bufflen;
	#ifdef INISCTEST
	clock_t ini, fin, tot;
	FILE *f;
	f = fopen("iniscnxS.dat", "a");
	#endif

	socket = (int*) args;
	Usuario_t* usr = NULL;
	usr = crearUsuario();
	if(usr == NULL)
		return NULL;
	
	if(!initUser(usr, *socket, grpkey, NULL, scheme, privKeyRSA)){
		fprintf(stdout, "Error: failure creating new user.\n");
		liberarUsuario(usr);
		free(usr);
		usr = NULL;
		return NULL;
	}
	
	#ifdef INISCTEST
	ini = clock();
	#endif

	if(!serverInitSConexion(usr->scnx, mgrkey, crl, gml)){
		fprintf(stdout, "Error: failure creating secure conexion.\n");
		liberarUsuario(usr);
		free(usr);
		usr = NULL;
		return NULL;
	}


	#ifdef INISCTEST
	fin = clock();
	tot = fin - ini;
	fprintf(f, "%ld\n", tot);
	fclose(f);
	#endif

	while(1){

		bufflen = reciveServerCiphMsg(usr->scnx, &buff);
		if(bufflen < 1){
			fprintf(stdout, "Error: invalid message at identification\n");
			delete_elem_list(listaUsuarios, (void*) usr);
			liberarUsuario(usr); 
			free(usr);
			usr = NULL;
			return NULL;
		}

		if(comando(buff)== NICK){
			recvNick(usr, buff);
			fprintf(stdout, "Se registro %s\n", usr->nick);
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


void* lecturaUsuario(void* args){
	char * buff = NULL;
	int bufflen, end = 0;
	Usuario_t* usr = verificarCliente(args);
	if(usr == NULL){
		fprintf(stdout, "Error: fallo al verificar cliente\n");
		return NULL;
	}
	while(!end){
		bufflen = reciveServerCiphMsg(usr->scnx, &buff);
		if(bufflen  < 1){
			fprintf(stdout,  "%d\n", delete_elem_list(listaUsuarios, (void*) usr));
			recvDisconnect(usr);
			liberarUsuario(usr);
			free(usr);
			usr = NULL;
			end = 1;
			fprintf(stdout, "cosas extrañas pueden pasar\n");
			break;
		}
		fwrite(buff, 1, bufflen, stdout);
		fprintf(stdout, "bufflen: [%d]\n", bufflen);
		switch (comando(buff)){
			case NICK:
				fprintf(stdout, "NICK\n");
				recvNick(usr, buff);
			break;

			case MSG:
				fprintf(stdout, "MSG\n");
				if(recvMsg(buff, bufflen)){
					/*SE REVOCA AL MIEMBRO*/
					revokeClient(usr->scnx, mgrkey, gml, crl, s_crlf);
					printf("se empieza a recargar el crl\n");
					crlRelaod(usr->scnx, &crl, s_crlf);
					printf("se recargo el crl\n");

					recvDisconnect(usr);
					liberarUsuario(usr);
					free(usr);
					end = 1;
				}
			break;

			case DISCONNECT:
				fprintf(stdout, "DISCONNECT\n");
				recvDisconnect(usr);
				liberarUsuario(usr);
				free(usr);
				end = 1;
			break;

			case PING:
				fprintf(stdout, "PING\n");
				recvPing(usr);
			break;

			case PONG:			
				fprintf(stdout, "PONG\n");
				recvPong(usr);
			break;

			default:
				fprintf(stdout, "default\n");

			break;



		}
		free(buff);
	}
	fprintf(stdout, "Usuario desconectado\n");

	return NULL;
}


int main(){
	int socket;
	int* socketcli;
	int usuarios = 0;
	pthread_t hiloPing;
	struct sockaddr_in ip4addr;

	char s_grpkey[] = ".fg/group/grp.key";
	char s_crl[] = ".fg/manager/crl";
	char s_gml[] = ".fg/manager/gml";
	char s_mgrkey[] = ".fg/manager/mgr.key";
	
	s_crlf = s_crl;

	char s_rsaPrivKey[] = "privkey.pem";

	RSAfileToPrivKey(&privKeyRSA, s_rsaPrivKey);
	if(privKeyRSA == NULL){
		fprintf(stdout, "Error: failure loading RSA key\n");
		return 0;
	}

	/*CONFIGURACION DE SEÑAELS DEL SISTEMA*/
	
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGINT\n");
	if(signal(SIGTERM, sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGTERM\n");
	/*INICIAIZACION DE COSAS VARIAS*/
	iniBigBrother(NULL);
	groupsig_init(time(NULL));

	if(0 == import_manager(&grpkey, &mgrkey, &crl, &gml, 
			s_grpkey, s_mgrkey, s_crl, s_gml, scheme))
		return 0;

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

		fprintf(stdout, "nuevo cliente\n");
		usuarios++;
		hilos= realloc(hilos, sizeof(pthread_t)*usuarios);

		pthread_create(&hilos[usuarios-1],NULL, lecturaUsuario, (void*) socketcli);
		pthread_detach(hilos[usuarios-1]);
		
	}

	fprintf(stdout, "fin\n");
	free(privKeyRSA);
	free(grpkey);
	free(mgrkey);
	free(crl);
	free(gml);
	freeBigBrother();

	return 0;
}

