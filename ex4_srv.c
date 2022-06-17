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

void test(int s);
void signalHandler (int signal);
void alarmHandler (int signal);
void readLineToArr(char* buf, char* arr, int fd);
int isDivisionByZero(char* operation, char* num2);
int calculate(int num1, int operation, int num2);

int main(int argc, char **argv) {
//    signal(SIGUSR1, signalHandler);
//    alarm(60);
//    while(1) {
//        printf("Listening.......\n");
//        pause();
//
//    }
    test(3);
}

void test(int s){
    int fdToSrvFile = open("to_srv.txt",O_RDONLY, 0777);
    char clientPID[255]="", num1Char[255]="", operationChar[255]="", num2Char[255]="", *byteBuf, answerChar[255]="",toClientFileName[255]="";
    readLineToArr(byteBuf, clientPID, fdToSrvFile);

    readLineToArr(byteBuf, num1Char, fdToSrvFile);

    readLineToArr(byteBuf, operationChar, fdToSrvFile);

    readLineToArr(byteBuf, num2Char, fdToSrvFile);

    if (!isDivisionByZero(operationChar, num2Char)) {
        strcpy(answerChar, "CANNOT_DIVIDE_BY_ZERO\n");

        exit(1);
    }
        //not a division by zero, assign answerChar with answer
    else {
        printf("calculting....\n");
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
    printf("%s", toClientFileName);
    int writeint = write(fdClientFile, answerChar, strlen(answerChar));
    printf("%d writeint \n",writeint);
    //close(fdClientFile);
    //send signal to client
    int client = atoi(clientPID);
    printf("im at bottom handler %d\n",client);
    kill(client, SIGUSR2);
    remove("to_srv.txt");




}
void signalHandler (int sig){
    signal(SIGUSR1, signalHandler);
    alarm(60);
    printf("in signalHandler\n");

    int pid;
    pid = fork();
    if (pid < 0){
        exit(1);
    }
    else if(pid == 0){
        //takecare of files
        printf("in son\n");
        int fdToSrvFile = open("to_srv.txt",O_RDONLY, 0777);
        char clientPID[255]="", num1Char[255]="", operationChar[255]="", num2Char[255]="", *byteBuf, answerChar[255]="",toClientFileName[255]="";
        readLineToArr(byteBuf, clientPID, fdToSrvFile);
        printf("client PID is %s\n",clientPID);
        readLineToArr(byteBuf, num1Char, fdToSrvFile);
        printf("client PID is %s\n",num1Char);
        readLineToArr(byteBuf, operationChar, fdToSrvFile);
        printf("client PID is %s\n",operationChar);
        readLineToArr(byteBuf, num2Char, fdToSrvFile);
        printf("client PID is %s\n",num2Char);
        printf("in son2\n");
        if (!isDivisionByZero(operationChar, num2Char)) {
            strcpy(answerChar, "CANNOT_DIVIDE_BY_ZERO\n");

            exit(1);
        }
        //not a division by zero, assign answerChar with answer
        else {
            printf("calculting....\n");
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
        printf("%s", toClientFileName);
        int writeint = write(fdClientFile, answerChar, strlen(answerChar));
        printf("%d writeint \n",writeint);
        //close(fdClientFile);
        //send signal to client
        int client = atoi(clientPID);
        printf("im at bottom handler %d\n",client);
        kill(client, SIGUSR2);
        remove("to_srv.txt");

    }

}

void readLineToArr(char* buf, char* arr, int fd) {
    int i=0, byte;
    byte = read(fd,&buf,1);
    printf("%d byte\n", byte);
    while (buf != '\n' || byte == -1 || byte == 0) {
        arr[i++] = buf;
        byte = read(fd,&buf,1);
        printf("%d byte\n", byte);
    }
    arr[i] = '\0';
    printf("FAiled\n");
}
int calculate(int num1, int operation, int num2){
    printf("in calculate\n");
    if (operation == 1){
        return num1 + num2;
    }
    else if (operation == 2){
        return num1 - num2;
    }
    else if (operation == 3){
        return num1 * num2;
    }
    else if (operation == 4) {
        return num1 / num2;
    }
}
void alarmHandler(int sig){
    printf("The server was closed because no service request was received for the last 60 seconds\n");
    exit(1);

}
int isDivisionByZero(char* operation, char* num2){
    if(strcmp(operation,"4") && strcmp(num2, "0")){
        return 1;
    }
    return 0;
}
