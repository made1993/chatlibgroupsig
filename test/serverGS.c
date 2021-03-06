#include "../include/funcionesGS.h"
#include "../include/conexion.h"

int main(int argc, char const **argv){

	int key_format;
	uint8_t scheme = -1;
	char s_grpkey[] = ".fg/group/grp.key";
	char s_memkey[] = ".fg/members/0.key";
	char msgstr[] = "hola mundo";
	char * sigstr, *dest;
	int size;
	groupsig_key_t *grpkey = NULL, *memkey = NULL;
	
	int sockfd;
	struct sockaddr_in ip4addr;
	int socketcli;	

	sockfd = abrirSocketTCP();
	abrirBind(sockfd, 8080);
	abrirListen(sockfd);
	printf("ESPERANDO CLIENTE\n");
	socketcli=aceptar(sockfd, ip4addr);

	if((groupsig_get_code_from_str(&scheme, "CPY06")) == IERROR) {
		fprintf(stderr, "Error: Wrong scheme\n");
		return IERROR;
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
		return IERROR;
	}

	groupsig_init(time(NULL));

	if(!(grpkey = groupsig_grp_key_import(scheme, key_format, s_grpkey))) {
		fprintf(stderr, "Error: invalid group key %s.\n", s_grpkey);
		return IERROR;
	}

	if(!(memkey = groupsig_mem_key_import(scheme,	key_format, s_memkey))) {
		fprintf(stderr, "Error: invalid member key %s.\n", s_memkey);
		return IERROR;
	}
	int siglen = 0, msglen = 0, res = 0, i = 0;
	char* signture, *message;
	printf("1\n");
	size = signMsgGS(grpkey, memkey, scheme, msgstr, &sigstr);
	printf("2: %d\n", size);
	size = sigMsgToStrGS(msgstr, strlen(msgstr)+1, sigstr, size, &dest);
	escribir(socketcli, dest, size);
	
	/*printf("3\n");
	strToSigMsgGS(&message, &msglen, &signture, &siglen, dest, size);
	res = verifySignGS(signture, grpkey, message, scheme);
	printf("i: %d\tres: %d\n", i++, res);*/

	close(socketcli);
	close(sockfd);
	return 0;
}