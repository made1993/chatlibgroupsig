#ifndef FUNCIONESDH_H

#define FUNCIONESDH_H

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/dh.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/dh.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <stdio.h>

/*
**	Usa  ua g y una p dada por generada con
**	anterioridad por openssl que se usaran en el 
**	protocolo de intercambio de clave.
*/
int getParamsIniDH(EVP_PKEY** params);

/*
**	Genere una g y una p que se usaran en el 
**	protocolo de intercambio de clave.
*/
int genNewParamsIniDH(EVP_PKEY** params, EVP_PKEY_CTX** pctx);

/*
**	Dados unos parametros genera una clave Diffie
**	Hellman.
**
*/
int genKeyFromParamsDH(EVP_PKEY_CTX** kctx, EVP_PKEY** dhkey, EVP_PKEY* params);

/*
**	Genera el secreto compartido de 2 claves dadas.
*/
unsigned  char* deriveSharedSecretDH(EVP_PKEY* privkey, EVP_PKEY* peerkey);


#endif
