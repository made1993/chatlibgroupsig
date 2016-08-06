#include "../include/parser.h"

int command(char* str){
	char* com = NULL;
	if (str == NULL)
		return -1;
	com = strtok(str, " ");
	switch(com){
		case CNICK:
		return NICK;
		
		case CMSG:
		return MSG;
		
		case CDISCONNECT:
		return NICK;
		
		case CPING:
		return NICK;
		
	}

}
int main (){
	return 0;
}