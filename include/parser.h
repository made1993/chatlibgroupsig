#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** banWords = NULL;
int nwords;
int parseNick(char* src, char** nick1, char ** nick2);

int parseMsg(char* src, char** nick, char ** msg);

int iniBigBrother(char* fname);
int bigBrother(char* msg, int msglen);
void freeBigBrother();

#endif