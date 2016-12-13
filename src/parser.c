#include "../include/parser.h"


int parseNick(char* src, char** nick1, char ** nick2){
	char *pch1 = NULL, *pch2 = NULL;	
	*nick1 =  NULL;
	*nick2 =  NULL;
	
	if(src == NULL || strlen(src) < 1 || nick1 == NULL || nick2 == NULL){
		return -1;
	}
	if((pch1 = strchr(src, ' ')) == NULL)
		return -1;
	pch1++;

	if((pch2 = strchr(pch1, ' ')) == NULL){
		*nick1 = malloc(strlen(pch1) + 1);
		memcpy(*nick1, pch1, strlen(pch1) + 1);
	}
	else{
		*nick1 = malloc(pch2 - pch1 +1);
		memcpy(*nick1, pch1, pch2 - pch1);
		(*nick1)[pch2 - pch1] = '\0';
	}
	if(pch2 == NULL)
		return 0;
	pch2 ++;
	*nick2 = malloc(strlen(pch2) + 1);
	memcpy(*nick2, pch2, strlen(pch2) + 1);
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
int iniBigBrother(char* fname){
	
	/*file* f = NULL;
	if(fname !=NULL){
		
	}*/
	banWords =  malloc(sizeof(char*));
	banWords[0] = malloc(strlen("cara anchoa")+1);
	strcpy(banWords[0], "cara anchoa");
	nwords = 1;
}

int bigBrother(char* msg, int msglen){
	int i = 0;

	for (; i < nwords; i++){
		if(msglen < strlen(banWords[i]))
			continue;

		if(strstr(msg, banWords[i]) != NULL)
			return 1;
	}

	return 0;
}

void freeBigBrother(){
	int i = 0;
	for (; i < nwords; i++){
		free(banWords[i]);
	}
	free(banWords);
}

