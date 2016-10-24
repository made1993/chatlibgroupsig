
#include "../include/funcionesDH.h"

#include <openssl/dh.h>
#include <openssl/evp.h>


int getParamsIniDH(EVP_PKEY** params){
	if(params == NULL) return 0;

	*params = NULL;	
	if(NULL == (*params = EVP_PKEY_new())) return 0;	 

	if(1 != EVP_PKEY_set1_DH(*params,DH_get_2048_256())) return 0;
	return 1;
}

int genNewParamsIniDH(EVP_PKEY** params, EVP_PKEY_CTX** pctx){
	*pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_DH, NULL);
	if(!*pctx){
		return 0;
	}

	if(1 != EVP_PKEY_paramgen_init(*pctx)){
		EVP_PKEY_CTX_free(*pctx);
		return 0;
	}

	if(1 != EVP_PKEY_CTX_set_dh_paramgen_prime_len(*pctx, 2048)){
		EVP_PKEY_CTX_free(*pctx);
		return 0;
	}

	if(1 != EVP_PKEY_paramgen(*pctx, params)){
		EVP_PKEY_CTX_free(*pctx);
		return 0;
	}

	return 1;
}

int genKeyFromParamsDH(EVP_PKEY_CTX** kctx, EVP_PKEY** dhkey, EVP_PKEY* params){
	
	if(kctx == NULL || dhkey == NULL || params == NULL)
	
	*dhkey = NULL;
	if(NULL == (*dhkey = EVP_PKEY_new())) return 0;
	
	/* Create context*/
	if(!(*kctx = EVP_PKEY_CTX_new(params, NULL))){
		return 0;
	}

	/* Generate a new key */
	if(1 != EVP_PKEY_keygen_init(*kctx)){
		EVP_PKEY_CTX_free(*kctx);
		return 0;
	}
	
	if(1 != EVP_PKEY_keygen(*kctx, dhkey)){
		EVP_PKEY_CTX_free(*kctx);
		return 0;
	}

	return 1;
}
unsigned  char* deriveSharedSecretDH(EVP_PKEY* privkey, EVP_PKEY* peerkey){
	unsigned char* skey = NULL;
	EVP_PKEY_CTX* ctx = NULL;
	size_t skeylen;
	ctx = EVP_PKEY_CTX_new(privkey, NULL);
	printf("1\n");
	if (!ctx){
		return NULL;
	}
	printf("2\n");
	
	if(EVP_PKEY_derive_init(ctx) <= 0){
		EVP_PKEY_CTX_free(ctx);
		return NULL;
	}
	printf("3\n");
	
	if (EVP_PKEY_derive_set_peer(ctx, peerkey) <= 0){
		EVP_PKEY_CTX_free(ctx);
		return NULL;
	}
	printf("4\n");

	/* Determine buffer length */
	if (EVP_PKEY_derive(ctx, NULL, &skeylen) <= 0){
		EVP_PKEY_CTX_free(ctx);
		return NULL;
	}
	printf("5\n");

	skey = OPENSSL_malloc(skeylen);

	if (!skey){
		EVP_PKEY_CTX_free(ctx);
		return NULL;
	}
	printf("6\n");

	if (EVP_PKEY_derive(ctx, skey, &skeylen) <= 0){
		OPENSSL_free(skey);
		EVP_PKEY_CTX_free(ctx);
		return NULL;
	}
	return skey;
}

int DHpubKeyToMsg(EVP_PKEY* pubKey, char ** msg){
	if(pubKey == NULL || msg == NULL) return 0;
	*msg =  NULL;
	return i2d_PUBKEY(pubKey, (unsigned char**) msg);
}



int msgToDHpubKey(EVP_PKEY** pubKey, char * msg, int msglen){
	if(pubKey == NULL || msg == NULL || msglen < 1) return 0;

	*pubKey = EVP_PKEY_new();
	if(*pubKey == NULL){
		printf("ERR EVP_PKEY_new\n");
		return 0;
	}
	d2i_PUBKEY(pubKey, (const unsigned char**) &msg, msglen);
	return 1;
}

int main(int argc, char** argv){
	EVP_PKEY* params,* dhkey1,* dhkey2,* pubKey;
	EVP_PKEY_CTX* kctx1,* kctx2,* pctx;
	DH* dhaux = NULL;
	unsigned char *skey;
	char* buff= NULL;
	int bufflen;



	/*Utiliza parametros ya inicializodos*/
	getParamsIniDH(&params);
	
	
	/*Genera parametros nuevos*/
	//genNewParamsIniDH(&params, &pctx);
	
	/*generate key 1*/
	genKeyFromParamsDH(&kctx1,&dhkey1, params);

	/*generate key 2*/
	genKeyFromParamsDH(&kctx2,&dhkey2, params);

	/*bufflen = i2d_DHxparams(EVP_PKEY_get1_DH(dhkey1), (unsigned char**)&buff);
	*/
	bufflen = DHpubKeyToMsg(dhkey1, &buff);
	printf("%d\n", bufflen);

	pubKey = NULL;
	msgToDHpubKey(&pubKey, buff, bufflen);

	skey = deriveSharedSecretDH(dhkey2, pubKey);

	BIO_dump_fp(stdout, (const char*) skey, 256);


	OPENSSL_free(skey);

	bufflen = DHpubKeyToMsg(dhkey2, &buff);
	printf("%d\n", bufflen);

	pubKey = NULL;
	msgToDHpubKey(&pubKey, buff, bufflen);

	skey = deriveSharedSecretDH(dhkey1, pubKey);

	BIO_dump_fp(stdout, (const char*) skey, 256);


/*	if(EVP_PKEY_assign_DH(pubKey,dhaux)){
		printf("asdas\n");
	 	return 0;
	}

	skey = deriveSharedSecretDH(dhkey2, pubKey);	


	OPENSSL_free(skey);

	EVP_PKEY_CTX_free(kctx1);
	EVP_PKEY_CTX_free(kctx2);
	EVP_PKEY_free(dhkey1);
	EVP_PKEY_free(dhkey2);
	EVP_PKEY_free(params);*/
	
}


