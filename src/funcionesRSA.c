#include "../include/funcionesRSA.h"



const char hn[] = "SHA256";

int generateKeysRSA(EVP_PKEY** privKey, EVP_PKEY** pubKey){
	RSA* rsa;
	*pubKey = EVP_PKEY_new();
	if(*pubKey == NULL){
		printf("ERR EVP_PKEY_new\n");
		return 0;
	}

	*privKey = EVP_PKEY_new();
	if(*privKey == NULL){
		printf("ERR EVP_PKEY_new\n");
		return 0;
	}
	
	rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
	
	if(rsa == NULL){
		printf("ERR RSA_generate_key\n");
		return 0;		
	}
	
	if(1 != EVP_PKEY_assign_RSA(*pubKey, RSAPublicKey_dup(rsa))){
		printf("ERR EVP_PKEY_assign_RSA\n");
		return 0;
	}
	
	if(1 != EVP_PKEY_assign_RSA(*privKey, RSAPrivateKey_dup(rsa))){
		printf("ERR EVP_PKEY_assign_RSA\n");
		return 0;
	}

	RSA_free(rsa);

	return 1;
}


int signMsgRSA(EVP_PKEY* key, const unsigned char* msg, unsigned char** sig, size_t* slen){
	EVP_MD_CTX* ctx = NULL;
	const EVP_MD* md = NULL;

	ctx = EVP_MD_CTX_create();
	md = EVP_get_digestbyname(hn);

	if(md == NULL){
		printf("ERR EVP_get_digestbyname\n");
		return 0;		
	}
	if(ctx == NULL){
		printf("ERR EVP_MD_CTX_create\n");
		return 0;		
	}

	if(1 != EVP_DigestInit_ex(ctx, md, NULL)){
		printf("ERR EVP_DigestInit_ex\n");
		return 0;
	}
	if(1 != EVP_DigestSignInit(ctx, NULL, md, NULL, key)){
		printf("ERR EVP_DigestSignInit\n");
		return 0;
	}

	/*SE FIRMA EL MENSAJE*/
	if(1 != EVP_DigestSignUpdate(ctx, msg, strlen((char*) msg))){
		printf("ERR EVP_DigestSignUpdate\n");
		return 0;
	}

	if(1 != EVP_DigestSignFinal(ctx, NULL, slen)){
		printf("ERR EVP_DigestSignFinal\n");
		return 0;	
	}
	*sig = OPENSSL_malloc(*slen);
	
	if(*sig == NULL){
		printf("ERR OPENSSL_malloc\n");
	}

	if(1 != EVP_DigestSignFinal(ctx, *sig, slen)){
		printf("ERR EVP_DigestSignFinal\n");
		return 0;
	}

	EVP_MD_CTX_destroy(ctx);
	ctx = NULL;

	return 1;

}
int verifySignRSA(EVP_PKEY* key, const unsigned char* sig, const unsigned char* msg, size_t slen){
	
	EVP_MD_CTX* ctx = NULL;
	const EVP_MD* md = NULL;

	if(!msg || !sig || !slen || !key) {
        assert(0);
        return -1;
    }

	ctx = EVP_MD_CTX_create();
	md = EVP_get_digestbyname(hn);

	if(md == NULL){
		printf("ERR EVP_get_digestbyname\n");
		return 0;		
	}
	if(ctx == NULL){
		printf("ERR EVP_MD_CTX_create\n");
		return 0;		
	}

	if(1 != EVP_DigestInit_ex(ctx, md, NULL)){
		printf("ERR EVP_DigestInit_ex\n");
		return 0;
	}

	if(1 != EVP_DigestVerifyInit(ctx, NULL, md, NULL, key)){
		printf("ERR EVP_DigestVerifyInit\n");
		return 0;
	}


	if(1 != EVP_DigestVerifyUpdate(ctx, msg, strlen((char*) msg))){
		printf("ERR EVP_DigestVerifyUpdate\n");
		return 0;
	}

	ERR_clear_error();

	

	return EVP_DigestVerifyFinal(ctx, sig, slen);
}

int reciveRSAsign(int sockfd, EVP_PKEY* pubKey, unsigned char** msg){
	unsigned char* sig = NULL, buff[8096];
	int size, moreMsg;

	/*SE OBTIENE LA FIRMA*/
	size = recibir(sockfd, (char*) buff);
	sig = malloc(sizeof(char) * 256);
	size += (1 + SHA256_SIGLEN);
	memcpy(sig, buff, 256);
	
	/*SE OBTIENE EL MENSAJE*/
	moreMsg = buff[256];
	msg = malloc(sizeof(unsigned char) * (size));
	memcpy(msg, &buff[SHA256_SIGLEN + 1], size);

	/*SE OBTIENEN SUCESICVAS PARTES DEL MENSAJE SI ES NECESARIO*/
	while(moreMsg){	
		size += recibir(sockfd, (char*) buff);
		size--;
		msg = realloc(msg, sizeof(unsigned char) * size);
		moreMsg = buff[0];
	}
	/*SE COMPRUEB LA FIRMA*/
	return verifySignRSA(pubKey, (const unsigned char*)sig, 
						(const unsigned char*)msg, SHA256_SIGLEN);

}


