#include "Client.h"

void showError(char errMes[]){
	char str[MAX_STRING_LENGTH];
	bzero(str, MAX_STRING_LENGTH);
	strcat(str, "[-] Error: ");
	strcat(str, errMes);
	strcat(str, "\n");
	write(1, str, 13 + strlen(errMes));
}

void recvMessFormServer(){
    bzero(recvBuff, MAX_STRING_LENGTH); 
	recv(clientFD, recvBuff, MAX_STRING_LENGTH, 0);
}

void sendMessToServer(char str[]){
    bzero(sendBuff, MAX_STRING_LENGTH);
    strcpy(sendBuff, str);
    if(send(clientFD, sendBuff, strlen(sendBuff), 0) != strlen(sendBuff)){
        showError("send fail!");
    }
}

void allHandle(){
    SingInSignUp();
    sendMessToServer("continue");
    if(signInRs == 1){
        handleIncomingInformation();
    }else{
        printf("$ Tai khoan dang online, khong the dang nhap!\n");
    }
    
}

int main(int argc, char *argv[]){


    if(checkParams(argc, argv[1]) == false) return 0;

    initializeClientSocket(argv[1]);    
    signInRs = 0;

    pthread_t allHandlePt;

    if(pthread_create(&allHandlePt, NULL, (void *)allHandle, NULL) != 0){
        printf("ERROR: pthread\n");
        return EXIT_FAILURE;
    }

    pthread_join(allHandlePt, NULL);

    close(clientFD);

    return 1;
}