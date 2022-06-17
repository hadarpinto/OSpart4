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


void signalHandler (int signal);
void readLineToArr(char* buf, char* arr, int fd);
int isDivisionByZero(char* operation, char* num2);
int calculate(int num1, int operation, int num2);

int main(int argc, char **argv) {
    signal(SIGUSR1, signalHandler);
    while(1) {
        pause();

    }
}


void signalHandler (int signal){
    signal(SIGUSR1, signalHandler);
    int pid;
    pid = fork();
    if (pid < 0){
        exit(1);
    }
    else if(pid == 0){
        //takecare of files
        int fdToSrvFile = open("to_srv",O_RDONLY, 0777);
        char clientPID[255], num1Char[255], operationChar[255], num2Char[255], *byteBuf, answerChar[255],toClientFileName[255];
        readLineToArr(byteBuf, clientPID, fdToSrvFile);
        readLineToArr(byteBuf, num1Char, fdToSrvFile);
        readLineToArr(byteBuf, operationChar, fdToSrvFile);
        readLineToArr(byteBuf, num2Char, fdToSrvFile);
        if (isDivisionByZero(operationChar, num2Char)) {
            strcpy(answerChar, "CANNOT_DIVIDE_BY_ZERO\n");

            exit(1);
        }
        //not a division by zero, assign answerChar with answer
        else {
            int num1Int, num2Int, operationInt, answerInt;
            num1Int = atoi(num1Char);
            operationInt = atoi(operationChar);
            num2Int = atoi(num2Char);
            answerInt = calculate(num1Int, operationInt, num2Int);
            sprintf(answerChar,"%d", answerInt);
        }

        //set file name according to client
        strcpy(toClientFileName, "to_client");
        strcat(toClientFileName,clientPID);
        strcat(toClientFileName,".txt");

        int fdClientFile = open(toClientFileName, O_CREAT | O_WRONLY|O_RDONLY, 0777);
        write(fdClientFile, answerChar, strlen(answerChar));

        //send signal to client
        kill(atoi(clientPID), SIGUSR1);
        remove("to_srv.txt");

    }
    else {
        int status;

    }
    //fork statements.





}

void readLineToArr(char* buf, char* arr, int fd) {
    int i=0, byte;
    byte = read(fd,&buf,1);
    while (buf != '\n' || byte == -1) {
        arr[i++] = buf;
        byte = read(fd,&buf,1);
    }
    arr[i] = '\0';
}
int calculate(int num1, int operation, int num2){
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

int isDivisionByZero(char* operation, char* num2){
    if(strcmp(operation,"4") && strcmp(num2, "0")){
        return 1;
    }
    return 0;
}
