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
		return DISCONNECT;
		
		case CPING:
		return PING;


		case CPONG:
		return PONG;

		case default:
			return -1;
		break
		
	}

}
int main (){
	return 0;
}