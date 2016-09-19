#ifndef FUNCIONESDH_H

#define FUNCIONESDH_H

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/dh.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <stdio.h>

/**
*	Genera la clave publica y privada que se usara
*	una vez inicializados los parametros de dh.
*	
* 
**/
int  DHkey(DH *dh);

/**
*	Genera la clave publica y privada de dh con los
*	valores compartidos g y p.
*
* 	TODO:
*	revisar si existe una funcion o algun procedimiento
* 	que haga las cosas mas limpias de lo que estan.
*	El problema es que hay una ñapa curiosa para usar
*	los p y g que pasan como argumentos. 	
**/
DH* DHkeyFromParam(BIGNUM* p, BIGNUM* g);

/**
*	Genera los parametros de dh dada la longuitud
*	de la clave que se usara.
*
* 
**/

DH* DHparam(int keylen);
/**
*	Genera la clave compartida que se va a usar.
*
* 
**/

unsigned char* DHsharedKey(DH* dh, BIGNUM* pubkey);

#endif
