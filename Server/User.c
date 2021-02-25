/*
******************************************************************
*    Battle Ship Game - User.c
*    Author:
*        +) Can Duc Loi
*        +) Truong Van Hung
*        +) Nguyen Thi Ngan 
*******************************************************************
*/

#include "User.h"

// create new data 
userData creatNewData(char *usN, char *pass, char *sts, char *page){
    userData newData;
    strcpy(newData.userName, usN);
    strcpy(newData.pass, pass);
    strcpy(newData.status, sts);
    strcpy(newData.page, page);
    newData.joinedGame = 0;
    newData.wonGame = 0;
    newData.rank = 0;
    return newData;
}

// coppy new data from old data
userData coppyData(userData sample){
    userData newData;
    strcpy(newData.userName, sample.userName);
    strcpy(newData.pass, sample.pass);
    strcpy(newData.status, sample.status);
    strcpy(newData.page, sample.page);
    newData.joinedGame = sample.joinedGame;
    newData.wonGame = sample.wonGame;
    newData.rank = sample.rank;
    return newData;
}

// check exist account ( -1 if not exist)
int checkACC(ACC head, char *userName){
    int position = 0;
    for(ACC p = head; p != NULL; p = p->next){
        if(strcmp((p->accData).userName, userName) == 0){
            return position;
        }
        ++position;
    }
    return -1;
}

// check password of one account (-1 if pass is incorrect)
int checkACCPass(ACC head,char *usN, char *pass){
    int position = 0;
    for(ACC p = head; p != NULL; p = p->next){
        if(strcmp((p->accData).pass, pass) == 0 && strcmp((p->accData).userName, usN) == 0){
            return position;
        }
        ++position;
    }
    return -1;
}

// get data of one account 
userData getData(ACC head, char *usN){
    ACC p = head;
    userData rsData;
    while( p != NULL){
        if(strcmp((p->accData).userName, usN) == 0){
            rsData = coppyData(p->accData);
            break;
        }
        p = p->next;
    }
    return rsData;
}
 
// create new account 
ACC createNewACC(ACC head, userData newData){
    if(checkACC(head, newData.userName) != -1) return NULL;
    ACC newACC;
    newACC = (ACC)malloc(sizeof(struct nodeACC));
    newACC -> accData = newData;
    newACC -> next = NULL;
    return newACC;
}
 
// linklist: add new account to end of list 
ACC addLast(ACC head, userData newData){
    ACC tmp, p;
    tmp = createNewACC(head, newData);
    if(head == NULL) head = tmp;
    else{
        p = head;
        while ( p -> next != NULL){
            p = p -> next;
        }
        p -> next = tmp;
    }
    return head;
}

// linklist: add new account to head of list 
ACC addHead(ACC head, userData newData){
    ACC tmp = createNewACC(head, newData);
    if(head == NULL){
        head = tmp; 
    }else{
        tmp->next = head; 
        head = tmp; 
    }
    return head;
}
 
// linklist: add new account to random palace of list 
ACC addAtRandom(ACC head, userData newData, int position){
    if(position == 0 || head == NULL){
        head = addHead(head, newData); 
    }else{
        int k = 1;
        ACC p = head;
        while(p != NULL && k != position){
            p = p->next;
            ++k;
        }
        if(k != position){
            head = addLast(head, newData);
        }else{
            ACC tmp = createNewACC(head, newData);
            tmp -> next = p -> next;
            p->next = tmp;
        }
    }
    return head;
}

// linklist: delete one account at head
ACC deleteHead(ACC head){
    if(head == NULL){
        printf("List ACC is empty! Can't remove");
    }else{
        head = head->next;
    }
    return head;
}
 
// linklist: delete one account at end list
ACC deleteLast(ACC head){
    if (head == NULL || head->next == NULL){
         return deleteHead(head);
    }

    ACC p = head;
    while(p->next->next != NULL){
        p = p->next;
    }
    p->next = p->next->next; 
    
    return head;
}
 
// linklist: delete one account at random palace
ACC deleteAtRadom(ACC head, int position){
    if(position == 0 || head == NULL || head->next == NULL){
        head = deleteHead(head); 
    }else{
        int k = 1;
        ACC p = head;
        while(p->next->next != NULL && k != position){
            p = p->next;
            ++k;
        }
        if(k != position){
            head = deleteLast(head);
        }else{
            p->next = p->next->next;
        }
    }
    return head;
}

// show all user joined system 
void showList(ACC head){
    printf("\nTest Data:\n");
    printf("-------------------------------------------\n");
    for(ACC p = head; p != NULL; p = p->next){
        printf("%-30s%-10s%-5s%-30s \n", (p->accData).userName, (p->accData).pass, (p->accData).status, (p->accData).page);
    }
    printf("-------------------------------------------\n");
}

