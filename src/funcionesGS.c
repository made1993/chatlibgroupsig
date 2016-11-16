#include "../include/funcionesGS.h"

int signMsgGS(groupsig_key_t* grpkey, groupsig_key_t* memkey, uint8_t scheme,
				char *msgstr, char** sigstr){
	message_t* msg = NULL, *sigmsg =  NULL;
	groupsig_signature_t* sig = NULL;
	int size;
	if(grpkey == NULL || memkey == NULL || msgstr == NULL || sigstr == NULL){
		fprintf(stderr, "Error: failed to sign a message.\n");
		return IERROR;
	}
	*sigstr = NULL;
	msg = message_from_string(msgstr);
	if(msg == NULL){
		fprintf(stderr, "Error: failed to initialize the message object.\n");
		return IERROR;
	}

	if(!(sig = groupsig_signature_init(scheme))) {
		fprintf(stderr, "Error: failed to initialize the group signature object.\n");
		return IERROR;
	}

	if(groupsig_sign(sig, msg, memkey, grpkey, UINT_MAX) == IERROR){
		fprintf(stderr, "Error: signing failure.\n");
		return IERROR;
	}


	
	sigmsg = message_init();
	groupsig_signature_export(sig, GROUPSIG_SIGNATURE_FORMAT_MESSAGE_NULL, sigmsg);	
	groupsig_signature_free(sig);

	*sigstr = malloc(sigmsg->length + sizeof(uint64_t));
	memcpy(*sigstr, &sigmsg->length, sizeof(uint64_t));
	memcpy((*sigstr) + sizeof(uint64_t), sigmsg->bytes, sigmsg->length);
	/*TODO quiza haya que meter el esquema que se usa*/
	
	size = (int)sigmsg->length + sizeof(uint64_t);
	message_free(msg);
	message_free(sigmsg);
	return size;
}

int verifySignGS(char* sigstr, groupsig_key_t *grpkey, char* msgstr, uint8_t scheme){

	uint8_t bool = 0;
	groupsig_signature_t* sig = NULL;
	message_t *msg = NULL, *sigmsg = NULL;

	if(sigstr == NULL || grpkey == NULL || msgstr == NULL){
		fprintf(stderr, "Error: NULL input.\n");
		return IERROR;
	}
	
	sigmsg =  message_init();	
	memcpy(&sigmsg->length, sigstr, sizeof(uint64_t));
	sigmsg->bytes = malloc(sigmsg->length);
	memcpy(sigmsg->bytes, sigstr + sizeof(uint64_t), sigmsg->length);

	if((sig = groupsig_signature_import(scheme, GROUPSIG_SIGNATURE_FORMAT_MESSAGE_NULL, sigmsg)) ==  NULL){
		printf("Error: failed to import the signature.\n" );
		return IERROR;	
	}

	msg = message_from_string(msgstr);
	if(msg == NULL){
		fprintf(stderr, "Error: failed to initialize the message object.\n");
		return IERROR;
	}
	if(groupsig_verify(&bool, sig, msg, grpkey) == IERROR) {
		fprintf(stderr, "Error: verification failure.\n");
		return IERROR;
	}
	message_free(msg);
	message_free(sigmsg);
	groupsig_signature_free(sig);
	return bool;
}

int sigMsgToStrGS(char * msgstr, int msglen, char* sigstr, int siglen, char** dst){
	int size = -1;
	if(msgstr == NULL || msglen < 1 || sigstr == NULL || siglen < 1 || dst == NULL){
		fprintf(stderr, "Error: NULL input.\n");
		return IERROR;
	}
	*dst = NULL;
	size = msglen + siglen + sizeof(int)*2;
	*dst = malloc (size);


	memcpy(*dst, &siglen, sizeof(int));
	memcpy((*dst) + sizeof(int), sigstr, siglen);

	memcpy((*dst) + sizeof(int) + siglen, &msglen, sizeof(int));
	memcpy((*dst) + sizeof(int)*2 + siglen, msgstr, msglen);

	return size;
}

int strToSigMsgGS(char** msgstr, int *msglen, char** sigstr, int* siglen, char* src, int srclen){

	if(msgstr == NULL || msglen == NULL || sigstr == NULL || siglen == NULL || src == NULL){
		fprintf(stdout, "Error: NULL input.\n");
		return IERROR;
	}
	memcpy(siglen, src, sizeof(int));
	if(*siglen > srclen - sizeof(int)){
		fprintf(stdout, "Error: ivalid size of signature %d\n", *siglen);
		return IERROR;
	}
	*sigstr = malloc(*siglen);
	memcpy(*sigstr, src + sizeof(int), *siglen);

	memcpy(msglen, src + sizeof(int) + *siglen, sizeof(int));
	if(*msglen > srclen - *siglen - sizeof(int)*2){
		fprintf(stdout, "Error: ivalid size of message %d\n", *msglen);
		return IERROR;
	}
	*msgstr = malloc(*msglen);
	memcpy(*msgstr, src + sizeof(int)*2 + *siglen, *msglen);
	
	return IOK;
}

