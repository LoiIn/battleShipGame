CC = gcc
CFLAGS = -c -Wall

all: Client

debug: CFLAGS += -g
debug: Client

Client: Main.o Client.o SignIn_SignUp.o Game.o
	${CC} Main.o Client.o SignIn_SignUp.o Game.o -o Client -pthread


Main.o: Main.c
	${CC} ${CFLAGS} Main.c

Client.o: Client.c
	${CC} ${CFLAGS} Client.c

SignIn_SignUp.o: SignIn_SignUp.c
	${CC} ${CFLAGS} SignIn_SignUp.c

Game.o: Game.c
	${CC} ${CFLAGS} Game.c

clean: 
	rm -f *.o *~