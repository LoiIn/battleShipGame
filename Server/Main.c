/*
******************************************************************
*    Battle Ship Game - Main.c
*    Author:
*        +) Can Duc Loi
*        +) Truong Van Hung
*        +) Nguyen Thi Ngan 
*******************************************************************
*/

#include "Server.h"

// init file log's name
void set_fileLog_name(int n, char *name){
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(name, "%d#%d-%d-%d#%d:%d:%d.log", n, timeinfo->tm_year + 1900,timeinfo->tm_mon + 1, timeinfo->tm_mday,  timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

// write content to filelog
void write_to_fileLog(char* logFile, char* buff){
	FILE *f = fopen(logFile, "a");
	if(f == NULL ){
		fclose(f);
		FILE* f = fopen(logFile, "a");
	  fprintf(f, "%s", buff);
	  fclose(f);
	} else {
		fprintf(f, "%s", buff);
  	fclose(f);
	}
}

// Display error during execution 
void showError(char errMes[]){
	char str[MAX_STRING_LENGTH];
	bzero(str, MAX_STRING_LENGTH);
	strcat(str, "[-] Error: ");
	strcat(str, errMes);
    strcat(str,"\n");
	write(1, str, 12 + strlen(errMes));
}

// Send message from server to client
void sendMessToClient(int cliSock, char str[]){
    bzero(sendBuff, MAX_STRING_LENGTH);
    strcpy(sendBuff, str);
    if(send(cliSock, sendBuff, strlen(sendBuff), 0) != strlen(sendBuff)){
        showError("send fail!");
    }
}

// receive messages from client to Server 
char* recvMessFromClient(int cliSock){
    char *s = malloc(MAX_STRING_LENGTH);
    bzero(s, MAX_STRING_LENGTH);
    recv(cliSock, s, MAX_STRING_LENGTH, 0);
    return s;
}

// All function in each thread 
void *handleAllGame(void *arg){
    struct arg_struct *args = arg;
    printf("$ Clients at Socket(%d) is connected!\n",args->cli->cliSock);
    signInToSystem(args->head, args->cli->cliSock, &(args->cli->joinedStatus));
    if(args->cli->joinedStatus == 1){
        handleIncomingInformations( args->head,args->cli->cliSock, args->cli->joinedStatus);
    }else{
        printf("$ Clients at Socket(%d) is disconnected!\n",args->cli->cliSock);
    }
}

int main(int argc, char *argv[]){

    serverLive = 1;
    struct arg_struct *args = malloc(sizeof(*args));
    args->head = initHead();
    args->head = readFile(args->head, userDatabasePath);
    curPos = 0;
    curRoom = 0;

    pthread_mutex_init(&clients_mutex, NULL);

    initializeServerSocket();

    while(true){
        
        
        int newSocket = handleIncomingConnections();

        userOnline *newCli = (userOnline *)malloc(sizeof(userOnline));
        newCli->cliSock = newSocket;
        newCli->joinedStatus = 0;
        bzero(newCli->userName, 30);

        args->cli = (userOnline *)malloc(sizeof(userOnline));
        args->cli->cliSock = newSocket;
        args->cli->joinedStatus = 0;
        bzero(args->cli->userName, 30);

        args->room = (roomData *)malloc(sizeof(roomData));
        bzero(args->room->roomName, 50);
        bzero(args->room->roomCode, 50);
        bzero(args->room->hostName, 30);
        bzero(args->room->clientName, 30);
        args->room->numMembers = 0;

        pthread_create(&tid, NULL, &handleAllGame, (void*)args);
               
    }
    
    return 1;
}