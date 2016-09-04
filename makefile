
FLAGS= -Wall

all: obj/usuario.o obj/conexion.o obj/comandosu.o obj/comandoss.o obj/comandos.o obj/linkedList.o


server: obj/usuario.o obj/conexion.o obj/comandoss.o obj/comandos.o obj/linkedList.o src/server.c
	@gcc $(FLAGS) -o server src/server.c obj/*.o -lpthread

cliente: obj/usuario.o obj/conexion.o obj/comandosu.o obj/comandos.o obj/linkedList.o
	@gcc $(FLAGS) -o cliente src/server.c obj/*.o -lpthread

obj/usuario.o: src/usuario.c
	@gcc $(FLAGS) -c -o obj/usuario.o src/usuario.c

obj/conexion.o: src/conexion.c
	@gcc $(FLAGS) -c -o obj/conexion.o src/conexion.c

obj/comandosu.o: src/comandosu.c
	@gcc $(FLAGS) -c -o obj/comandosu.o src/comandosu.c

obj/comandoss.o: src/comandoss.c
	@gcc $(FLAGS) -c -o obj/comandoss.o src/comandoss.c

obj/comandos.o: src/comandos.c
	@gcc $(FLAGS) -c -o obj/comandos.o src/comandos.c

obj/linkedList.o: src/linkedList.c
	@gcc $(FLAGS) -c -o obj/linkedList.o src/linkedList.c 

mrProper:
	@rm -f obj/*


commit:
	@git add .
	@git commit -m "$(msg)"
	@git push