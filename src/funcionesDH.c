
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

	if (!ctx){
		return NULL;
	}
	
	if(EVP_PKEY_derive_init(ctx) <= 0){
		EVP_PKEY_CTX_free(ctx);
		return NULL;
	}
	
	if (EVP_PKEY_derive_set_peer(ctx, peerkey) <= 0){
		EVP_PKEY_CTX_free(ctx);
		return NULL;
	}

	/* Determine buffer length */
	if (EVP_PKEY_derive(ctx, NULL, &skeylen) <= 0){
		EVP_PKEY_CTX_free(ctx);
		return NULL;
	}

	skey = OPENSSL_malloc(skeylen);

	if (!skey){
		EVP_PKEY_CTX_free(ctx);
		return NULL;
	}
	if (EVP_PKEY_derive(ctx, skey, &skeylen) <= 0){
		OPENSSL_free(skey);
		EVP_PKEY_CTX_free(ctx);
		return NULL;
	}
	return skey;
}

int main(int argc, char** argv){
	EVP_PKEY* params,* dhkey1,* dhkey2;
	EVP_PKEY_CTX* kctx1,* kctx2;
	unsigned char *skey;

	/* Use built-in parameters */
	if(NULL == (params = EVP_PKEY_new())) return 0;
	if(NULL == (dhkey1 = EVP_PKEY_new())) return 0;
	if(NULL == (dhkey2 = EVP_PKEY_new())) return 0;

	/*Utiliza parametros ya inicializodos*/
	getParamsIniDH(&params);
	
	/*Genera parametros nuevos*/
	//genNewParamsIniDH(&params, &pctx);
	
	/*generate key 1*/
	genKeyFromParamsDH(&kctx1,&dhkey1, params);

	/*generate key 2*/
	genKeyFromParamsDH(&kctx2,&dhkey2, dhkey1);


	
	
	skey = deriveSharedSecretDH(dhkey1, dhkey2);

	BIO_dump_fp(stdout, (const char*) skey, 256);
	printf("\n");

	OPENSSL_free(skey);

	skey = deriveSharedSecretDH(dhkey2, dhkey1);	

	BIO_dump_fp(stdout, (const char*) skey, 256);

	OPENSSL_free(skey);

	EVP_PKEY_CTX_free(kctx1);
	EVP_PKEY_CTX_free(kctx2);
	EVP_PKEY_free(dhkey1);
	EVP_PKEY_free(dhkey2);
	EVP_PKEY_free(params);


	
}


