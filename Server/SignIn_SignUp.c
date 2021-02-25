/*
******************************************************************
*    Battle Ship Game - HandleGame.c
*    Author:
*        +) Can Duc Loi
*        +) Truong Van Hung
*        +) Nguyen Thi Ngan 
*******************************************************************
*/


#include "Server.h"

/* 1. SignIn */

// check SignIn result (1 is succesfully)
int SignIn(ACC head, char user[], char pass[]){

    if(checkACCPass(head, user, pass) != -1){
        if(strcmp(getData(head, user).status, "0") == 0 || strcmp(getData(head, user).status, "2") == 0){
            return -1;
        }
        else{
            return 1;
        }
    }else{
        return -1;  
    }
    
}

// check user is online?
int checkCurOn(char user[]){
    for(int i = 0 ; i < curPos; i ++ ){
        if(strcmp(listOn[i]->userName, user) == 0) return 1;
    }
    return 0;
}

/* 2. SignUp */

// check signUp result (1 is successfully)
int registerACC(ACC head, char file[], char usn[], char pass[]){
   if(checkACC(head, usn) != -1){
       return -1;
   }else{
       head = addLast(head, creatNewData(usn, pass, "1", "group3_LTM.com"));
       writeFile(head, file);
       return 1;
   }
}


/* 3. SignOut */


// Set signin - sign up menu to send client 
void setSignInScreen(){
    char menu[MAX_STRING_LENGTH];
    bzero(menu, MAX_STRING_LENGTH);
    strcat(menu, "------------------------WELCOME-----------------------\n");
    strcat(menu, "--1) SignIn.                                         -\n");
    strcat(menu, "--2) SignUp.                                         -\n");
    strcat(menu, "------------------------------------------------------\n");
    bzero(sendBuff, MAX_STRING_LENGTH);
    strcpy(sendBuff, menu);  
    
}

// Sign-in, signup fuction 
void signInToSystem(ACC head, int cliSock, int *joinedStatus){
    setSignInScreen();
    if(send(cliSock, sendBuff, strlen(sendBuff), 0) != strlen(sendBuff)){
        showError("send fail!");
    }
    handleIncomingSingInOrSignOut(cliSock, head, &joinedStatus);
}

void handleIncomingSingInOrSignOut(int cliSock, ACC head, int **joinedStatus){
    char user[50], pass[10], rq[10];
    bzero(user, 50); bzero(pass, 10); bzero(rq, 10);
    const char s[2] = " ";
    char *token;

    char *mesSignIn = malloc(MAX_STRING_LENGTH);
    bzero(mesSignIn, MAX_STRING_LENGTH);
    strcpy(mesSignIn, recvMessFromClient(cliSock));

    if(strlen(mesSignIn) == 0){
        close(cliSock);
        cliSock = 0;
        return;
    }

    token = strtok(mesSignIn, s);
    strcpy(rq, token);
    token = strtok(NULL, s);
    strcpy(user, token);
    token = strtok(NULL, s);
    strcpy(pass, token);

    // Sign in
    if(strcmp(rq, "signIn") == 0){
        int signInRs;
        do{
            signInRs = SignIn(head, user, pass);
            if(signInRs == 1){
                int singInRs2 = checkCurOn(user);
                if(singInRs2 == 0){
                    sendMessToClient(cliSock, "signIn success");
                    addListOnline(cliSock, user);
                    **joinedStatus = 1;
                    break;
                } 
                else{
                    sendMessToClient(cliSock, "curOnline");
                    close(cliSock);
                    cliSock = 0;
                    break;
                }
            }else{
                sendMessToClient(cliSock, "fail");
                bzero(mesSignIn, MAX_STRING_LENGTH);
                strcpy(mesSignIn, recvMessFromClient(cliSock));

                if(strlen(mesSignIn) == 0){
                    close(cliSock);
                    cliSock = 0;
                    return;
                }

                bzero(user, 50);bzero(pass, 10);
                token = strtok(mesSignIn, s);
                strcpy(user, token);
                token = strtok(NULL, s);
                strcpy(pass, token);
            }
        }while(signInRs != 1 );
    }else{                      // Sign up 
        int signUpRs;
        do{
            signUpRs = registerACC(head, userDatabasePath, user, pass);
            if(signUpRs == 1){
                head = readFile(head, userDatabasePath);
                sendMessToClient(cliSock, "signUp success");
                
                bzero(mesSignIn, MAX_STRING_LENGTH);
                strcpy(mesSignIn, recvMessFromClient(cliSock));

                if(strlen(mesSignIn) == 0){
                    close(cliSock);
                    cliSock = 0;
                    return;
                }

                bzero(user, 50);bzero(pass, 10);
                token = strtok(mesSignIn, s);
                strcpy(user, token);
                token = strtok(NULL, s);
                strcpy(pass, token);

                int signInRs;
                do{
                    
                    signInRs = SignIn(head, user, pass);
                    if(signInRs == 1){
                        int singInRs2 = checkCurOn(user);
                        if(singInRs2 == 0){
                            sendMessToClient(cliSock, "signIn success");
                            addListOnline(cliSock, user);
                            **joinedStatus = 1;
                            break;
                        } 
                        else{
                            sendMessToClient(cliSock, "curOnline");
                            close(cliSock);
                            cliSock = 0;
                            break;
                        }
                    }else{
                        sendMessToClient(cliSock, "fail");
                        bzero(mesSignIn, MAX_STRING_LENGTH);
                        strcpy(mesSignIn, recvMessFromClient(cliSock));

                        if(strlen(mesSignIn) == 0){
                            close(cliSock);
                            cliSock = 0;
                            return;
                        }

                        bzero(user, 50);bzero(pass, 10);
                        token = strtok(mesSignIn, s);
                        strcpy(user, token);
                        token = strtok(NULL, s);
                        strcpy(pass, token);
                    }
                    
                }while(signInRs != 1 );
                break;
            }else{
                sendMessToClient(cliSock, "fail");
                bzero(mesSignIn, MAX_STRING_LENGTH);
                strcpy(mesSignIn, recvMessFromClient(cliSock));

                if(strlen(mesSignIn) == 0){
                    close(cliSock);
                    cliSock = 0;
                    return;
                }

                bzero(user, 50);bzero(pass, 10);
                token = strtok(mesSignIn, s);
                strcpy(user, token);
                token = strtok(NULL, s);
                strcpy(pass, token);
            }
        }while (signUpRs != 1);
    }

    bzero(mesSignIn, MAX_STRING_LENGTH);
    strcpy(mesSignIn, recvMessFromClient(cliSock));
    printf("%s\n",mesSignIn);

}

void signOutToSystem(int cliSock){
    char tmpStr[MAX_STRING_LENGTH];
    bzero(tmpStr, MAX_STRING_LENGTH);
    strcat(tmpStr,"\n------Do you want to exit?------\n");
    strcat(tmpStr, "-1. Yes                            -\n");
    strcat(tmpStr, "-2. No                             -\n");
    strcat(tmpStr, "------------------------------------\n");
    sendMessToClient(cliSock, tmpStr);
    char *mesSignOut = malloc(MAX_STRING_LENGTH);
    bzero(mesSignOut, MAX_STRING_LENGTH);
    strcpy(mesSignOut, recvMessFromClient(cliSock));
    if(strcmp(mesSignOut, "yes") == 0){
        sendMessToClient(cliSock, "bye");
    }
}   