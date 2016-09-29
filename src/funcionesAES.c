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


EVP_CIPHER_CTX* create_ctx(){
	EVP_CIPHER_CTX* ctx = NULL;
	ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(ctx);
	
	return ctx;
}



void encrypt_cbc256(EVP_CIPHER_CTX* ctx, unsigned char* key, unsigned char* iv, const unsigned char* text, unsigned char* out){
	int blk = 0, i = 0, tmp = 0;
	
	EVP_EncryptInit(ctx, EVP_aes_256_cbc(), key, iv);
	blk = strlen((char*) text) / TEXT_BLOCK;
	if(strlen((char*) text) % TEXT_BLOCK > 1)
		blk++;
	for (; i < blk; i++){
		EVP_EncryptUpdate(ctx, out +(TEXT_BLOCK * i), &tmp, text +(TEXT_BLOCK * i), TEXT_BLOCK);
	}

	EVP_EncryptFinal_ex(ctx, out +(TEXT_BLOCK *blk), &tmp);
	
	return;
}
void decrypt_cbc256(EVP_CIPHER_CTX* ctx, unsigned char* key, unsigned char* iv, const unsigned char* text, unsigned char* out){
	int tmp;
	EVP_DecryptInit(ctx, EVP_aes_256_cbc(), key, iv);
	EVP_DecryptUpdate(ctx, out, &tmp, text, 64);
	EVP_DecryptFinal_ex(ctx, out, &tmp);
	
	return;

}

int main (int argc, char ** argv){
	int i = 0;
	unsigned char usr_key[] = "FkQCx$K9A:KWQo'P^/.6*qGRyXkRS";
	unsigned char msg[] = "hola esto esta cifrado en AES";
	unsigned char ivec[] = "dontusethisinput";
	unsigned char* cipher;
	unsigned char* plain;
	EVP_CIPHER_CTX* ctx;
	ctx = create_ctx();
	
	cipher = malloc(sizeof(unsigned char) * 8096);
	plain = malloc(sizeof(unsigned char) * 8096);



	for( i = 0; i< 100; i++){
		encrypt_cbc256(ctx, usr_key, ivec, msg, cipher);
		EVP_CIPHER_CTX_cleanup(ctx);
		decrypt_cbc256(ctx, usr_key, ivec, cipher, plain);
		EVP_CIPHER_CTX_cleanup(ctx);
	}
	EVP_CIPHER_CTX_free(ctx);
	free(cipher);
	free(plain);
	return 1;

}
/*
int main (int argc, char ** argv){
	unsigned char usr_key[] = "FkQCx$K9A:KWQo'P^/.6*qGRyXkRS";
	unsigned char msg[] = "hola esto esta cifrado en AES";
	unsigned char out1[8096], out2[8096];
	unsigned char ivec[] = "dontusethisinput";
	int out1n = 0, tmp, out2n = 0;
	EVP_CIPHER_CTX* x = (EVP_CIPHER_CTX*) malloc(sizeof(EVP_CIPHER_CTX));
	EVP_CIPHER_CTX_init(x);

	

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
	
	return 0;
}*/