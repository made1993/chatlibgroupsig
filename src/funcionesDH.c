#include "../include/funcionesDH.h"


int DHkey(DH *dh){
	if(1 != DH_generate_key(dh)) 
		return 0;

	return 1;
}

DH* DHkeyFromParam(BIGNUM* p, BIGNUM* g){
	DH *dh = NULL;


	if(NULL == (dh = DH_new())) 
		return NULL;

	dh->p = malloc(sizeof(BIGNUM));
	dh->g = malloc(sizeof(BIGNUM));
	*dh->p = *p;
	*dh->g = *g;

	if(1 != DHkey(dh))
		return NULL;
	return dh;
}

DH* DHparam(int keylen){
	DH *dh = NULL;
	int codes = 0;
	if(NULL == (dh = DH_new())) 
		return NULL;
	if(1 != DH_generate_parameters_ex(dh, keylen, DH_GENERATOR_2, NULL)) 
		return 0;

	if(1 != DH_check(dh, &codes)) 
		return NULL;
	if(codes != 0){
	    abort();
	}
	return dh;
}

unsigned char* DHsharedKey(DH* dh, BIGNUM* pubkey){
	unsigned char *secret;
	if(NULL == (secret = OPENSSL_malloc(sizeof(unsigned char) * (DH_size(dh))))) 
		return NULL;

	if(0 > DH_compute_key(secret, pubkey, dh)) 
		return NULL;
	return secret;
}

int main(){
	DH *dh1;
	DH *dh2;
	int keylen = 2048;
	unsigned char *secret;

	dh1 = DHparam(keylen);
	
	DHkey(dh1);

	if (dh1 == NULL){
		return 1;
	}
	dh2 = DHkeyFromParam(dh1->p, dh1->g);
	DHkey(dh2);	
	if (dh2 == NULL){
		return 1;
	}


	/* Send the public key to the peer.
	 * How this occurs will be specific to your situation (see main text below) */


	/* Receive the public key from the peer. In this example we're just hard coding a value */
	BIGNUM *pubkey = dh2->pub_key;

	/* Compute the shared secret */
	
	secret = DHsharedKey(dh1, pubkey);
	/* Do something with the shared secret */
	/* Note secret_size may be less than DH_size(dh1) */
	printf("The shared secret is:\n");
	BIO_dump_fp(stdout, (const char *) secret, 256);

	/* Clean up */
	OPENSSL_free(secret);

	pubkey = dh1->pub_key;
	/* Compute the shared secret */

	secret = DHsharedKey(dh2, pubkey);

	/* Do something with the shared secret */
	/* Note secret_size may be less than DH_size(dh1) */
	printf("The shared secret is:\n");
	BIO_dump_fp(stdout, (const char*) secret, 256);

	DH_free(dh1);
	DH_free(dh2);
	

	return 0;

}