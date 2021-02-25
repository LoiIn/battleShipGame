#include "Server.h"

// All Server's services will send to Client 
void setMenuToShow(int cliSock){
    char menu[MAX_STRING_LENGTH];
    bzero(menu, MAX_STRING_LENGTH);
    strcat(menu, "\n\n");
    strcat(menu, "---------------------BATTLESHIP GAME----------------------\n");
    strcat(menu, "--1) Play with machine.                                  -\n");
    strcat(menu, "--2) Play with other player.                             -\n");
    strcat(menu, "--3) Create Room.                                        -\n");
    strcat(menu, "--4) SignOut.                                            -\n");
    strcat(menu, "----------------------------------------------------------\n");
    strcat(menu, "\n");    
    if(send(cliSock, menu, strlen(menu), 0) != strlen(menu)){
            showError("send fail!");
    }
}


// Initialize Server 
void initializeServerSocket(){

    // Innitialize server socket that use TCP protocol
    if((serverSocketFD =  socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
        showError("Fail to open socket!");
        return;
    }

    int opt = 1;
    if(setsockopt(serverSocketFD, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0){
        showError("Fail to setsockopt!");
        exit(EXIT_FAILURE);
    }

    // socket address used for the server
    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr)); 
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // allow all networks's address 
    serverAddr.sin_port = htons(PORT);    
    serverAddr.sin_family = AF_INET;  

    // binding server
    if(bind(serverSocketFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0){
        showError("Fail to bind!");
        return;
    }

    // allows maximum clients to connect to server
    if (listen(serverSocketFD, MAX_CLIENTS) < 0){   
        showError("Fail to Listen!");  
        exit(EXIT_FAILURE);   
    }

    printf("$ Server wait connecting ...\n");

}

// Handle each client's connection
int handleIncomingConnections(){
    
    // Initilize new socket 
    int newSocket = 0;
    struct sockaddr_in cli_addr;
    bzero(&cli_addr, sizeof(cli_addr));
    int clientLen = sizeof(cli_addr);

    // accept new socket
    if((newSocket = accept(serverSocketFD, (struct sockaddr *)&cli_addr, (socklen_t *)&clientLen)) < 0){
        showError("Fail to accept new socket!");
        exit(0);
    }

   return newSocket;
}

// handle all game
void handleIncomingInformations(ACC head, int cliSock, int joinedStatus){

    char *mesClient = malloc(MAX_STRING_LENGTH);
    bzero(mesClient, MAX_STRING_LENGTH);
    // strcpy(mesClient, recvMessFromClient(cliSock));
    // printf("Mes: %s\n", mesClient);

    // Internal processing loop 
    do{
        
        setMenuToShow(cliSock);
        bzero(mesClient, MAX_STRING_LENGTH);
        strcpy(mesClient, recvMessFromClient(cliSock));

        if(strlen(mesClient) == 0){
            closeSocket(cliSock);
            break;
        }

        // play with machine service 
        if(strcmp(mesClient, "playmachine") == 0){
            playWithMachine(cliSock);

            if(getPosOfUser(cliSock) == -1){
                closeSocket(cliSock);
                return;
            }

            bzero(mesClient, MAX_STRING_LENGTH);
            strcpy(mesClient, recvMessFromClient(cliSock));
        }

        // join other's room service 
        else if(strcmp(mesClient, "joinRoom") == 0){

            joinGameInServer(head, cliSock);

            if(getPosOfUser(cliSock) == -1){
                return;
            }

            bzero(mesClient, MAX_STRING_LENGTH);
            strcpy(mesClient, recvMessFromClient(cliSock));

        }

        // create room service 
        else if(strcmp(mesClient, "createRoom") == 0){

            createRoomInServer(head, cliSock);
            
            if(getPosOfUser(cliSock) == -1){
                return;
            }

            bzero(mesClient, MAX_STRING_LENGTH);
            strcpy(mesClient, recvMessFromClient(cliSock));

        }

        // sign out service
        else if(strcmp(mesClient, "signOut") == 0){
            signOutToSystem(cliSock);
        }

        // exit 
        if(strcmp(mesClient, "continue") != 0){
            closeSocket(cliSock);
            break;
        }
        
    }while (serverLive == 1);
        
}

// Close client's socket 
void closeSocket(int cliSock){
    printf("\n$ %s is disconect!\n", listOn[getPosOfUser(cliSock)]->userName);
    
    // Delete all room (peole creating or joining)
    removeRoomHaveByOnePeople(cliSock);

    // Delete user from list online user
    removeListOnline(cliSock);

    // close socket 
    close(cliSock);

    // set clisock to 0
    cliSock = 0;
    return;
}