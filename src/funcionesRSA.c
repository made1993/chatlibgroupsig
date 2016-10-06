#include "../include/funcionesRSA.h"

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
	int i = 0;

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

int reciveRSASign(EVP_PKEY* privKey, const unsigned char* buff){

}


int sendRSASign(int sockfd, EVP_PKEY* privKey, const unsigned char* msg){
	const unsigned char* sig = NULL;
	size_t slen = 0;
	if(privKey == NULL){
		return 0;
	}

	if (1 != signMsgRSA(privKey, msg, &sig, &slen)){
		return 0;
 	}

 	escribir();


}

int main (){

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
        printf("%02X", sig[i]);*/
	printf("Verificar firma\n");
	printf("%d\n", verifySignRSA(pubKey, sig, msg, slen));
	return EXIT_SUCCESS;
}