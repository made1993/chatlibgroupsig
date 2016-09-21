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
	AES_KEY* key;
	char usr_key[] = "FkQCx$K9A:KWQo'P^/.6*qGRyXkRS";
	char msg[] = "esto esta cifrado con AES 256 CBC";
	char out1[8096], out2[8096];
	key = malloc(sizeof(AES_KEY));

	AES_set_encrypt_key((const unsigned char*) usr_key, 256, key);
	
	AES_encrypt((const unsigned char *) msg, (unsigned char *) out1, 
				key);
	printf("%s\n", out1);

	AES_decrypt((const unsigned char *) out1, (unsigned char *) out2, 
				key);
	printf("%s\n", out2);

	return 0;
}