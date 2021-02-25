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

// count enter character
int countEdnterCharacter(char str[]){
    const char s[2] = "\n";
    char *token;
    token = strtok(str, s);
    int count  = 0;
    while( token != NULL){
        token = strtok(NULL, s);
        count ++;
    }
    return count;
}

// create room service
void createRoom(){
    sendMessToServer("createRoom");
    recvMessFormServer();
    printf("$ %s\n", recvBuff);

    char roomName[50], roomCode[10], tmpbuff[100];
    bzero(tmpbuff, strlen(tmpbuff));
    bzero(roomName, strlen(roomName));
    bzero(roomCode, strlen(roomCode));

    do{
        do{
            __fpurge(stdin);
            printf("\n$ Room's name: ");
            fgets(roomName, 50, stdin);
            roomName[strlen(roomName) - 1] = '\0';
            if(checkLength(roomName) == 0){
                printf("[-] Input must no null!\n");
            }
        }while(checkLength(roomName) == 0);
        
        do{
            __fpurge(stdin);
            printf("$ Room's code: ");
            fgets(roomCode, 50, stdin);
            roomCode[strlen(roomCode) - 1] = '\0';
            if(checkLength(roomCode) == 0){
                printf("[-] Input must no null!\n");
            }
        }while(checkLength(roomCode) == 0);

        strcat(tmpbuff, roomName);
        strcat(tmpbuff, " ");
        strcat(tmpbuff, roomCode);

        sendMessToServer(tmpbuff);

        recvMessFormServer();

        if(strcmp(recvBuff, "success") == 0){
            printf("$ Success! Watting some one ... \n");
            hostComunicateClient();
            sendMessToServer("continue");
            // recvMessFormServer();
            // sendMessToServer("continue");
            break;
        }
        else{
            printf("$ Exist room! Try again!\n");
        }
        bzero(tmpbuff, strlen(tmpbuff));

    }while(strcmp(recvBuff, "fail") == 0);
}

// join game function
void joinGame(){
    sendMessToServer("joinRoom");

    recvMessFormServer();
    printf("%s\n", recvBuff);

    // if have not any room => return menu
    if(countEdnterCharacter(recvBuff) == 4){
        printf("$ There is no suitable room. Create a room now!\n");
        sendMessToServer("last");
        recvMessFormServer();
        sendMessToServer("continue");
        return;
    }else{
        char roomName[50], roomCode[10], tmpbuff[100];
        bzero(tmpbuff, strlen(tmpbuff));
        bzero(roomName, strlen(roomName));
        bzero(roomCode, strlen(roomCode));

        do{
            do{
                __fpurge(stdin);
                printf("\n$ Room's name: ");
                fgets(roomName, 50, stdin);
                roomName[strlen(roomName) - 1] = '\0';
                if(checkLength(roomName) == 0){
                    printf("[-] Input must no null!\n");
                }
            }while(checkLength(roomName) == 0);
       
            do{
                __fpurge(stdin);
                printf("\n$ Room's code: ");
                fgets(roomCode, 50, stdin);
                roomCode[strlen(roomCode) - 1] = '\0';
                if(checkLength(roomCode) == 0){
                    printf("[-] Input must no null!\n");
                }
            }while(checkLength(roomCode) == 0);

            strcat(tmpbuff, roomName);
            strcat(tmpbuff, " ");
            strcat(tmpbuff, roomCode);

            sendMessToServer(tmpbuff);

            recvMessFormServer();
            
            printf("%-30s\n", recvBuff);
            if(strcmp(recvBuff, "success") == 0){
                clientComunicateHost();
                sendMessToServer("continue");
                break;
            }
            else{
                printf("$ Incorrect room's name or code! Check again!");
            }
            bzero(tmpbuff, strlen(tmpbuff));

        }while (strcmp(recvBuff, "fail") == 0);
    }
}

// function: room owner play with client
void hostComunicateClient(){

    // The loop ensures the game only starts when the room has client
    while (1){
        sleep(1);
        sendMessToServer("$ any one\n");
        recvMessFormServer();
        if(strcmp(recvBuff,  "ok") == 0) break;
    }
    // send connect signal to server
    sendMessToServer("connect please");
    
    // receive client's name
    recvMessFormServer();
    char optName[100];
    bzero(optName, 100);
    strcpy(optName, recvBuff);
    printf("\n$ Client is: %s\n", recvBuff);

    // make map
    printf("& Make yourself map:\n");
    makeMap();
    initViewOfMapOpt();

    // host defense turn 
    defenseClient(optName);
}

