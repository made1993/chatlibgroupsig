#ifndef USUARIO_H
#define USUARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NICK_MAX_LEN 8

/*Por ahora esto, pero es posible que haya que añadir cosas de claves*/
typedef struct USUARIO
{
	int id;
	int pingt;
	char* nick;
	int* socket;
}Usuario_t;


Usuario_t* crearUsuario(int* socket);
int liberarUsuario(Usuario_t* usr);

int setNick(Usuario_t* usr, char* nick);
char* getNick(Usuario_t* usr);



int setId(Usuario_t* usr, int id);
int getId(Usuario_t* usr);

int setCurrentPingt(Usuario_t* usr);
int getPingt(Usuario_t* usr);

int compareUsr(const void* usr1, const void* usr2);



#endif	