
FLAGS= -Wall -std=gnu99 -pedantic -O3

all: server cliente

server: obj/usuario.o obj/conexion.o objS/comandoss.o obj/comandos.o obj/linkedList.o src/server.c
	@gcc $(FLAGS) -o server src/server.c obj/*.o objS/*.o -lpthread

cliente: obj/usuario.o obj/conexion.o objC/comandosu.o obj/comandos.o obj/linkedList.o objC/clientui.o src/cliente.c
	@gcc $(FLAGS) -o cliente src/cliente.c obj/*.o  objC/*.o -lpthread -lncurses

obj/usuario.o: src/usuario.c
	@gcc $(FLAGS) -c -o obj/usuario.o src/usuario.c

obj/conexion.o: src/conexion.c
	@gcc $(FLAGS) -c -o obj/conexion.o src/conexion.c

objC/comandosu.o: src/comandosu.c
	@gcc $(FLAGS) -c -o objC/comandosu.o src/comandosu.c

objS/comandoss.o: src/comandoss.c
	@gcc $(FLAGS) -c -o objS/comandoss.o src/comandoss.c

obj/comandos.o: src/comandos.c
	@gcc $(FLAGS) -c -o obj/comandos.o src/comandos.c

obj/linkedList.o: src/linkedList.c
	@gcc $(FLAGS) -c -o obj/linkedList.o src/linkedList.c 


objC/clientui.o: src/clientui.c
	@gcc $(FLAGS) -c -o objC/clientui.o src/clientui.c 
	

mrProper:
	@rm -f obj/* objS/* objC/* server cliente


commit:
	@git add .
	@git commit -m "$(msg)"
	@git push