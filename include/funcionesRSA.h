#ifndef FUNCIONESRSA_H
#define FUNCIONESRSA_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include "conexion.h"


#define SHA256_SIGLEN 256

const char hn[] = "SHA256";

/**
*	
*		
*	
*	
*	
*	
*	
*	
**/
int generateKeysRSA(EVP_PKEY** privKey, EVP_PKEY** pubKey);

/**
*	
*	
*	
*	
*	
*	
*	
*	
**/
int signMsgRSA(EVP_PKEY* key, const unsigned char* msg, unsigned char** sig, size_t* slen);

/**
*	
*	
*	
*	
*	
*	
*	
*	
**/
int verifySignRSA(EVP_PKEY* key, const unsigned char* sig, const unsigned char* msg, size_t slen);

/**
*	
*	
*	
*	
*	
*	
*	
*	
**/
int reciveRSAsign(int sockfd, EVP_PKEY* pubKey, unsigned char** msg);

/**
*	
*	
*	
*	
*	
*	
*	
*	
**/
int sendRSAsign(int sockfd, EVP_PKEY* privKey, const unsigned char* msg, int msglen);


/**
*	
*	
*	
*	
*	
*	
*	
*	
**/
int reciveRSAkey(int sockfd, EVP_PKEY* pubKey);

/**
*	
*	
*	
*	
*	
*	
*	
*	
**/
int sendRSAKEY(int sockfd, EVP_PKEY* privKey);

#endif