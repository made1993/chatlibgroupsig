#include "../include/funcionesAES.h"

/*                        AES_KEY *key);
int AES_set_decrypt_key(const unsigned char *userKey, const int bits,
                        AES_KEY *key);




void AES_encrypt(const unsigned char *in, unsigned char *out,
                 const AES_KEY *key);
void AES_decrypt(const unsigned char *in, unsigned char *out,
                 const AES_KEY *key);

void AES_cbc_encrypt(const unsigned char *in, unsigned char *out,
                     size_t length, const AES_KEY *key,
                     unsigned char *ivec, const int enc);
*/

unsigned char* encrypt_cbc256(unsigned char* key, unsigned char* iv, const unsigned char* text){
	EVP_CIPHER_CTX* ctx = NULL;
	unsigned char* ret = NULL;
	int blk = 0, i = 0, tmp;

	ctx = (EVP_CIPHER_CTX*) malloc(sizeof(EVP_CIPHER_CTX));
	EVP_CIPHER_CTX_init(ctx);
	EVP_EncryptInit(ctx, EVP_aes_256_cbc(), key, iv);

	blk = strlen((char*) text) / TEXT_BLOCK;
	if(strlen((char*) text) % TEXT_BLOCK > 1)
		blk++;
	ret = malloc(sizeof(char) * (TEXT_BLOCK * blk));
	for (; i < blk; i++){
		EVP_EncryptUpdate(ctx, ret +(TEXT_BLOCK * i), &tmp, text +(TEXT_BLOCK * i), TEXT_BLOCK);
	}

	EVP_EncryptFinal(ctx, ret +(TEXT_BLOCK *blk), &tmp);

	return ret;
}


int main (int argc, char ** argv){
	unsigned char usr_key[] = "FkQCx$K9A:KWQo'P^/.6*qGRyXkRS";
	unsigned char msg[] = "hola esto esta cifrado en AES";
	unsigned char out1[8096], out2[8096];
	unsigned char ivec[] = "dontusethisinput";
	int out1n = 0, tmp, out2n = 0;
	EVP_CIPHER_CTX* x = (EVP_CIPHER_CTX*) malloc(sizeof(EVP_CIPHER_CTX));
	EVP_CIPHER_CTX_init(x);
//	enc_key = malloc(sizeof(AES_KEY));
//	dec_key = malloc(sizeof(AES_KEY));

/*	AES_set_encrypt_key((const unsigned char*) usr_key, 256, enc_key);
	AES_set_decrypt_key((const unsigned char*) usr_key, 256, dec_key);
*/
	EVP_EncryptInit(x, EVP_aes_256_cbc(), usr_key, ivec);


	EVP_EncryptUpdate(x, out1 + out1n, &tmp, msg + out1n, 64);
	out1n += tmp;
	printf("%d\n", out1n);



	printf("%d\t %s\n", out1n, out1);

	EVP_EncryptFinal(x, out1 + out1n, &tmp);

	printf("%d\t %s\n", out1n, out1);

	EVP_DecryptInit(x, EVP_aes_256_cbc(), usr_key, ivec);

		
	EVP_DecryptUpdate(x, out2 + out2n, &tmp, out1 + out2n, out1n);
	out2n += tmp;
	
	printf("%d\t %s\n", out2n, out2);

	EVP_DecryptFinal(x, out2, &tmp);

	printf("%d\t %s\n", out2n, out2);
	
	/*AES_encrypt((const unsigned char *) msg, (unsigned char *) out1, 
				enc_key);

	AES_decrypt((const unsigned char *) out1, (unsigned char *) out2, 
				dec_key);
	*/

	/*for(i = 0; i<strlen(msg); i++){
	}*/
	

//	free(enc_key);
//	free(dec_key);
	return 0;
}