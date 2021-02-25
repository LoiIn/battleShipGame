/*
******************************************************************
*    Battle Ship Game - SingIn_SignUp.c
*    Author:
*        +) Can Duc Loi
*        +) Truong Van Hung
*        +) Nguyen Thi Ngan 
*******************************************************************
*/


#include "Client.h"

void SingInSignUp(){

    recvMessFormServer();
	
    char str[10];
    int choice;
    do{
        printf("%s", recvBuff);
        __fpurge(stdin);
        bzero(str, 10);
        printf("\nYour choice: ");
        fgets(str, 10, stdin);
        choice = atoi(str);
    }while(choice != 1 && choice != 2);
    
    char user[50], pass[10], tmpbuff[100];
    bzero(tmpbuff, strlen(tmpbuff));
    bzero(user, strlen(user));
    bzero(pass, strlen(pass));

    choice == 1 ? strcat(tmpbuff, "signIn") : strcat(tmpbuff, "signUp");
    strcat(tmpbuff, " ");

    do{
       do{
            __fpurge(stdin);
            printf("\n$ Username: ");
            fgets(user, 50, stdin);
            user[strlen(user) - 1] = '\0';
            if(checkLength(user) == 0){
                printf("[-] Input must no null!\n");
            }
        }while(checkLength(user) == 0);
        do{
            __fpurge(stdin);
            printf("$ Password: ");
            fgets(pass, 50, stdin);
            pass[strlen(pass) - 1] = '\0';
            if(checkLength(pass) == 0){
                printf("[-] Input must no null!\n");
            }
        }while(checkLength(pass) == 0);
        strcat(tmpbuff, user);
        strcat(tmpbuff, " ");
        strcat(tmpbuff, pass);

        sendMessToServer(tmpbuff);

        recvMessFormServer();
        bzero(tmpbuff, strlen(tmpbuff));

        if(strcmp(recvBuff, "signIn success") == 0){
            signInRs = 1;
            printf("$ Sign in successfully!\n");
            // sendMessToServer("continue");
            break;
        }

        else if(strcmp(recvBuff, "curOnline") == 0){
            signInRs = 0;
            break;
        }

        else if(strcmp(recvBuff, "signUp success") == 0){
            printf("$ Sign up successfully!\n");
            printf("$ Please Sign In:\n");
            do{
                do{
                     __fpurge(stdin);
                    printf("\n$ Username: ");
                    fgets(user, 50, stdin);
                    user[strlen(user) - 1] = '\0';
                    if(checkLength(user) == 0){
                        printf("[-] Input must no null!\n");
                    }
                }while(checkLength(user) == 0);
               
                do{
                    __fpurge(stdin);
                    printf("$ Password: ");
                    fgets(pass, 50, stdin);
                    pass[strlen(pass) - 1] = '\0';
                    if(checkLength(pass) == 0){
                        printf("[-] Input must no null!\n");
                    }
                }while(checkLength(pass) == 0);

                strcat(tmpbuff, user);
                strcat(tmpbuff, " ");
                strcat(tmpbuff, pass);

                sendMessToServer(tmpbuff);
                recvMessFormServer();

                if(strcmp(recvBuff, "signIn success") == 0){
                    signInRs = 1;
                    printf("$ SignIn successfully!\n");
                    // sendMessToServer("continue");
                    return;
                }
                printf("$ Server: %s\n", recvBuff);
                bzero(tmpbuff, strlen(tmpbuff));
            }while (strcmp(recvBuff, "fail") == 0);
        }else{
            printf("$ The account is exiisted! Try again\n");
        }
    }while (strcmp(recvBuff, "fail") == 0);

}

void SignOut(){
    sendMessToServer("signOut");
    recvMessFormServer();
    char str[10];
    int choice;
    do{
        printf("%s", recvBuff);
        __fpurge(stdin);
        bzero(str, 10);
        printf("\n$ Please choose 1 or 2: ");
        fgets(str, 10, stdin);
        choice = atoi(str);
    }while(choice != 1 && choice != 2);

    choice == 1 ? sendMessToServer("yes") : sendMessToServer("continue");
    recvMessFormServer();
    printf("$ %-30s\n", recvBuff);
}
