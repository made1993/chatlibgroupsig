#include "../include/funcionesAES.h"

/*                        AES_KEY *key);
int AES_set_decrypt_key(const unsigned char *userKey, const int bits,
                        AES_KEY *key);



void AES_cbc_encrypt(const unsigned char *in, unsigned char *out,
                     size_t length, const AES_KEY *key,
                     unsigned char *ivec, const int enc);

void AES_encrypt(const unsigned char *in, unsigned char *out,
                 const AES_KEY *key);
void AES_decrypt(const unsigned char *in, unsigned char *out,
                 const AES_KEY *key);

*/


int main (int argc, char ** argv){
	AES_KEY* enc_key,* dec_key;
	char usr_key[] = "FkQCx$K9A:KWQo'P^/.6*qGRyXkRS";
	char msg[] = "hola esto esta cifrado en AES";
	char out1[8096], out2[8096];
	enc_key = malloc(sizeof(AES_KEY));
	dec_key = malloc(sizeof(AES_KEY));

	AES_set_encrypt_key((const unsigned char*) usr_key, 256, enc_key);
	AES_set_decrypt_key((const unsigned char*) usr_key, 256, dec_key);
	
	AES_encrypt((const unsigned char *) msg, (unsigned char *) out1, 
				enc_key);

	AES_decrypt((const unsigned char *) out1, (unsigned char *) out2, 
				dec_key);

	free(enc_key);
	free(dec_key);
	return 0;
}