/*
******************************************************************
*    Battle Ship Game - HandleGame.c
*    Author:
*        +) Can Duc Loi
*        +) Truong Van Hung
*        +) Nguyen Thi Ngan 
*******************************************************************
*/

<<<<<<< HEAD
#include "Server.h"
=======
    char roomName[50], roomCode[10];
    bzero(roomName, 50); bzero(roomCode, 10); 
    const char s[2] = " ";
    char *token;

    recvMessFromClient(cliSock);
    token = strtok(recvBuff, s);
    strcpy(roomName, token);
    token = strtok(NULL, s);
    
    strcpy(roomCode, token);

    int createRs;
    do{
        createRs = checkExistRoom(roomName);
        if(createRs == -1){
            sendMessToClient(cliSock, "success");
            addRoomToList(roomName, roomCode, hostName, "none");
            break;
        }else{
            sendMessToClient(cliSock, "fail");
            recvMessFromClient(cliSock);
            bzero(roomName, 50);bzero(roomCode, 10);
            token = strtok(recvBuff, s);
            strcpy(roomName, token);
            token = strtok(NULL, s);
            strcpy(roomCode, token);
        }
    }while(createRs != -1 );
}
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c

// Set the list of rooms that can participate
void setMenuListRoomCanJoin(){
    char menu[MAX_STRING_LENGTH], oneLine[100], num[10];
    bzero(menu, MAX_STRING_LENGTH);   
    strcat(menu, "\n\n");
    strcat(menu, "------------------GAME's ROOM--------------------\n");
    strcat(menu,"- Name\t\tCode\tHost\tPlayer's number\n");
    strcat(menu, "-------------------------------------------------\n");
    for(int i = 0; i < curRoom; i ++){
        if(listRoom[i]->numMembers == 1){
            bzero(oneLine, 100);
            bzero(num, 10);
            snprintf( num, 10, "%d", listRoom[i]->numMembers);
            strcat(oneLine, "- ");
            strcat(oneLine, listRoom[i]->roomName);
            strcat(oneLine, "\t\t");
            strcat(oneLine, listRoom[i]->roomCode);
            strcat(oneLine, "\t");
            strcat(oneLine, listRoom[i]->hostName);
            strcat(oneLine, "\t");
            strcat(oneLine, num);
            strcat(oneLine, "\n");
            strcat(menu, oneLine);
        }
    }
    strcat(menu, "-------------------------------------------------\n");
    bzero(sendBuff, MAX_STRING_LENGTH);
    strcpy(sendBuff, menu);
}

// Delete all room (peole creating or joining)
void removeRoomHaveByOnePeople(int cliSock){
    char userName[30];
    bzero(userName, 30); 
    int pos = getPosOfUser(cliSock);
    strcpy(userName, listOn[pos]->userName);

    int type = getTypeRoomOfOneUser(userName);

    int posRoom = getPosRoom(userName);
    
    switch (type){
    case 1:         // host room
        if(strcmp(listRoom[posRoom]->clientName, "none") == 0){
            removeRoomFromList(listRoom[posRoom]->roomName);
        }else{
            char newRN[30], newRC[30], newClN[30];
            bzero(newRN, 30); bzero(newRC, 30);  bzero(newClN, 30);
            strcpy(newRN, listRoom[posRoom]->roomName);
            strcpy(newRC, listRoom[posRoom]->roomCode);
            strcpy(newClN, listRoom[posRoom]->clientName);
            removeRoomFromList(listRoom[posRoom]->roomName);
            addRoomToList(newRN, newRC, newClN, "none");
        }
        break;
    case 2:         // client
        updateClientJoinRoom(posRoom, "none");
        break;
    default:        // out of room 
        break;
    }
}

