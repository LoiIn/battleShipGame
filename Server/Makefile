CC = gcc
CFLAGS = -c -Wall
LIBS =  -pthread

all: Server

debug: CFLAGS += -g
debug: Server

Server: Main.o Server.o SignIn_SignUp.o HandleGame.o User.o Game.o 
	${CC} Main.o Server.o SignIn_SignUp.o HandleGame.o User.o Game.o -o Server -pthread

Main.o: Main.c
	${CC} ${CFLAGS} Main.c

Server.o: Server.c
	${CC} ${CFLAGS} Server.c 

HandleGame.o: HandleGame.c
	${CC} ${CFLAGS} HandleGame.c

SignIn_SignUp.o: SignIn_SignUp.c
	${CC} ${CFLAGS} SignIn_SignUp.c

User.o: User.c
	${CC} ${CFLAGS} User.c

Game.o: Game.c
	${CC} ${CFLAGS} Game.c

clean: 
	rm -f *.o *~