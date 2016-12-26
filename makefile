
FLAGS= -Wall -std=gnu99 -pedantic -O3 -Wcomment

SSLLIBS= -lssl -lcrypto
GSLIBS = -lgroupsig
all: server cliente

server: chatS src/server.c
	@gcc $(FLAGS) -o server src/server.c obj/*.o objS/*.o -lpthread $(SSLLIBS) $(GSLIBS)

cliente: chatC src/cliente.c
	@gcc $(FLAGS) -o cliente src/cliente.c obj/*.o  objC/*.o -lpthread -lncurses $(SSLLIBS) $(GSLIBS)

# OBJETOS CHAT

chatC: cripto obj/usuario.o obj/conexion.o objC/comandosu.o obj/parser.o obj/comandos.o obj/linkedList.o objC/clientui.o
	@echo "compilado el chat para el cliente"

chatS: cripto obj/usuario.o obj/conexion.o objS/comandoss.o obj/parser.o obj/comandos.o obj/linkedList.o
	@echo "compilado el chat para el servidor"

obj/usuario.o: obj src/usuario.c include/usuario.h
	@gcc $(FLAGS) -c -o obj/usuario.o src/usuario.c

obj/conexion.o: obj src/conexion.c include/conexion.h
	@gcc $(FLAGS) -c -o obj/conexion.o src/conexion.c

objC/comandosu.o: objC src/comandosu.c include/comandosu.h
	@gcc $(FLAGS) -c -o objC/comandosu.o src/comandosu.c

objS/comandoss.o: objS src/comandoss.c include/comandoss.h
	@gcc $(FLAGS) -c -o objS/comandoss.o src/comandoss.c

obj/parser.o: src/parser.c include/parser.h
	@gcc $(FLAGS) -c -o obj/parser.o src/parser.c

obj/comandos.o: obj src/comandos.c include/comandos.h
	@gcc $(FLAGS) -c -o obj/comandos.o src/comandos.c

obj/linkedList.o: obj src/linkedList.c include/linkedList.h
	@gcc $(FLAGS) -c -o obj/linkedList.o src/linkedList.c 

objC/clientui.o: objC src/clientui.c include/linkedList.h
	@gcc $(FLAGS) -c -o objC/clientui.o src/clientui.c 

#PROTOCOLOS CRIPTOGRAFICOS

cripto: obj/funcionesDH.o obj/funcionesAES.o obj/funcionesRSA.o obj/funcionesGS.o obj/sconexion.o
	@echo "compilados los protocolos criptograficos"

obj/sconexion.o: obj src/sconexion.c include/sconexion.h
	@gcc $(FLAGS) -c -o obj/sconexion.o src/sconexion.c 

obj/funcionesDH.o: obj src/funcionesDH.c include/funcionesDH.h
	@gcc $(FLAGS) -c -o obj/funcionesDH.o src/funcionesDH.c 

obj/funcionesAES.o: obj src/funcionesAES.c include/funcionesAES.h
	@gcc $(FLAGS) -c -o obj/funcionesAES.o src/funcionesAES.c 

obj/funcionesRSA.o: obj src/funcionesRSA.c obj/conexion.o include/funcionesRSA.h
	@gcc $(FLAGS) -c -o obj/funcionesRSA.o src/funcionesRSA.c

obj/funcionesGS.o: obj src/funcionesGS.c include/funcionesGS.h
	@gcc $(FLAGS) -c -o obj/funcionesGS.o src/funcionesGS.c 

# TESTS

testRSA: obj test/serverRSA.c test/clientRSA.c obj/funcionesRSA.o obj/conexion.o
	@gcc $(FLAGS) -o TestServerRSA test/serverRSA.c obj/funcionesRSA.o obj/conexion.o $(SSLLIBS)	
	@gcc $(FLAGS) -o TestClientRSA test/clientRSA.c obj/funcionesRSA.o obj/conexion.o $(SSLLIBS)

testDH: obj test/serverDH.c test/clientDH.c obj/funcionesDH.o obj/conexion.o
	@gcc $(FLAGS) -o TestServerDH test/serverDH.c obj/* $(SSLLIBS)
	@gcc $(FLAGS) -o TestClientDH test/clientDH.c obj/* $(SSLLIBS)

testAES: obj test/AES.c obj/funcionesAES.o
	@gcc $(FLAGS) -o TestAES test/AES.c obj/funcionesAES.o $(SSLLIBS)

testGS: obj test/serverGS.c test/clientGS.c obj/funcionesGS.o obj/conexion.o
	@gcc $(FLAGS) -o TestServerGS test/serverGS.c obj/* $(GSLIBS) $(SSLLIBS)
	@gcc $(FLAGS) -o TestClientGS test/clientGS.c obj/* $(GSLIBS) $(SSLLIBS)

testBan: obj/parser.o test/bans.c
	@gcc $(FLAGS) -o TestBan test/bans.c obj/*

# DIRECTORIOS

obj:
	@mkdir obj

objC:
	@mkdir objC

objS:
	@mkdir objS

#GENERAR CLAVES
RSAkeys:
	@openssl genrsa -out privkey.pem 2048
	@openssl rsa -in privkey.pem -pubout -out pubkey.pub


#LIMPIEZA

mrProper:
	@rm -f obj/* objS/* objC/* server cliente funcionesDH funcionesAES Test* main funciones* main
	@rm -f *.txt
#GIT

commit:
	@git add .
	@git commit -m "$(msg)"
	@git push