/*
******************************************************************
*    Battle Ship Game - Server.h
*    Author:
*        +) Can Duc Loi
*        +) Truong Van Hung
*        +) Nguyen Thi Ngan 
*******************************************************************
*/

#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stdio_ext.h"
#include <stdbool.h>
<<<<<<< HEAD
=======
#include <time.h>


>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h> 
#include <pthread.h>

/* my library */
#include "User.h"
#include "Game.h"

#define MAX_STRING_LENGTH 3000 
#define MAXLINE 100

// default port for connect to server
#define PORT 8000                  

// database path, User database path, max count of map for server
#define databasePath "../Database/"
#define userDatabasePath "../Database/User.txt"
#define MAX_MAP 7                   

// mutext 
pthread_mutex_t clients_mutex;

// messages sent to client 
char sendBuff[MAX_STRING_LENGTH];


int serverSocketFD;             // description of the server
int serverLive;                 // variable to determine the server is still alive
pthread_t tid;                  // pthread structure declaration
time_t timeHihi;                

// Declaration the structure containing the data to be manipulated by each thread
struct arg_struct {
    ACC head;
    roomData *room;
    userOnline *cli;
};

// save match's history to file log
void set_fileLog_name(int n, char *name);
void write_to_fileLog(char* logFile, char* buff);   

// Main's functions
void *handleAllGame(void *arg);

// Menu's functions
void setMenuToShow(int cliSock);
void setSignInScreen();
void setMenuListRoomCanJoin();

// Server's funcitons
void initializeServerSocket();
int handleIncomingConnections();
void handleIncomingInformations(ACC head, int cliSock, int joinedStatus);
void closeSocket(int cliSock);

// Sign in, Sign out, Sign up;s functions
int SignIn(ACC head, char user[], char pass[]);
int registerACC(ACC head, char file[], char usn[], char pass[]);
int checkCurOn(char user[]);
void signInToSystem(ACC head, int cliSock, int *joinedStatus);
void handleIncomingSingInOrSignOut(int cliSock, ACC head, int **joinedStatus);
void signOutToSystem(int cliSock);

// HandleGame's functions 
void createRoomInServer(ACC head, int cliSock);
void joinGameInServer(ACC head, int cliSock);
void playWithMachine(int cliSock);
void defenseClient(int cliSock, int mapServer[10][10], int *endG);
void shotClient(int cliSock, int *endG);
void playGameInMyRoom(ACC head, int cliSock);
void playGameInOtherRoom(ACC head, int cliSock);
void removeRoomHaveByOnePeople(int cliSock);

// HandleGame's function (Server's map)
void initMap(int mapServer[10][10]);
void loadMap(char filePath[], int mapServer[10][10]);
void showMap(int mapServer[10][10]);

// lazy function (main file)
void showError(char errMes[]);
void sendMessToClient(int cliSock, char str[]);
char* recvMessFromClient(int cliSock);

// show list of online user 
void showListOnline();

#endif