void clientComunicateHost(){
    sendMessToServer("connect please");
    
    // get room owner's name 
    recvMessFormServer();
    char optName[30];
    bzero(optName, 30);
    strcpy(optName, recvBuff);
    printf("\n$ Host la: %s\n", recvBuff);
    
    // make map
    printf("\n& Make yourself map: \n");
    makeMap();
    initViewOfMapOpt();

    // client shot turn 
    shotClient(optName);    
}

void shotClient(char optName[]){
    
    // input palace and send to server
    shot(&rowS, &colS);

    // receive result from server 
    recvMessFormServer();

    // if Opponent sent "endGame" signal to Server => send "iwin" signal to Server 
    if(strcmp(recvBuff, "endGame")==0){
        printf("\n& You win because shotted %s's heart\n", optName);
        sendMessToServer("iwin");
        return;
    }
    // if Opponent sent "boom" signal to Server => shot turn continue , else defense 
    else if(strcmp(recvBuff, "boom") == 0){
        mapOpt[rowS][colS] = -1000;
        printMapOpt(optName);
        shotClient(optName);
    }else if(strcmp(recvBuff, "miss") == 0){
        sendMessToServer("your turn");
        mapOpt[rowS][colS] = 1000;
        printMapOpt(optName);
        defenseClient(optName);
    }
    // else if(strcmp(recvBuff, "outRoom") == 0){
    //     printf("\n&Ban da thang vi doi thu roi khoi phong\n");
    //     return;
    // }
}

void defenseClient(char optName[]){

    // receive message from Server  
    recvMessFormServer();

    // if Opponent sent "outRoom" signal to Server => exit
    if(strcmp(recvBuff, "outRoom") == 0){
        printf("\n& You win! (%s has left room)\n", optName);
        recvMessFormServer();
        sendMessToServer("continue");
        return;
    }

    // if coordinates => Analysising => send 
    char *token;
    char *otherMes = malloc(MAX_STRING_LENGTH);
    bzero(otherMes, MAX_STRING_LENGTH);
    strcpy(otherMes, recvBuff);
    char colc[10], rowc[10];
    int col, row;

    token = strtok(otherMes, " ");
    strcpy(rowc, token);
    token = strtok(NULL, " ");
    strcpy(colc, token);

    row = atoi(rowc);col = atoi(colc);
    printf("$ %s shoted at: X(%d) - Y(%d)\n", optName, row, col);

    // if heart 
    if(map[row][col] == 2){
        printf("\n& %s shotted your heart! You lose\n", optName);
        initMap();
        printMap();
        sendMessToServer("endGame");
    }
    // if one of ship's palaces 
    else if(map[row][col] == 1){
        printf("\n& %s shotted boom! Defense continue\n", optName);
        map[row][col] = -1000;
        printMap();
        sendMessToServer("boom");
        defenseClient(optName);
    }else{
        printf("\n& %s shotted miss. Your turn!\n", optName);
        map[row][col] = 1000;
        printMap();
        printMapOpt(optName);
        sendMessToServer("miss");
        recvMessFormServer();
        shotClient(optName);
    }
}

// play with machine function
void playWithMachine(){
    sendMessToServer("playmachine");
    recvMessFormServer();
    printf("$ %s\n", recvBuff);

    makeMap();
    sendMessToServer("mapDone");

    int endGame = 0;
    recvMessFormServer();
    printf("\n& %s\n", recvBuff);
    initViewOfMapOpt();
    shot(&rowS, &colS);

    do{
        recvMessFormServer();
        if(strcmp(recvBuff, "endGame") == 0){
            printf("\n& Ban da chien thang!\n");
            endGame = 2;
            break;
        }
        else if(strcmp(recvBuff, "boom") == 0){
            endGame = 0;
            mapOpt[rowS][colS]=-1000;
            printMapOpt("Machine");
            shot(&rowS,&colS);

        }else if(strcmp(recvBuff, "machine_miss") == 0){
            endGame = 0;
            shot(&rowS,&colS);

        }else if(strcmp(recvBuff, "machine_boom") == 0){
            endGame = 1;
            defenseServer();
        }else if(strcmp(recvBuff, "miss") == 0){
            endGame = 1;
            mapOpt[rowS][colS] = 1000;
            defenseServer();
        }else if(strcmp(recvBuff, "machine_win") == 0){
            endGame = 2;
            break;
        }
    }while (endGame == 0 || endGame == 1);
    
    sendMessToServer("continue");
}