// Create your room and wait for your opponent
void createRoomInServer(ACC head, int cliSock){

    sendMessToClient(cliSock, "----------Create your's room game----------");
    char hostName[30];
    bzero(hostName, 30);
    int pos = getPosOfUser(cliSock);
    strcpy(hostName, listOn[pos]->userName);

    char roomName[50], roomCode[10];
    bzero(roomName, 50); bzero(roomCode, 10); 
    const char s[2] = " ";
    char *token;

    char *mesCreate = malloc(MAX_STRING_LENGTH);
    bzero(mesCreate, MAX_STRING_LENGTH);
    strcpy(mesCreate, recvMessFromClient(cliSock));

    if(strlen(mesCreate) == 0) return;

    token = strtok(mesCreate, s);
    strcpy(roomName, token);
    token = strtok(NULL, s);
    strcpy(roomCode, token);

    int createRs;
    do{
        createRs = checkExistRoom(roomName);
        if(createRs == -1){
            sendMessToClient(cliSock, "success");
            addRoomToList(roomName, roomCode, hostName, "none");
            playGameInMyRoom(head, cliSock);
            break;
        }else{
            sendMessToClient(cliSock, "fail");
            bzero(mesCreate, MAX_STRING_LENGTH);
            strcpy(mesCreate, recvMessFromClient(cliSock));

            if(strlen(mesCreate) == 0) return;

            bzero(roomName, 50);bzero(roomCode, 10);
            token = strtok(mesCreate, s);
            strcpy(roomName, token);
            token = strtok(NULL, s);
            strcpy(roomCode, token);
        }
    }while(createRs != -1 );
}

// Processing function for room owners after room is created
void playGameInMyRoom(ACC head, int cliSock){
    int k = getPosOfUser(cliSock);
    char hostName[30];
    bzero(hostName, 30);
    strcpy(hostName, listOn[k]->userName);

    int curRoom = getPosRoom(hostName);
    char cliName[30];
    char recvMes1[MAX_STRING_LENGTH];

    // The loop ensures the game only starts when the room has client
    while(1){
        bzero(recvMes1, MAX_STRING_LENGTH);
        strcpy(recvMes1, recvMessFromClient(cliSock));

        if(strlen(recvMes1) == 0){
            closeSocket(cliSock);
            return;
        }

        bzero(cliName, 30);
        strcpy(cliName, listRoom[curRoom]->clientName);

        if(strcmp(cliName, "none") != 0){
            sendMessToClient(cliSock, "ok");
            break;
        }
        sendMessToClient(cliSock, "not ok");
        sleep(1);
    }

    // checking host is ok status    
    bzero(recvMes1, MAX_STRING_LENGTH);
    strcpy(recvMes1, recvMessFromClient(cliSock));

    if(strcmp(recvMes1, "connect please") == 0){

        // send the room owner's name to the client
        sendMessToClient(cliSock, cliName);
        int cliSock2 = getCliSockByName(cliName);

        // khoi tao file log
        // char nameFile[100];
        // set_fileLog_name(1, nameFile);
        
        while (1){
            
            // if the guest is disconnected, the winner is room owner
            if(getPosOfUser(cliSock2) == -1){
                printf("$ %s out room\n", cliName);
                // bzero(recvMes1, MAX_STRING_LENGTH);
                // strcpy(recvMes1, recvMessFromClient(cliSock));
                head = updateACC(head, hostName, 1);
                head = readFile(head, userDatabasePath);
                head = updateACC(head, cliName, 0);
                writeFile(head, userDatabasePath);

                sendMessToClient(cliSock, "tiep nhe");
                return;
            }

            // receive a message from the room owner
            bzero(recvMes1, MAX_STRING_LENGTH);
            strcpy(recvMes1, recvMessFromClient(cliSock));

            // if the room owner is abruptly disconnected
            // send message to the client
            // and close room owner's socket
            if(strlen(recvMes1) == 0){
                sendMessToClient(cliSock2, "outRoom"); 
                closeSocket(cliSock);
                break;
            }

            // if you receive information that the room owner of is the winner
            // end game
            else if(strcmp(recvMes1, "iwin") == 0){
                printf("$ Winner is: %s (host)\n", hostName);

                // update player parameters
                head = updateACC(head, hostName, 1);
                head = readFile(head, userDatabasePath);
                head = updateACC(head, cliName, 0);
                writeFile(head, userDatabasePath);
                removeRoomFromList(listRoom[curRoom]->roomName);
                // write_to_fileLog(nameFile, hostName);
                break;
            }

            // send room owner's host to client
            sendMessToClient(cliSock2, recvMes1);  

            // if host is loser => end game 
            if(strcmp(recvMes1, "endGame") == 0){
                printf("$ Winner is: %s (client)\n",cliName);

                 // update player parameters
                head = updateACC(head, hostName, 0);
                head = readFile(head, userDatabasePath);
                head = updateACC(head, cliName, 1);
                writeFile(head, userDatabasePath);
                removeRoomFromList(listRoom[curRoom]->roomName);
                // write_to_fileLog(nameFile, cliName);
                break;
            }
        }

    }
}

