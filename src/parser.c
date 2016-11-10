#include "../include/parser.h"


int parseNick(){
	return 0;
}
int parseMsg(char* src, char** nick, char ** msg){
	int msglen = 0;
	int nlen = 0;
	char *pch = NULL;
	char *pch2 = NULL;

	if(src == NULL || strlen(src) < 1 || nick == NULL || msg == NULL){
		return 0;
	}
	if((pch = strchr(src, ' ')) == NULL)
		return 0;
	pch++;
	if((pch2 = strchr(pch, ' ')) == NULL)
		return 0;

	nlen = pch2 - pch + 1;
	*nick =  malloc(nlen);
	memcpy(*nick, pch, nlen-1);
	(*nick)[nlen-1] = '\0';

	pch2++;
	msglen = strlen(pch2)+1;
	*msg = malloc(msglen);
	memcpy(*msg, pch2, msglen-1);
	(*msg)[msglen-1] = '\0';

	return 1;
}