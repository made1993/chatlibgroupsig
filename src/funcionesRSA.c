#include "../include/funcionesRSA.h"



const char hn[] = "SHA256";

int generateKeysRSA(EVP_PKEY** privKey, RSA** rsaPub){
	RSA* rsa =  NULL;
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
	
	if(1 != EVP_PKEY_assign_RSA(*privKey, RSAPrivateKey_dup(rsa))){
		printf("ERR EVP_PKEY_assign_RSA\n");
		return 0;
	}

	*rsaPub = RSAPublicKey_dup(rsa);
	return 1;
}


int signMsgRSA(EVP_PKEY* key, const unsigned char* msg, unsigned char** sig, size_t* slen, size_t msglen){
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
	if(1 != EVP_DigestSignUpdate(ctx, msg, msglen)){
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
int verifySignRSA(EVP_PKEY* key, const unsigned char* sig, const unsigned char* msg, size_t slen, size_t msglen){
	
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


	if(1 != EVP_DigestVerifyUpdate(ctx, msg, msglen)){
		printf("ERR EVP_DigestVerifyUpdate\n");
		return 0;
	}

	ERR_clear_error();

	

	return EVP_DigestVerifyFinal(ctx, sig, slen);
}

int reciveRSAsign(int sockfd, EVP_PKEY* pubKey, unsigned char** msg){
	char* buff;
	unsigned char* sig = NULL,* auxMsg;
	int msglen = 0;
	if(sockfd <1 || pubKey == NULL || msg == NULL)
		return 0;
	msglen = recibir(sockfd, &buff);

	if(msglen == -1)
		return 0;	

	sig = malloc(sizeof(char) * SHA256_SIGLEN);
	auxMsg =  malloc(sizeof(char) * (msglen - SHA256_SIGLEN));

	if(*msg == NULL || sig == NULL)
		return 0;
	
	memcpy(sig, buff, sizeof(char) * SHA256_SIGLEN);
	memcpy(auxMsg, &buff[SHA256_SIGLEN], msglen - SHA256_SIGLEN);
	
	*msg = auxMsg;

	return verifySignRSA(pubKey, (const unsigned char*)sig, 
						(const unsigned char*) auxMsg,  SHA256_SIGLEN, msglen - SHA256_SIGLEN);
}


int sendRSAsign(int sockfd, EVP_PKEY* privKey, const unsigned char* msg, int msglen){
	unsigned char* buff = NULL;
	size_t slen = 0;
	/*CONTROL DE ERRORES*/
	if(privKey == NULL || msg == NULL || msglen <= 0){
		return 0;
	}

	/*SE FIRMA EL MENSAJE*/
	if (1 != signMsgRSA(privKey, msg, &buff, &slen, msglen)){
		return 0;
	}

	buff = realloc(buff, sizeof(char) * (msglen + slen));

	memcpy(&buff[slen], msg, msglen);
	
	if(-1 == escribir(sockfd, (char*)buff, slen + msglen)){
		return 0;
	}

	return 1;
}

int reciveRSAkey(int sockfd, EVP_PKEY** pubKey){
	RSA * rsaPub = NULL;
	char* buffKey;
	int keylen = 0;

	if(sockfd <= 0 || pubKey== NULL || *pubKey == NULL)
		return 0;

	keylen = recibir(sockfd, &buffKey);
	if (keylen == -1)
		return 0;
	rsaPub = d2i_RSAPublicKey(&rsaPub, (const unsigned char**) &buffKey, keylen);
	*pubKey = EVP_PKEY_new();
	if(*pubKey == NULL){
		printf("ERR EVP_PKEY_new\n");
		return 0;
	}
	if(1 != EVP_PKEY_assign_RSA(*pubKey, rsaPub)){
		printf("ERR EVP_PKEY_assign_RSA\n");
		return 0;
	}
	return 1;
}


int sendRSAkey(int sockfd, RSA* rsaPub){
	char* buffKey = NULL;
	int bufflen = 0;
	
	bufflen = i2d_RSAPublicKey(rsaPub, (unsigned char **)&buffKey);

	escribir(sockfd, buffKey, bufflen);
	free(buffKey);
	return 1;
}

