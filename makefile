
FLAGS= -Wall -std=gnu99 -pedantic -O3 -Wcomment

SSLLIBS= -lssl -lcrypto
GSLIBS = -lgroupsig
all: server cliente

server: obj/usuario.o obj/conexion.o obj/comandos.o obj/parser.o obj/linkedList.o objS/comandoss.o src/server.c
	@gcc $(FLAGS) -o server src/server.c obj/*.o objS/*.o -lpthread

cliente: obj/usuario.o obj/conexion.o obj/comandos.o obj/parser.o obj/linkedList.o objC/comandosu.o objC/clientui.o  src/cliente.c
	@gcc $(FLAGS) -o cliente src/cliente.c obj/*.o  objC/*.o -lpthread -lncurses

# OBJETOS
obj/usuario.o: src/usuario.c include/usuario.h
	@gcc $(FLAGS) -c -o obj/usuario.o src/usuario.c

obj/conexion.o: src/conexion.c include/conexion.h
	@gcc $(FLAGS) -c -o obj/conexion.o src/conexion.c

objC/comandosu.o: src/comandosu.c include/comandosu.h
	@gcc $(FLAGS) -c -o objC/comandosu.o src/comandosu.c

objS/comandoss.o: src/comandoss.c include/comandoss.h
	@gcc $(FLAGS) -c -o objS/comandoss.o src/comandoss.c

obj/parser.o: src/parser.c include/parser.h
	@gcc $(FLAGS) -c -o obj/parser.o src/parser.c

obj/comandos.o: src/comandos.c include/comandos.h
	@gcc $(FLAGS) -c -o obj/comandos.o src/comandos.c

obj/linkedList.o: src/linkedList.c include/linkedList.h
	@gcc $(FLAGS) -c -o obj/linkedList.o src/linkedList.c 

objC/clientui.o: src/clientui.c include/linkedList.h
	@gcc $(FLAGS) -c -o objC/clientui.o src/clientui.c 

#PROTOCOLOS CRIPTOGRAFICOS

cripto: obj/funcionesDH.o obj/funcionesAES.o obj/funcionesRSA.o obj/funcionesGS.o
	@echo "compilados los protocolos criptograficos"

obj/funcionesDH.o: src/funcionesDH.c include/funcionesDH.h
	@gcc $(FLAGS) -c -o obj/funcionesDH.o src/funcionesDH.c 

obj/funcionesAES.o: src/funcionesAES.c include/funcionesAES.h
	@gcc $(FLAGS) -c -o obj/funcionesAES src/funcionesAES.c 

obj/funcionesRSA.o: src/funcionesRSA.c obj/conexion.o include/funcionesRSA.h
	@gcc $(FLAGS) -c -o obj/funcionesRSA.o src/funcionesRSA.c $(SSLLIBS)

obj/funcionesGS.o: src/funcionesGS.c include/funcionesGS.h
	@gcc $(FLAGS) -c -o obj/funcionesGS.o src/funcionesGS.c 

# TESTS

testRSA: test/serverRSA.c test/clientRSA.c obj/funcionesRSA.o obj/conexion.o
	@gcc $(FLAGS) -o TestServerRSA test/serverRSA.c obj/funcionesRSA.o obj/conexion.o $(SSLLIBS)	
	@gcc $(FLAGS) -o TestClientRSA test/clientRSA.c obj/funcionesRSA.o obj/conexion.o $(SSLLIBS)

testDH: test/serverDH.c test/clientDH.c obj/funcionesDH.o obj/conexion.o
	@gcc $(FLAGS) -o TestServerDH test/serverDH.c obj/* $(SSLLIBS)
	@gcc $(FLAGS) -o TestClientDH test/clientDH.c obj/* $(SSLLIBS)

testAES: test/AES.c obj/funcionesAES.o obj/conexion.o
	@gcc $(FLAGS) -o TestAES test/AES.c obj/* $(SSLLIBS)

testGS: test/serverGS.c test/clientGS.c obj/funcionesGS.o obj/conexion.o
	@gcc $(FLAGS) -o TestServerGS test/serverGS.c obj/* $(GSLIBS)
	@gcc $(FLAGS) -o TestClientGS test/clientGS.c obj/* $(GSLIBS)


#GENERAR CLAVES
RSAkeys:
	@openssl genrsa -out privkey.pem 2048
	@openssl rsa -in privkey.pem -pubout > pubkey.pub


#LIMPIEZA

mrProper:
	@rm -f obj/* objS/* objC/* server cliente funcionesDH funcionesAES Test* main funciones* main

#GIT

commit:
	@git add .
	@git commit -m "$(msg)"
	@git push