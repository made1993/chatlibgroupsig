#ifndef CLIENTUI_H
#define CLIENTUI_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_LEN 8096 

void getWIndowSize();

WINDOW *createNewWin(int height, int width, int starty, int startx);
WINDOW *createNewWinHidden(int height, int width, int starty, int startx);

void destroyWin(WINDOW *win);

void printMsg(char* str);
char* scanMsg();
void createClientUI();
void destroyClientUI();
#endif