// Join other's room functon 
void joinGameInServer(ACC head, int cliSock){

    // set menu 
    setMenuListRoomCanJoin();
    send(cliSock, sendBuff, strlen(sendBuff), 0);

    char *mesJion = malloc(MAX_STRING_LENGTH);
    bzero(mesJion, MAX_STRING_LENGTH);
    strcpy(mesJion, recvMessFromClient(cliSock));

    if(strlen(mesJion) == 0) return;

    if(strcmp(mesJion, "last") == 0){
        sendMessToClient(cliSock, "other");
        return;
    }
    else{
        char cliName[30];
        bzero(cliName, 30);

        // get name of user and sang to host 
        int pos = getPosOfUser(cliSock);
        strcpy(cliName, listOn[pos]->userName);

        // get room's name and room's code from client's input
        char roomName[50], roomCode[10];
        bzero(roomName, 50); bzero(roomCode, 10); 
        const char s[2] = " ";
        char *token;

        token = strtok(mesJion, s);
        strcpy(roomName, token);
        token = strtok(NULL, s);
        strcpy(roomCode, token);

        int joinRs;                             // join room's result 
        do{
            joinRs = checkAllExistRoom(roomName, roomCode); 
            if(joinRs != -1){
                sendMessToClient(cliSock, "success");
                updateClientJoinRoom(joinRs, cliName);
                playGameInOtherRoom(head, cliSock);
                break;
            }else{
                sendMessToClient(cliSock, "fail");
                bzero(mesJion, MAX_STRING_LENGTH);
                strcpy(mesJion, recvMessFromClient(cliSock));  

                if(strlen(mesJion) == 0) return;

                bzero(roomName, 50);bzero(roomCode, 10);
                token = strtok(mesJion, s);
                strcpy(roomName, token);
                token = strtok(NULL, s);
                strcpy(roomCode, token);
            }
        }while(joinRs == -1 );
    }
}

