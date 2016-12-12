#ifndef FUNCIONESGS_H
#define FUNCIONESGS_H

#include <groupsig/groupsig.h>
#include <groupsig/gml.h>
#include <groupsig/crl.h>
#include <time.h>

#include <openssl/pem.h>
#include <openssl/evp.h>

int import_server(groupsig_key_t** grpkey,  crl_t** crl, gml_t** gml, char* grpkeyf, char* crlf, char* gmlf);

int import_client(groupsig_key_t** grpkey, groupsig_key_t** memkey, char* grpkeyf, char* memkeyf);

int signMsgGS(groupsig_key_t* grpkey, groupsig_key_t* memkey, uint8_t scheme, char *msgstr, char** sigstr);

int verifySignGS(char* sigstr, groupsig_key_t *grpkey, char* msgstr, uint8_t scheme);

int sigMsgToStrGS(char * msgstr, int msglen, char* sigstr, int siglen, char** dst);

int strToSigMsgGS(char** msgstr, int *msglen, char** sigstr, int* siglen, char* src, int srclen);

#endif