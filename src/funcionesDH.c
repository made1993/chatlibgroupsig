#include "../include/funcionesDH.h"


DH* generarDH(int keylen){
	DH *dh;
	int codes;

	/* Generate the parameters to be used */
	if(NULL == (dh = DH_new())) 
		return NULL;
	if(1 != DH_generate_parameters_ex(dh, keylen, DH_GENERATOR_2, NULL)) 
		return 0;

	if(1 != DH_check(dh, &codes)) 
		return NULL;

	if(codes != 0)
	{
	    /* Problems have been found with the generated parameters */
	    /* Handle these here - we'll just abort for this example */
	    printf("DH_check failed\n");
	    abort();
	}

	/* Generate the public and private key pair */
	if(1 != DH_generate_key(dh)) 
		return NULL;

	return dh;
}
DH* generarDHparam(unsigned char * key, BIGNUM *pub_key, BIGNUM* g, BIGNUM* p){
	DH *dh = NULL;
	int codes;
	if(NULL == (dh = DH_new())) 
		return NULL;
	dh->g = g;
	dh->p = p;
	codes=DH_compute_key(key, pub_key, dh);
	if(codes == -1){
		DH_free(dh);
		return NULL;
	}
	printf("%d\n", codes);
	return dh;
}
int main(){
	DH *dh1;
	DH *dh2;
	int secret_size;
	int keylen = 512;
	unsigned char * key = NULL; 
	dh1 = generarDH(keylen);
	key = malloc(sizeof(unsigned char) * (keylen + 1));
	if (dh1 == NULL){
		printf("ERROR dh1\n");
		return 1;
	}
	dh2 = generarDHparam(key, dh1->pub_key, dh1->g, dh1->p);
	if (dh2 == NULL){
		printf("ERROR dh2\n");
		return 1;
	}

	/* Send the public key to the peer.
	 * How this occurs will be specific to your situation (see main text below) */


	/* Receive the public key from the peer. In this example we're just hard coding a value */
	BIGNUM *pubkey = dh2->pub_key;

	/* Compute the shared secret */
	unsigned char *secret;
	if(NULL == (secret = OPENSSL_malloc(sizeof(unsigned char) * (DH_size(dh1))))) return 0;

	if(0 > (secret_size = DH_compute_key(secret, pubkey, dh1))) return 0;

	/* Do something with the shared secret */
	/* Note secret_size may be less than DH_size(dh1) */
	printf("The shared secret is:\n");
	BIO_dump_fp(stdout, (const char *) secret, secret_size);

	/* Clean up */
	OPENSSL_free(secret);

	pubkey = dh1->pub_key;
	/* Compute the shared secret */
	if(NULL == (secret = OPENSSL_malloc(sizeof(unsigned char) * (DH_size(dh2))))) return 0;

	if(0 > (secret_size = DH_compute_key(secret,pubkey, dh2))) return 0;

	/* Do something with the shared secret */
	/* Note secret_size may be less than DH_size(dh1) */
	printf("The shared secret is:\n");
	BIO_dump_fp(stdout, (const char*) secret, secret_size);
	printf("secret size:%d\n", secret_size);
	DH_free(dh1);
	DH_free(dh2);




}