// Processing function for client after joined room
void playGameInOtherRoom(ACC head, int cliSock){
    int k = getPosOfUser(cliSock);
    char userName[30];
    bzero(userName, 30);
    strcpy(userName, listOn[k]->userName);

    int curRoom = getPosRoom(userName);
    char hostName[30];
    bzero(hostName, 30);
    strcpy(hostName, listRoom[curRoom]->hostName);

    // check client ready?
    char recvMes2[MAX_STRING_LENGTH];
    bzero(recvMes2, MAX_STRING_LENGTH);
    strcpy(recvMes2, recvMessFromClient(cliSock));

    if(strcmp(recvMes2, "connect please") == 0){

        // get room owner's name and send to client 
        sendMessToClient(cliSock, hostName);
        int cliSock2 = getCliSockByName(hostName);

        while (1){
            
            if(getPosOfUser(cliSock2) == -1){
                printf("%s out room\n", hostName);

                head = updateACC(head, userName, 1);
                head = readFile(head, userDatabasePath);
                head = updateACC(head, hostName, 0);
                writeFile(head, userDatabasePath);

                sendMessToClient(cliSock, "tiep nhe");
                return;
            }

            // receive a message from the room's client 
            bzero(recvMes2, MAX_STRING_LENGTH);
            strcpy(recvMes2, recvMessFromClient(cliSock));

            // if the client is abruptly disconnected
            // send message to the room owner
            // and close room client's socket
            if(strlen(recvMes2) == 0){
                sendMessToClient(cliSock2, "outRoom");
                closeSocket(cliSock);
                break;
            }

            // if client ís winner => end game
            else if(strcmp(recvMes2, "iwin") == 0){
                // printf("$ Winner is: %s (client)\n", userName);
                break;
            }

            // send mess from client to the host 
            sendMessToClient(cliSock2, recvMes2);

            // if client is the loser => end game   
            if(strcmp(recvMes2, "endGame") == 0){
                // printf("$ Winner is: %s (host)\n", hostName);
                break;
            }
            
        }
    }
}

// play with machine function
void playWithMachine(int cliSock){
    char userName[30];
    bzero(userName, 30);
    strcpy(userName, listOn[getPosOfUser(cliSock)]->userName);
    
    sendMessToClient(cliSock, "---------------- play with machine! ------------------");

    char *mesPlayMachine = malloc(MAX_STRING_LENGTH);
    bzero(mesPlayMachine, MAX_STRING_LENGTH);
    strcpy(mesPlayMachine, recvMessFromClient(cliSock));

    if(strcmp(mesPlayMachine, "mapDone") == 0){
        int mapServer[10][10];
        initMap(mapServer);
<<<<<<< HEAD
        
        char *mapArr[10] = {"map1.txt","map2.txt","map3.txt","map4.txt","map5.txt","map6.txt","map7.txt"};
        char *mapPath =  malloc(MAX_STRING_LENGTH);
        bzero(mapPath, MAX_STRING_LENGTH);
        strcat(mapPath, databasePath);
        srand((unsigned) time(&timeHihi));

        int k = rand() % MAX_MAP;
        
        strcat(mapPath, mapArr[k]);

        loadMap(mapPath, mapServer);
=======
        int MAX_MAP=7;
        char *array[10]={"map1.txt","map2.txt","map3.txt","map4.txt","map5.txt","map6.txt","map7.txt"};

        srand(time(NULL));

        loadMap(array[rand() % MAX_MAP], mapServer);
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c
        showMap(mapServer);

        int endGame = 0;
        sendMessToClient(cliSock, "Toa do cua ban?");
        defenseClient(cliSock, mapServer, &endGame);
    
        while (endGame == 0){
            if(strcmp(sendBuff, "endGame") == 0 || strcmp(sendBuff, "machine_win") == 0){
                endGame = 2;
                break;
            }else if(strcmp(sendBuff, "boom") == 0||strcmp(sendBuff, "machine_miss") == 0){
                defenseClient(cliSock, mapServer, &endGame);
            }else if(strcmp(sendBuff, "miss") == 0||strcmp(sendBuff, "machine_boom") == 0){
                shotClient(cliSock, &endGame);
            }
        }
        
    }
}

// machine defense turn
void defenseClient(int cliSock, int mapServer[10][10], int *endG){
    char *mesDefend = malloc(MAX_STRING_LENGTH);
    bzero(mesDefend, MAX_STRING_LENGTH);
    strcpy(mesDefend, recvMessFromClient(cliSock));

    if(strlen(mesDefend) ==  0){
        *endG = 1;
        return;
    }

    char *token;
    char colc[10], rowc[10];
    int col, row;

    token = strtok(mesDefend, " ");
    strcpy(rowc, token);
    token = strtok(NULL, " ");
    strcpy(colc, token);

    row = atoi(rowc);col = atoi(colc);
    if(mapServer[row][col] == 2){
        initMap(mapServer);
        showMap(mapServer);
        sendMessToClient(cliSock, "endGame");
    }
    else if(mapServer[row][col] == 1){
        mapServer[row][col] = 0;
        printf("\n$-------------------------------------");
        printf("\n$%d shoted",cliSock);
        showMap(mapServer);
        sendMessToClient(cliSock, "boom");
    }else{
        sendMessToClient(cliSock, "miss");
    }
}