// shot function 
void shot(int *rowSend, int *colSend){
    int col, row;
    printf("\n& Your coordinates: \n");

    do{
        printf("& ROW: ");
        scanf("%d", &row);
        *rowSend=row;
        printf("& COL: ");
        scanf("%d", &col);
        *colSend=col;
        if(mapOpt[row][col]!=0){
            printf("[ERROR][This is %d-%d Shotted!] Shot again PLEASE!\n",row,col );
        }
    }while(mapOpt[row][col]!=0);

    char rowc[10], colc[10];
    char str[MAX_STRING_LENGTH];
    bzero(str, MAX_STRING_LENGTH);

    snprintf(rowc, 10, "%d", row);
    snprintf(colc, 10, "%d", col);
    strcat(str, rowc);
    strcat(str, " ");
    strcat(str, colc);

    bzero(sendBuff, MAX_STRING_LENGTH);
    strcpy(sendBuff, str);
    if(send(clientFD, sendBuff, strlen(sendBuff), 0) != strlen(sendBuff)){
        showError("send fail!");
    }
}

// defense from server function 
void defenseServer(){
    sendMessToServer("invite");
    recvMessFormServer();
    char *token;
    char colc[10], rowc[10];
    int col, row;

    token = strtok(recvBuff, " ");
    strcpy(rowc, token);
    token = strtok(NULL, " ");
    strcpy(colc, token);

    row = atoi(rowc);col = atoi(colc);
    printf("\n& Machine shotted at: %d - %d\n", row, col);
    if(map[row][col] == 2){
        printf("& Machine shotted heart! Machine win!");
        initMap();
        showMapBoomAll();
        sendMessToServer("cliLose");
    }
    else if(map[row][col] == 1){
        printf("\n& Machine shotted boom! Defense continue!\n");
        map[row][col] = -1000;
        printMap();
        sendMessToServer("boom");
    }else{
        printf("\n& Machine shotted miss! Your turn!\n");
        map[row][col]=+1000;
        printMap();
        printMapOpt("Machine");
        sendMessToServer("miss");
    }
}

// initilize map 
void initMap(){
    for(int i = 0 ; i < 10; i ++){
        for(int j = 0 ; j < 10; j ++){
            map[i][j] = 0;
        }
    }
}

