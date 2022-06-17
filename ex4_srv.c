//
// Created by Hadar Pinto 316460146
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/random.h>
#include <time.h>
void test();
void signalHandler (int signal);
void alarmHandler (int signal);
void readLineToArr(char* buf, char* arr, int fd);
int isDivisionByZero(char* operation, char* num2);
int calculate(int num1, int operation, int num2);

int main(int argc, char **argv) {
    signal(SIGUSR1, signalHandler);
    signal(SIGALRM, alarmHandler);
    alarm(60);
    while(1) {
        printf("Listening.......\n");
        pause();
        signal(SIGUSR1, signalHandler);
        alarm(60);
    }

}

void test() {
    printf("in son\n");
    //takecare of files
    int fdToSrvFile = open("to_srv.txt",O_RDWR | O_APPEND | O_CREAT, 0777);
    char clientPID[255]="", num1Char[255]="", operationChar[255]="", num2Char[255]="", *byteBuf, answerChar[255]="",toClientFileName[255]="";

    readLineToArr(byteBuf, clientPID, fdToSrvFile);
    readLineToArr(byteBuf, num1Char, fdToSrvFile);
    readLineToArr(byteBuf, operationChar, fdToSrvFile);
    readLineToArr(byteBuf, num2Char, fdToSrvFile);


    if((!strcmp("4",operationChar)) && (!strcmp("0",num2Char))){
        printf("division");
        printf(" by zero\n");
        strcpy(answerChar, "CANNOT_DIVIDE_BY_ZERO\n");
    }
    else {
        printf("not division\n");
        int num1Int, num2Int, operationInt, answerInt;
        num1Int = atoi(num1Char);
        operationInt = atoi(operationChar);
        num2Int = atoi(num2Char);
        answerInt = calculate(num1Int, operationInt, num2Int);
        sprintf(answerChar,"%d\n", answerInt);
    }

    //set file name according to client
    strcpy(toClientFileName, "to_client_");
    strcat(toClientFileName,clientPID);
    strcat(toClientFileName,".txt");


    int fdClientFile = open(toClientFileName, O_CREAT | O_RDWR, 0777);
    write(fdClientFile, answerChar, strlen(answerChar));
    close(fdClientFile);
    //send signal to client
    printf("send singal to client\n");
    int client = atoi(clientPID);
    kill(client, SIGUSR2);
    remove("to_srv.txt");
}
void signalHandler (int sig){
    int pid;
    pid = fork();
    if (pid < 0){
        exit(1);
    }
    else if(pid == 0){
        printf("in son\n");
        //takecare of files
        int fdToSrvFile = open("to_srv.txt",O_RDWR | O_APPEND | O_CREAT, 0777);
        char clientPID[255]="", num1Char[255]="", operationChar[255]="", num2Char[255]="", *byteBuf = 'a', answerChar[255]="",toClientFileName[255]="";

        readLineToArr(byteBuf, clientPID, fdToSrvFile);
        printf("clientPID %s", clientPID);
        readLineToArr(byteBuf, num1Char, fdToSrvFile);
        readLineToArr(byteBuf, operationChar, fdToSrvFile);
        readLineToArr(byteBuf, num2Char, fdToSrvFile);
        printf("num2Char %s", num2Char);


        if((!strcmp("4",operationChar)) && (!strcmp("0",num2Char))){
            printf("division");
            printf(" by zero\n");
            strcpy(answerChar, "CANNOT_DIVIDE_BY_ZERO\n");
        }
        else {
            printf("not division\n");
            int num1Int, num2Int, operationInt, answerInt;
            num1Int = atoi(num1Char);
            operationInt = atoi(operationChar);
            num2Int = atoi(num2Char);
            answerInt = calculate(num1Int, operationInt, num2Int);
            sprintf(answerChar,"%d\n", answerInt);
        }

        //set file name according to client
        strcpy(toClientFileName, "to_client_");
        strcat(toClientFileName,clientPID);
        strcat(toClientFileName,".txt");


        int fdClientFile = open(toClientFileName, O_CREAT | O_RDWR, 0777);
        write(fdClientFile, answerChar, strlen(answerChar));
        close(fdClientFile);
        //send signal to client
        printf("send singal to client\n");
        int client = atoi(clientPID);
        kill(client, SIGUSR2);
        remove("to_srv.txt");
        exit(1);
    }
    else {

    }



}

void readLineToArr(char* buf, char* arr, int fd) {
    int i=0, byte;
    byte = read(fd,&buf,1);
    while (buf != '\n') {
        arr[i++] = buf;
        byte = read(fd,&buf,1);
    }
    arr[i] = '\0';
}
int calculate(int num1, int operation, int num2){
    int sum;
    if (operation == 1){
        return num1 + num2;
    }
    else if (operation == 2){
        return num1 - num2;
    }
    else if (operation == 3){
        sum = num1*num2;
        return sum;
    }
    else if (operation == 4) {
        sum = num1/num2;
        return sum;
    }
}
void alarmHandler(int sig){
    printf("The server was closed because no service request was received for the last 60 seconds\n");
    while(wait(NULL) != -1){}
    exit(1);

}
int isDivisionByZero(char* operation, char* num2){
    if(!strcmp(operation,"4") && !strcmp(num2, "0")){
        return 1;
    }
    return 0;
}
