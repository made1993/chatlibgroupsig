#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comandos.h"

int parseNick();

int parseMsg(char* src, char** nick, char ** msg);

#endif