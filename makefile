
FLAGS= -Wall

all: obj/usuario.o obj/conexion.o obj/comandosu.o obj/comandoss.o obj/comandos.o


obj/usuario.o:
	@gcc $(FLAGS) -c -o obj/usuario.o src/usuario.c


obj/conexion.o:
	@gcc $(FLAGS) -c -o obj/conexion.o src/conexion.c


obj/comandosu.o:
	@gcc $(FLAGS) -c -o obj/comandosu.o src/comandosu.c


obj/comandoss.o:
	@gcc $(FLAGS) -c -o obj/comandoss.o src/comandoss.c


obj/comandos.o:
	@gcc $(FLAGS) -c -o obj/comandos.o src/comandos.c


mrProper:
	@rm -f obj/*