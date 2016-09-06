#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}
WINDOW *create_newwin_hidden(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}
int main(int argc, char** argv){
	int row, col;
	WINDOW* win1, * win2, * win3, * win4;
	int pos = 0, max;

	char* str = malloc(sizeof(char)* 8096);

	initscr();
	getmaxyx(stdscr,row,col);
	max = row - 8;
	win1 = create_newwin(row - 6, col, 0, 0);
	win2 = create_newwin(6, col, row - 6, 0);
	win3 = create_newwin_hidden(row - 8, col - 2, 1, 1);
	win4 = create_newwin_hidden(4, col -2 , row - 5, 1);
	

	while(1){
		mvwgetstr( win4, 0, 0, str);
		destroy_win(win4);
		win4 = create_newwin_hidden(4, col -2 , row - 5, 1);
		sprintf(str, "%s\n", str);
		mvwaddstr(win3, pos, 0, str);
		wrefresh(win3);
		pos++;
		pos += (strlen(str)-1)/(col -2);
		pos= pos%max;
	}
	destroy_win(win1);
	destroy_win(win2);
	destroy_win(win3);
	destroy_win(win4);
	endwin();
	return 0;
}