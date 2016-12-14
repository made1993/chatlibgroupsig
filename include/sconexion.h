#ifndef SCONEXION_H
#define SCONEXION_H
#include "conexion.h"
#include "funcionesAES.h"
#include "funcionesRSA.h"
#include "funcionesGS.h"
#include "funcionesDH.h"

typedef struct SCONEXION
{
	int socket;
	EVP_CIPHER_CTX* ctx;
	unsigned char* key;
	unsigned char* iv;
	groupsig_key_t *grpkey;
	groupsig_key_t *memkey;	
	int scheme;
	EVP_PKEY* keyRSA;
}Sconexion_t;

Sconexion_t* initSconexion(int socket, groupsig_key_t *grpkey, 
	groupsig_key_t *memkey, int scheme, EVP_PKEY* keyRSA);

int freeSconexion(Sconexion_t* scnx);

int sendClientCiphMsg(Sconexion_t* scnx,
	const unsigned char* text, int textlen);

int reciveServerCiphMsg(Sconexion_t* scnx, char** msg);


int sendServerCiphMsg(Sconexion_t* scnx,
	const unsigned char* text, int textlen);

int reciveClientCiphMsg(Sconexion_t* scnx, char** msg);

int clientInitSConexion(Sconexion_t* scnx);

int serverInitSConexion(Sconexion_t* scnx, groupsig_key_t *mgrkey,
	crl_t* crl, gml_t* gml);


#endif