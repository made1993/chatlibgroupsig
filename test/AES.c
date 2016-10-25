#include "../include/funcionesAES.h"

int main (int argc, char ** argv){
	int ciphlen = 0;
	unsigned char usr_key[] = "FkQCx$K9A:KWQo'P^/.6*qGRyXkRS";
	unsigned char msg[] = "0123456789012345678901234567890123456789012345678901234567890123456789";
	unsigned char ivec[] = "dontusethisinput";
	unsigned char* cipher = NULL;
	unsigned char* plain = NULL;
	EVP_CIPHER_CTX* ctx = NULL;

	ctx = create_ctx();



	ciphlen = encrypt_cbc256(ctx, usr_key, ivec, msg, &cipher, strlen((const char*)msg)+1);
	decrypt_cbc256(ctx, usr_key, ivec, cipher, &plain, ciphlen);

	printf("%s\n", plain);

	EVP_CIPHER_CTX_free(ctx);
	free(cipher);
	free(plain);
	return 1;
}