void clearChar (int error){
    int ch=0;
    if(error != 1)
    {
        printf("[-]ERROR: Nhap so! PLEASE!\n");
        while ( ( ch = getchar()) != '\n' && ch != EOF) {
            //clear input buffer
        }
        if ( ch == EOF) {
            exit ( 1);
        }
    }
}
void clearChar (int error){
    int ch=0;
    if(error != 1)
    {
        printf("[-]ERROR: Nhap so! PLEASE!\n");
        while ( ( ch = getchar()) != '\n' && ch != EOF) {
            //clear input buffer
        }
        if ( ch == EOF) {
            exit ( 1);
        }
    }
}
void makeMap(){
    initMap();

    int ERROR = 0;
	int row,col;
	int CHEDO;
	int heartROW,heartcol;
	int error=0;

	printMap(map);
<<<<<<< HEAD
	printf("\n$ Heart palace:\n");
	// printf("[+] ROW: ");
	// scanf("%d",&heartROW);
	// printf("[+] COL: ");
	// scanf("%d",&heartcol);
     do{
        printf("[+] ROW: ");
         error=scanf("%d",&heartROW);
         clearChar(error);
         if(heartROW <0 || heartROW >9){
            printf("[-]ERROR: Nhap so tu 0=>9\n");
         }
    }while(heartROW <0 || heartROW >9||error !=1 );
    do{
        printf("[+] COL: ");
        scanf("%d",&heartcol);
        clearChar(error);
         if(heartcol <0 || heartcol >9){
            printf("[-]ERROR: Nhap so tu 0=>9\n" );
         }

    }while(heartcol <0 || heartcol >9||error !=1);
    
	map[heartROW][heartcol] = 2;
    for(int t=0; t<3; t++){
			if (heartcol !=0) {map[heartROW-1+t][heartcol-1]=-1;}
			if (heartcol !=9) {map[heartROW-1+t][heartcol+1]=-1;}
	}
	map[heartROW-1][heartcol]=-1;
	map[heartROW+1][heartcol]=-1;
=======
	printf("\n$ Dat so chi Huy\n");
    int error=0;
    
    do{
        printf("[+] ROW: ");
         error=scanf("%d",&sochihuyROW);
         clearChar(error);
         if(sochihuyROW <0 || sochihuyROW >9){
            printf("[-]ERROR: Nhap so tu 0=>9\n");
         }
    }while(sochihuyROW <0 || sochihuyROW >9||error !=1 );
    do{
        printf("[+] COL: ");
        scanf("%d",&sochihuycol);
        clearChar(error);
         if(sochihuycol <0 || sochihuycol >9){
            printf("[-]ERROR: Nhap so tu 0=>9\n" );
         }

    }while(sochihuycol <0 || sochihuycol >9||error !=1);

	map[sochihuyROW][sochihuycol] = 2;
	for(int t=0; t<3; t++){
			if (sochihuycol !=0) {map[sochihuyROW-1+t][sochihuycol-1]=-1;}
			if (sochihuycol !=9) {map[sochihuyROW-1+t][sochihuycol+1]=-1;}
	}
	map[sochihuyROW-1][sochihuycol]=-1;
	map[sochihuyROW+1][sochihuycol]=-1;
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c
	printMap(map);
    char *dateFormat[4] = {"st", "nd", "rd", "th"};

	for(int i= 4; i>0; i--){
		for(int j=4-i; j>=0; j--){
			ERROR =0;
			do{	
<<<<<<< HEAD
				printf("\n$ Putting Ship: %d%s thu %d \n",4-(i+j)+1, dateFormat[4 - (i + j)], i);
				if(i!=1){				
    				printf("$ Ship's mode: \n 1: Row \n 2: Col\n");
=======
				printf("\n$ Dat Thuyen: %d thu %d \n",i,4-(i+j)+1);
				if(i!=1){				
    				printf("$ Moi chon che do dat thuyen: \n 1: Ngang \n 2: Doc\n");
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c
    				
                    int error=0;
    				do{
    					error=scanf("%d",&CHEDO);
    					if(CHEDO!=1&&CHEDO!=2)
<<<<<<< HEAD
    						printf("[-]ERROR: Invalid mode. Choose 1 or 2.\n");
=======
    						printf("[-]ERROR: SAI CHEDO. Nhap 1 or 2.\n");
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c
                        clearChar(error);
    				}while((CHEDO!=1&&CHEDO!=2)||error!=1);
                }
				
<<<<<<< HEAD
				printf("\n$ Putting head of ship: \n");
=======
				printf("\n$ Moi nhap vi tri dau thuyen: \n");
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c
				 do{
                    printf("[+] ROW: ");
                    error=scanf("%d",&row);
                    clearChar(error);
                    if(row<0 || row>9){
<<<<<<< HEAD
                        printf("[-]ERROR: Number only 0=>9\n" );
=======
                        printf("[-]ERROR: Nhap lai trong khoang 0=>9\n" );
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c
                    }
                }while(row <0 ||row >9||error !=1 );
        	   do{
                    printf("[+] COL: ");
                    error=scanf("%d",&col );
                    clearChar(error);
                    if(col<0 || col>9){
<<<<<<< HEAD
                        printf("[-]ERROR: Number only 0=>9\n" );
=======
                        printf("[-]ERROR: Nhap lai trong khoang 0=>9\n" );
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c
                    }

                }while(col<0 ||col>9||error!=1 );
				if(CHEDO==1){
					ERROR=checkNgang(row,col,i);
				}else if(CHEDO==2){
					ERROR=checkDoc(row,col,i);

				}
			}while(ERROR == 1);

			updateMap(CHEDO,row,col,i);

			printMap(map);
        }
	}
    // map[0][0] = 2;
    // map[4][4] = 1;
    // printMap();
}

// check row palacement
int checkNgang(int row, int col,int i){
	if ((row>9||row<0)||(col > 10-i)|| col <0){
		printf("[-] Invalid placement!\n");
		printf("\n$ Please choose: 0 <= row <=9 \n 0 <= col <= 10-%d \n",i );

		return 1;
	}else{
		for(int t=0; t < i ; t++){
			if ( map[row][col + t] == 1 || map[row][col + t] == 2 ){
				printf("[-] Same palacement at: %d\n",t+1 );
				return 1;
			}
            if ( map[row ][col+t] == -1){
				printf("[-] Same %d's location banned \n",t+1);
				return 1;
			}
			if ( map[row ][col+t] == -1){
				printf("[-] ERROR: Ban da dat thuyen trung vao mot vi tri CAM tai toa thuyem thu: %d \n",t+1);
				return 1;
			}
		}
		return 0;
	}
}

// check col palacement 
int checkDoc(int row, int col,int i){
	if ((row>10 -i||row<0)||(col > 9)|| col <0){
		printf("[-] Invalid placement!\n");
		printf("[-] Please choose: 0 <= row <=10-%d and, 0 <= col <= 9 \n",i );

		return 1;
	}else{
		for(int t=0; t < i ; t++){
			if ( map[row + t][col] == 1 || map[row + t][col] == 2){
				printf("[-] Same palacement at: %d \n",t+1 );
				return 1;
			}
            if ( map[row + t][col] == -1){
				printf("[-] Same %d's location banned \n",t+1);
				return 1;
			}
			if ( map[row + t][col] == -1){
				printf("[-] ERROR: Ban da dat thuyen trung vao mot vi tri CAM  tai toa tau so:  %d \n",t+1);
				return 1;
			}
		}
		return 0;
	}
}

// print your map 
void printMap(){
    printf("\n--------------------------------\n");
    printf("[+]Your map:\n");
	printf("   0  1  2  3  4  5  6  7  8  9 \n");
	for( int i=0; i< 10; i++){
<<<<<<< HEAD
        printf("%d  ",i );
        for( int j=0;  j<10; j++){
            if (map[i][j]==-1000){
                printf("** ");
            }else if(map[i][j]==+1000){
                printf("-- " );
            }else if(map[i][j] == -1){
                printf("0  ");
            }
            else{
                printf("%d  ",map[i][j]);
            }
        }
        printf("\n");
	}
    printf("--------------------------------\n");
}
=======
		printf("%d  ",i );
		for( int j=0;  j<10; j++){
			if(map[i][j]==-1){
				printf("x  ");
			}else{
			printf("%d  ",map[i][j] );}
			
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c

void showMapBoomAll(){
     printf("\n--------------------------------\n");
    printf("[+]Your map:\n");
	printf("   0  1  2  3  4  5  6  7  8  9 \n");
	for( int i=0; i< 10; i++){
        printf("%d  ",i );
        for( int j=0;  j<10; j++){
           printf("** ");
        }
        printf("\n");
	}
    printf("--------------------------------\n");
}

// initilize oponent's map 
void initViewOfMapOpt(){
    for(int i = 0 ; i < 10; i ++){
        for(int j = 0 ; j < 10; j ++){
            mapOpt[i][j] = 0;
        }
    }
}

// show opponent's map 
void printMapOpt(char optName[]){
    printf("\n--------------------------------\n");
    printf("[+][%s's map]\n", optName);
    printf("   0  1  2  3  4  5  6  7  8  9 \n");
    for( int i=0; i< 10; i++){
        printf("%d  ",i );
        for( int j=0;  j<10; j++){
            if (mapOpt[i][j]==-1000){
                printf("** ");
            }else if(mapOpt[i][j]==+1000){
                printf("-- " );
            }
            else{
                printf("%d  ",mapOpt[i][j] );
            }

        }
        printf("\n");
    }
    printf("--------------------------------\n");
}

// update map after putting one palace 
void updateMap(int CHEDO, int row, int col, int ship_len){
<<<<<<< HEAD
    /*  xxxx
        x11x
        xxxx */
=======
/* xxxx
   x11x
   xxxx */
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c
	if(CHEDO == 1){
		for(int k= 0; k < ship_len; k++){
			map[row][col+k]=1;
			map[row+1][col+k]=-1;
			map[row-1][col+k]=-1;
			
		}
		for(int t=0; t<3; t++){
			if(col!=0){
				map[row-1+t][col-1]=-1;
			}
			if(col+ship_len-1!=9){
				map[row-1+t][col+ship_len]=-1;
			}
		}
	}
	/* x x x 
	   X 1 X
	   X 1 x
	   x x x */
	if(CHEDO ==2 ){
		for(int k= 0; k < ship_len; k++){
			map[row+k][col]=1;  
			if(col!=0){
				map[row+k][col-1]=-1;
			}
			if(col!=9){
				map[row+k][col+1]=-1;
			}
			if(col!=0) {map[row-1][col-1]=-1;}        map[row-1][col]=-1;         if(col!=9)  { map[row-1][col+1]=-1;}
			if(col!=0) {map[row+ship_len][col-1]=-1;} map[row+ship_len][col]=-1;  if(col!=9) {map[row+ship_len][col+1]=-1;}
			
<<<<<<< HEAD
=======
			
>>>>>>> 2c36085673b758ff28e18fbe5eb901a8d502825c
		}
		
	}
}

