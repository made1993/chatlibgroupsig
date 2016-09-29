#ifndef FUNCIONESAES_H
#define FUNCIONESAES_H

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <stdio.h>


#define TEXT_BLOCK 64



EVP_CIPHER_CTX* create_ctx();


void encrypt_cbc256(EVP_CIPHER_CTX* ctx, unsigned char* key, unsigned char* iv, const unsigned char* text, unsigned char* out);


void decrypt_cbc256(EVP_CIPHER_CTX* ctx, unsigned char* key, unsigned char* iv, const unsigned char* text, unsigned char* out);

#endif