
#include "../include/funcionesDH.h"


int getParamsIniDH(EVP_PKEY** params){
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

int main(int argc, char** argv){
	EVP_PKEY* params,* dhkey1,* dhkey2,* pubKey;
	EVP_PKEY_CTX* kctx1,* kctx2;
	unsigned char *skey;
	char* buff;
	int bufflen;

	/* Use built-in parameters */
	if(NULL == (params = EVP_PKEY_new())) return 0;
	if(NULL == (dhkey1 = EVP_PKEY_new())) return 0;
	if(NULL == (dhkey2 = EVP_PKEY_new())) return 0;
	if(NULL == (pubKey = EVP_PKEY_new())) return 0;

	/*Utiliza parametros ya inicializodos*/
	getParamsIniDH(&params);
	
	/*Genera parametros nuevos*/
	//genNewParamsIniDH(&params, &pctx);
	
	/*generate key 1*/
	genKeyFromParamsDH(&kctx1,&dhkey1, params);

	/*generate key 2*/
	genKeyFromParamsDH(&kctx2,&dhkey2, params);

	
	
	skey = deriveSharedSecretDH(dhkey1, dhkey2);
	if (skey == NULL)
		return 0;
	BIO_dump_fp(stdout, (const char*) skey, 256);
	printf("\n");

	OPENSSL_free(skey);

	bufflen = i2d_PublicKey(dhkey1, (unsigned char **)&buff);

	d2i_PublicKey(EVP_PKEY_DH ,&pubKey, (const unsigned char**) &buff, bufflen);

	skey = deriveSharedSecretDH(dhkey2, pubKey);	

	BIO_dump_fp(stdout, (const char*) skey, 256);

	OPENSSL_free(skey);

	EVP_PKEY_CTX_free(kctx1);
	EVP_PKEY_CTX_free(kctx2);
	EVP_PKEY_free(dhkey1);
	EVP_PKEY_free(dhkey2);
	EVP_PKEY_free(params);


	
}


