/*
******************************************************************
*    Battle Ship Game - Game.c
*    Author:
*        +) Can Duc Loi
*        +) Truong Van Hung
*        +) Nguyen Thi Ngan 
*******************************************************************
*/


#include "Game.h"

// show room's list
void showListRoom(){
    printf("----------------Room List----------------\n");
    for(int i = 0; i < curRoom; i++){
        printf("%d. %s - %s - %s - %s - %d\n", i + 1, listRoom[i]->roomName, listRoom[i]->roomCode, listRoom[i]->hostName, listRoom[i]->clientName, listRoom[i]->numMembers);
    }
    printf("-----------------------------------------\n");
}

// check room's name and code (return -1 if dose not exist)
int checkAllExistRoom(char rN[], char rC[]){
    for(int i = 0; i < curRoom; i ++){
        if(strcmp(listRoom[i]->roomName, rN) == 0 && strcmp(listRoom[i]->roomCode, rC) == 0){
            return i;
        }
    }
    return -1;
}

// check exist room name (return -1 if dose not exist)
int checkExistRoom(char rN[]){
    for(int i = 0; i < curRoom; i ++){
        if(strcmp(listRoom[i]->roomName, rN) == 0){
            return i;
        }
    }
    return -1;
}

// check position of room 
int getPosRoom(char uN[]){
    for(int i = 0; i < curRoom; i ++){
        if(strcmp(listRoom[i]->hostName, uN) == 0 || strcmp(listRoom[i]->clientName, uN) == 0){
            return i;
        }
    }
    return -1;
}

/* check type of user in room 
 * host: 1
 * client: 2
 * out: 1
*/
int getTypeRoomOfOneUser(char uN[]){
    for(int i = 0; i < curRoom; i ++){
        if(strcmp(listRoom[i]->hostName, uN) == 0){
            return 1;
        }
        if(strcmp(listRoom[i]->clientName, uN) == 0){
            return 2;
        }
    }
    return -1;
}

// add room to list
void addRoomToList(char rN[], char rC[], char hN[], char cN[]){
    listRoom[curRoom] = (roomData*)malloc(sizeof(roomData));
    strcpy(listRoom[curRoom]->roomName, rN);
    strcpy(listRoom[curRoom]->roomCode, rC);
    strcpy(listRoom[curRoom]->hostName, hN);
    strcpy(listRoom[curRoom]->clientName, cN);
    if(strcmp(cN, "none") == 0){
        listRoom[curRoom]->numMembers = 1;
    }else listRoom[curRoom]->numMembers = 2;
    curRoom ++;
}

// remove room to list
void removeRoomFromList(char rN[]){
    if(curRoom == 0) return;
    else{
        for(int i = 0 ; i <  curRoom; i ++){
            if(strcmp(listRoom[i]->roomName, rN) == 0){
                listRoom[i] = NULL;
                for(int j = i; j < curRoom - 1; j ++){
                    listRoom[j] = listRoom[j+1];
                }
                listRoom[curRoom] = NULL;
            }
        }
        curRoom --;
    }
}

// update room's infomation after one client joined room
void updateClientJoinRoom(int pos, char cliName[]){
    bzero(listRoom[pos]->clientName, 30);
    if(strcmp(cliName, "") != 0){
        strcpy(listRoom[pos]->clientName, cliName);
        listRoom[pos]->numMembers = 2;
    }else{
        strcpy(listRoom[pos]->clientName, "none");
        listRoom[pos]->numMembers = 1;
    }
}


