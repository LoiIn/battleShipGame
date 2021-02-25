/*
******************************************************************
*    Battle Ship Game - Client.h
*    Author:
*        +) Can Duc Loi
*        +) Truong Van Hung
*        +) Nguyen Thi Ngan 
*******************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>

#define MAX_STRING_LENGTH 3000
#define PORT 8000

int signInRs;                           // signIn result of client
int clientFD;                           // clientSocket FD

struct sockaddr_in clientAddress;       
struct sockaddr_in serverAddress;

char sendBuff[MAX_STRING_LENGTH];       // send message
char recvBuff[MAX_STRING_LENGTH];       // receive message

int map[10][10];                        // map of client
int mapOpt[10][10];                     // map of opt (use to show shoted palace)
int rowS;                               // contain shoted row's palace (X)
int colS;                               // contain shoted col's palace (Y)

// client's function
bool checkParams(int argc, char port[]);
void initializeClientSocket(char IP[]);
void handleIncomingInformation();
void allHandle();

// signIn or SignUp's functions
void SingInSignUp();
void SignOut();

// game's functions
void createRoom();
void joinGame();
void hostComunicateClient();
void clientComunicateHost();
void playWithMachine();
void shot(int *rowSend, int *colSend);
void defenseServer();
void initMap();
void makeMap();
void initViewOfMapOpt();
int checkNgang(int row, int col,int i);
int checkDoc(int row, int col,int i);
void printMap();
void showMapBoomAll();
void printMapOpt(char optName[]);
void updateMap(int CHEDO, int row, int col, int ship_len);
void defenseClient(char optName[]);
void shotClient(char optName[]);
void clearChar (int error);
int checkLength(char str[]);

// lazy function
void showError(char errMes[]);
void recvMessFormServer();
void sendMessToServer(char str[]);
int countEdnterCharacter(char str[]);