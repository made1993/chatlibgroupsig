#include "../include/funcionesDH.h"


DH* generarDH(){
	DH *ent1;
	int codes;
	int secret_size;

	/* Generate the parameters to be used */
	if(NULL == (ent1 = DH_new())) 
		return NULL;
	if(1 != DH_generate_parameters_ex(ent1, 2048, DH_GENERATOR_2, NULL)) 
		return 0;

	if(1 != DH_check(ent1, &codes)) 
		return NULL;

	if(codes != 0)
	{
	    /* Problems have been found with the generated parameters */
	    /* Handle these here - we'll just abort for this example */
	    printf("DH_check failed\n");
	    abort();
	}

	/* Generate the public and private key pair */
	if(1 != DH_generate_key(ent1)) 
		return NULL;

	return ent1;
}
int main(){
	DH *ent1;
	DH *ent2;
	int codes;
	int secret_size;

	/* Generate the parameters to be used */
	if(NULL == (ent1 = DH_new())) return 0;
	if(1 != DH_generate_parameters_ex(ent1, 512, DH_GENERATOR_2, NULL)) return 0;

	if(1 != DH_check(ent1, &codes)) return 0;
	if(codes != 0)
	{
	    /* Problems have been found with the generated parameters */
	    /* Handle these here - we'll just abort for this example */
	    printf("DH_check failed\n");
	    abort();
	}

	/* Generate the public and private key pair */
	if(1 != DH_generate_key(ent1)) return 0;


	/* Generate the parameters to be used */
	if(NULL == (ent2 = DH_new())) return 0;
	if(1 != DH_generate_parameters_ex(ent2, 512, DH_GENERATOR_2, NULL)) return 0;

	if(1 != DH_check(ent2, &codes)) return 0;
	if(codes != 0)
	{
	    /* Problems have been found with the generated parameters */
	    /* Handle these here - we'll just abort for this example */
	    printf("DH_check failed\n");
	    abort();
	}

	/* Generate the public and private key pair */
	if(1 != DH_generate_key(ent2)) return 0;

	/* Send the public key to the peer.
	 * How this occurs will be specific to your situation (see main text below) */


	/* Receive the public key from the peer. In this example we're just hard coding a value */
	BIGNUM *pubkey = ent2->pub_key;

	/* Compute the shared secret */
	unsigned char *secret;
	if(NULL == (secret = OPENSSL_malloc(sizeof(unsigned char) * (DH_size(ent1))))) return 0;

	if(0 > (secret_size = DH_compute_key(secret, pubkey, ent1))) return 0;

	/* Do something with the shared secret */
	/* Note secret_size may be less than DH_size(ent1) */
	printf("The shared secret is:\n");
	BIO_dump_fp(stdout, (const char *) secret, secret_size);

	/* Clean up */
	OPENSSL_free(secret);

	pubkey = ent1->pub_key;
	/* Compute the shared secret */
	if(NULL == (secret = OPENSSL_malloc(sizeof(unsigned char) * (DH_size(ent2))))) return 0;

	if(0 > (secret_size = DH_compute_key(secret,pubkey, ent2))) return 0;

	/* Do something with the shared secret */
	/* Note secret_size may be less than DH_size(ent1) */
	printf("The shared secret is:\n");
	BIO_dump_fp(stdout, secret, secret_size);
	DH_free(ent1);
	DH_free(ent2);




}