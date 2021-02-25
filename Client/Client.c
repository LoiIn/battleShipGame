/*
******************************************************************
*    Battle Ship Game - Client.c
*    Author:
*        +) Can Duc Loi
*        +) Truong Van Hung
*        +) Nguyen Thi Ngan 
*******************************************************************
*/

#include "Client.h"

// check IP fucntion
bool checkIP(char Ip[]){
	return true;
}

int checkLength(char str[]){
    if(strcmp(str, "\n") != 0 && strlen(str) != 0) return 1;
    else return 0;
}

// check parameters
bool checkParams(int argc, char ip[]){
	if(argc != 2){
		showError("number of paramaters!");
		return false;
	}else{
		if(checkIP(ip)) return true;
		else{
			showError("ip address!");
			return false;
		}
	}
}

// initialize client Socket
void initializeClientSocket(char IP[]){
	if((clientFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		showError("Fail to open socket!");
        exit(0);
	}

	bzero(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = inet_addr(IP);
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_family = AF_INET;

	if(connect(clientFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
		showError("Connect fail!");
		exit(0);
	}
}

// handle all 
void handleIncomingInformation(){
	do{
		recvMessFormServer();
		char str[10];
		int choice;
		do{
			printf("%s", recvBuff);						// menu from server
			__fpurge(stdin);
			bzero(str, 10);
			printf("\n$ Your choice: ");
			fgets(str, 10, stdin);
			choice = atoi(str);
		}while(choice != 1 && choice != 2 && choice != 3 && choice != 4);

		switch (choice)
		{
		case 1:
			playWithMachine();
			break;
		case 2:
			joinGame();
			break;
		case 3:
			createRoom();
			break;
		case 4:
			SignOut();
			break;
		default:
			break;
		}
	}while (strcmp(sendBuff, "continue") == 0);
	
}