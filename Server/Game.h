/*
******************************************************************
*    Battle Ship Game - Game.h
*    Author:
*        +) Can Duc Loi
*        +) Truong Van Hung
*        +) Nguyen Thi Ngan 
*******************************************************************
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_GAME_LENGTH 30

typedef struct {
    char roomName[MAX_GAME_LENGTH];
    char roomCode[MAX_GAME_LENGTH];
    char hostName[MAX_GAME_LENGTH];
    char clientName[MAX_GAME_LENGTH];
    int numMembers;
}roomData;

roomData *listRoom[MAX_GAME_LENGTH];

int curRoom;

int curGame;

void showListRoom();

int checkExistRoom(char rN[]);   

int checkAllExistRoom(char rN[], char rC[]);  

void addRoomToList(char rN[], char rC[], char hN[], char cN[]);

void removeRoomFromList(char rN[]);

void updateClientJoinRoom(int pos, char cliName[]);

int getTypeRoomOfOneUser(char uN[]);

int getPosRoom(char uN[]);
