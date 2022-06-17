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
#define MAX_CHAR 255
#define WAIT_TIME 60
#define ADD 1
#define SUB 2
#define MUL 3
#define DIV 4

void signalHandler (int signal);
void alarmHandler (int signal);
void readLineToArr(char* buf, char* arr, int fd);
int isDivisionByZero(char* operation, char* num2);
int calculate(int num1, int operation, int num2);

int main(int argc, char **argv) {
    signal(SIGUSR1, signalHandler);
    signal(SIGALRM, alarmHandler);
    alarm(WAIT_TIME);
    while(1) {
        pause();
        signal(SIGUSR1, signalHandler);
        alarm(WAIT_TIME);
    }
}

void signalHandler (int sig){
    int pid;
    pid = fork();
    if (pid < 0){
        printf("ERROR_FROM_EX4\n");
        exit(1);
    }
    else if(pid == 0){
        //take care of files
        int fdToSrvFile = open("to_srv.txt",O_RDWR | O_APPEND | O_CREAT, 0777);
        char clientPID[MAX_CHAR]="", num1Char[MAX_CHAR]="", operationChar[MAX_CHAR]="", num2Char[MAX_CHAR]="",
        *byteBuf = 'a', answerChar[MAX_CHAR]="",toClientFileName[MAX_CHAR]="";

        readLineToArr(byteBuf, clientPID, fdToSrvFile);
        readLineToArr(byteBuf, num1Char, fdToSrvFile);
        readLineToArr(byteBuf, operationChar, fdToSrvFile);
        readLineToArr(byteBuf, num2Char, fdToSrvFile);

        if((!strcmp("4",operationChar)) && (!strcmp("0",num2Char))){
            strcpy(answerChar, "CANNOT_DIVIDE_BY_ZERO\n");
        }
        else {
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
    if (operation == ADD){
        return num1 + num2;
    }
    else if (operation == SUB){
        return num1 - num2;
    }
    else if (operation == MUL){
        sum = num1*num2;
        return sum;
    }
    else if (operation == DIV) {
        sum = num1/num2;
        return sum;
    }
}
void alarmHandler(int sig){
    printf("The server was closed because no service request was received for the last 60 seconds\n");
    while(wait(NULL) != -1){}
    exit(1);

}
