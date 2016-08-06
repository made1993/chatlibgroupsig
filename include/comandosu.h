#ifndef COMANDOSU_H
#define COMANDOSU_H

#include "usuario.h"
#include "conexion.h"

int ping(int socket);
int pong(int socket);
int nick(int socket, char* nick);
int disconnect(int socket);

#endif