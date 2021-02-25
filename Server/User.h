/*
******************************************************************
*    Battle Ship Game - User.h
*    Author:
*        +) Can Duc Loi
*        +) Truong Van Hung
*        +) Nguyen Thi Ngan 
*******************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USER_NAME_LENGTH 50
#define MAX_PASS_LENGTH 10
#define MAX_PAGE_LENGTH 30
#define MAX_ONLINE_LENGTH 10
#define MAX_CLIENTS 50

struct data{
    char userName[MAX_USER_NAME_LENGTH];
    char pass[MAX_PASS_LENGTH];
    char status[2];
    char page[MAX_PAGE_LENGTH];
    int joinedGame;
    int wonGame;
    double rank;
};

typedef struct data userData;

struct nodeACC{
    userData accData;
    struct nodeACC *next;
};

typedef struct nodeACC *ACC;

userData creatNewData(char *usN, char *pass, char *sts, char *page);

userData coppyData(userData sample);

int checkACC(ACC head, char *userName);

int checkACCPass(ACC head,char *usN, char *pass);

int checkACCStatus(ACC head, char *usN);

userData getData(ACC head, char *usN);
 
ACC createNewACC(ACC head, userData newData);
 
ACC addLast(ACC head, userData newData);
 
ACC addHead(ACC head, userData newData);
 
ACC addAtRandom(ACC head, userData newData, int position);
 
ACC deleteHead(ACC head);
 
ACC deleteTail(ACC head);
 
ACC deleteAtRadom(ACC head, int position);
 
void showList(ACC head);
 
ACC initHead();
 
int getLengthOfList(ACC head);
 
ACC readFile(ACC head, char file[]);

void writeFile(ACC head, char file[]);

ACC updateACC(ACC head, char *usN, int win);

void showMenu();

int checkACCSignedIn(ACC head, char *usN);

void updateListSignedIn(ACC head, char *usN);

ACC updateACC(ACC head, char *usN, int win);

// struct of user online's list
typedef struct{
    int cliSock;
    char userName[30];
    int joinedStatus;       // 1 is joined, 0 is dose not join
}userOnline;

userOnline *listOn[MAX_CLIENTS];
int curPos;

void showListOnline();

void addListOnline(int cliSock, char userName[]);

void removeListOnline(int cliSock);

int getPosOfUser(int cliSock);

int getCliSockByName(char userName[]);