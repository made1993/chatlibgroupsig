#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int parseNick(char* src, char** nick1, char ** nick2);

int parseMsg(char* src, char** nick, char ** msg);

void iniBigBrother(char* fname);
int bigBrother(char* src);
void freeBigBrother();

#endif