// initilize link list 
ACC initHead(){
    ACC head;
    head = NULL;
    return head;
}
 
// get list's length
int getLengthOfList(ACC head){
    int length = 0;
    for(ACC p = head; p != NULL; p = p->next){
        ++length;
    }
    return length;
}

// read list of user from database
ACC readFile(ACC head, char file[]){
    FILE *f = fopen(file, "r");
    if( f == NULL){
        printf("\n===>Can't open this file to read!\n");
        return NULL;
    }else{
        while (!feof(f)){
            userData newData;
            char joinedGame[10], wonGame[10], rank[10];
            fscanf(f, "%s %s %s %s %s %s %s", newData.userName, newData.pass, newData.status, newData.page, joinedGame, wonGame, rank);
            newData.joinedGame = atoi(joinedGame);
            newData.wonGame = atoi(wonGame);
            newData.rank = atof(rank);

            head = addLast(head, newData);
        }
        fclose(f);
        return head;
    }
}

// write cur link list to database
void writeFile(ACC head, char file[]){
    FILE *f = fopen(file, "w+");
    if( f == NULL){
        printf("\n===>Can't open this file to write!\n");
        return;
    }else{
        ACC p = head;
        while (p != NULL){
            char line[MAX_USER_NAME_LENGTH + MAX_PASS_LENGTH + 100] = "";
            char joinedGame[10], wonGame[10], rank[10];
            snprintf( joinedGame, 10, "%d", (p->accData).joinedGame);
            snprintf( wonGame, 10, "%d", (p->accData).wonGame);
            snprintf( rank, 10, "%lf", (p->accData).rank);

            strcat(line, (p->accData).userName);
            strcat(line, "          ");
            strcat(line, (p->accData).pass);
            strcat(line, "          ");
            strcat(line, (p->accData).status);
            strcat(line, "          ");
            strcat(line, (p->accData).page);
            strcat(line, "          ");
            strcat(line, joinedGame);
            strcat(line, "          ");
            strcat(line, wonGame);
            strcat(line, "          ");
            strcat(line, rank);

            strcat(line, "\n");
            fputs(line, f);
            p = p -> next;
        }
        fclose(f);
    }
}

// update info of one account 
ACC updateACC(ACC head, char *usN, int win){
    int pos = checkACC(head, usN);
    userData odlInfo = getData(head, usN);

    head = deleteAtRadom(head, pos);

    userData newInfo;
    strcpy(newInfo.userName, usN);
    strcpy(newInfo.pass, odlInfo.pass);
    strcpy(newInfo.status, odlInfo.status);
    strcpy(newInfo.page, odlInfo.page);
    newInfo.joinedGame = odlInfo.joinedGame + 1;
    if(win == 1){
        newInfo.wonGame = odlInfo.wonGame + 1;
    }else{
        newInfo.wonGame = odlInfo.wonGame;
    }
    newInfo.rank = (double) newInfo.wonGame / newInfo.joinedGame;

    head = addAtRandom(head, newInfo, pos);
}

/*online user function*/

// show online user's list
void showListOnline(){
    for(int i = 0 ; i < curPos; i ++){
        printf("%d. %d - %s - %d\n",i + 1, listOn[i]->cliSock, listOn[i]->userName, listOn[i]->joinedStatus);
    }
}

// add one user to list online after signed in 
void addListOnline(int cliSock, char userName[]){
    listOn[curPos] = (userOnline *)malloc(sizeof(userOnline));
    listOn[curPos]->cliSock = cliSock;
    listOn[curPos]->joinedStatus = 0;
    strcpy(listOn[curPos]->userName, userName);
    curPos ++;
}

// remove one user to list online after signed out
void removeListOnline(int cliSock){
    for(int i = 0 ; i  < curPos; i++){
        if(listOn[i]->cliSock == cliSock){
            listOn[i] = NULL;
            for(int j = i; j < curPos - 1; j ++){
                listOn[j] = listOn[j+1];
            }
            listOn[curPos] = NULL;
        }
    }
    curPos --;
}

// get clisocket of user by name
int getCliSockByName(char userName[]){
    for(int i = 0; i < curPos; i ++){
        if(strcmp(listOn[i]->userName, userName) == 0){
            return listOn[i]->cliSock;
        }
    }
    return 0;
}

// get position of user by clisocket
int getPosOfUser(int cliSock){
    for(int i = 0; i < curPos; i ++){
        if(listOn[i]->cliSock == cliSock){
            return i;
        }
    }
    return -1;
}

// get name of user by clisocket
char *getUserName(int cliSock){
    return listOn[getPosOfUser(cliSock)]->userName;
}