int sendRSAsign(int sockfd, EVP_PKEY* privKey, const unsigned char* msg, int msglen){
	unsigned char* buff = NULL;
	size_t slen = 0;
	int ret = 0;
	/*CONTROL DE ERRORES*/
	if(privKey == NULL || msg == NULL || msglen <= 0){
		return 0;
	}

	/*SE FIRMA EL MENSAJE*/
	if (1 != signMsgRSA(privKey, msg, &buff, &slen)){
		return 0;
	}

	/*SE CPMPRUEBA DE QUE TAMÑO ES EL MENSAJE Y SE ENVIA*/

	if(msglen > (MAX_MSG_LEN -1 - SHA256_SIGLEN)){
		buff = realloc(buff, MAX_MSG_LEN);
		buff[SHA256_SIGLEN] = MORE_MSG;
		memcpy (&buff[SHA256_SIGLEN + 1], msg, MAX_MSG_LEN -1 - SHA256_SIGLEN);
		msglen -= (MAX_MSG_LEN -1 - SHA256_SIGLEN);
	}
	else{
		buff = realloc(buff, SHA256_SIGLEN + msglen + 1);
		buff[SHA256_SIGLEN] = LAST_MSG;
		memcpy (&buff[SHA256_SIGLEN + 1], msg, msglen);
		msglen = 0;
	}
	if(MAX_MSG_LEN  < (msglen - SHA256_SIGLEN - 1))
		ret = escribir(sockfd, (char*) buff, MAX_MSG_LEN);
	else
		ret = escribir(sockfd, (char*) buff, msglen + SHA256_SIGLEN + 1);

	ret -= (SHA256_SIGLEN + 1);

	/*SI EL MENSAJE ES DEMASIADO LARGO SE ENVIA EL RESTO DE LAS PARTES*/
	while(msglen != 0){	

		if(msglen > (MAX_MSG_LEN -1 - SHA256_SIGLEN)){
			buff[0] = MORE_MSG;
			memcpy (&buff[1], &msg[ret], MAX_MSG_LEN -1);
			msglen -= (MAX_MSG_LEN -1);
		}
		else{
			buff = realloc(buff, msglen + 1);
			buff[1] = LAST_MSG;
			memcpy (&buff[+ 1], &msg[ret], msglen);
			msglen = 0;
		}
		if(MAX_MSG_LEN  < (msglen - SHA256_SIGLEN - 1))
			ret += escribir(sockfd, (char*) buff, MAX_MSG_LEN);
		else
			ret += escribir(sockfd, (char*) buff, msglen + SHA256_SIGLEN + 1);

		ret--;
 	}	
 	if (msglen == ret)
		return 1;
	return 0;	
}

int reciveRSAkey(int sockfd, EVP_PKEY** pubKey){
	char* buff = NULL;
	int size = 0;
	buff = (char*) malloc(sizeof(EVP_PKEY));
	while(size < sizeof(EVP_PKEY)){
		size += recibir(sockfd, &buff[size]);
	}

	*pubKey = (EVP_PKEY*) buff;
	return 1;
}
int sendRSAkey(int sockfd, EVP_PKEY* privKey){
	char* buff = (char*) privKey;
	int size = 0;
	while(size < sizeof(EVP_PKEY)){
		if(size + MAX_MSG_LEN > sizeof(EVP_PKEY))
			size += escribir(sockfd, &buff[size], MAX_MSG_LEN);
		else
			size += escribir(sockfd, &buff[size], sizeof(EVP_PKEY) - size);

		printf("%d\n", size);
	}
	return 1;
}

/*int main (){

	EVP_PKEY *privKey = NULL, *pubKey = NULL;
	unsigned char msg[] = "Estos esta firmado con rsa"; 
	unsigned char* sig = NULL;
	size_t slen;
	OpenSSL_add_all_algorithms();

	printf("Generar claves\n");
	generateKeysRSA(&privKey, &pubKey);

	printf("Firmar\n");
	signMsgRSA(privKey, msg, &sig, &slen);

	/**printf("FIRMA:");
	for(i=0; i < slen; ++i)
        printf("%02X", sig[i]);*//*
	printf("Verificar firma\n");
	printf("%d\n", verifySignRSA(pubKey, sig, msg, slen));
	return EXIT_SUCCESS;
}*/