// machine shot turn 
void shotClient(int cliSock, int *endG){
    char *mesShot = malloc(MAX_STRING_LENGTH);
    bzero(mesShot, MAX_STRING_LENGTH);
    strcpy(mesShot, recvMessFromClient(cliSock));

    int col, row;
    srand((unsigned) time(&timeHihi));
<<<<<<< HEAD

=======
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c
    int map_rand[10][10];
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            map_rand[i][j]=0;
        }
    }
    col = rand() % 10;
    row = rand() % 10;
    while(map_rand[row][col]==-1){
        col = rand() % 10;
        row = rand() % 10;
    }
    map_rand[row][col]=-1;
<<<<<<< HEAD
    // row = 0;
    // col = 0;
=======
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c

    char rowc[10], colc[10];
    char str[30];
    bzero(str, 30);

    snprintf(rowc, 10, "%d", row);
    snprintf(colc, 10, "%d", col);
    strcat(str, rowc);
    strcat(str, " ");
    strcat(str, colc);

    bzero(sendBuff, MAX_STRING_LENGTH);
    strcpy(sendBuff, str);
    if(send(cliSock, sendBuff, strlen(sendBuff), 0) != strlen(sendBuff)){
        showError("send fail!");
    }
    
    bzero(mesShot, MAX_STRING_LENGTH);
    strcpy(mesShot, recvMessFromClient(cliSock));

    if(strlen(mesShot) == 0){
        *endG = 1;
        return;
    }

    bzero(sendBuff, MAX_STRING_LENGTH);
    if(strcmp(mesShot, "cliLose") == 0){
        sendMessToClient(cliSock, "machine_win");
    }
    else if(strcmp(mesShot, "boom") == 0){
        sendMessToClient(cliSock, "machine_boom");
        sleep(2);
    }else if(strcmp(mesShot, "miss") == 0){
        sendMessToClient(cliSock, "machine_miss");
    }
}

// initilize server's map
void initMap(int mapServer[10][10]){
    for(int i = 0 ; i < 10; i++){
        for(int j = 0; j  < 10; j ++){
            mapServer[i][j] = 0;
        }
    }
}

// load map from database 
void loadMap(char fileMap[], int mapServer[10][10]){
<<<<<<< HEAD
  FILE *f = fopen(fileMap, "r");
=======
   FILE *f = fopen(fileMap, "r");
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c
   printf("%s\n",fileMap );
    if( f == NULL){
        printf("\n===>Can't open this file to read!\n");
    }else{
        // int i= 0, j = 0;
        while (!feof(f)){
<<<<<<< HEAD
            fscanf(f, "%d", &mapServer[i][j]);
            j ++;
            if(j == 10) i ++;
=======
            // j ++;
            // if(j == 10) i ++;
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c
            for(int i = 0 ; i < 10; i++){
                for(int j = 0; j  < 10; j ++){
                    fscanf(f, "%d", &mapServer[i][j]);

                }
            }
        }
        fclose(f);
    }
}

// show map to screen
void showMap(int mapServer[10][10]){
    printf("\n--------------------------------\n");
    printf("[+] Machine's map:\n");
    printf("   0  1  2  3  4  5  6  7  8  9 \n");
	for( int i=0; i< 10; i++){
		printf("%d  ",i );
		for( int j=0;  j<10; j++){
			printf("%d  ",mapServer[i][j] );

		}
		printf("\n");
	}
    printf("--------------------------------\n");
}