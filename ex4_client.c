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


void alarmHandler(int sig);
void readLineToArr(char* buf, char* arr, int fd);
int main(int argc, char **argv) {
    int serverID, num1, operation, num2;
    serverID = atoi(argv[1]);

    signal(SIGALRM, alarmHandler);
    alarm(30);

    int j;
    for(j = 1; j <= 10; j++){
        int randomWait = rand() % 6;
        printf("%d\n", randomWait);
        sleep(randomWait);
        //make file to_srv
        int fd = open("to_srv.txt", O_CREAT | O_WRONLY | O_RDONLY | O_EXCL, 0777);
        if (fd == -1){
            if (j == 10){
                printf("Couldnt open file... now exiting\n");
                // intructions says to remove files... which??
            }
            continue;
        }

        //insert lines
        char pid[255];
        sprintf(pid, "%d\n", getpid());
        write(fd, pid, strlen(pid));
        int i;
        for (i = 2; i <= 4; i++) {
            char line[255];
            sprintf(line, "%s\n", argv[i]);
            write(fd, line, strlen(line));

        }
        break;

    }

    kill(serverID,SIGUSR1);
    pause();
    char fileAnswer[255];
    strcpy(fileAnswer, "to_client_");
    sprintf(fileAnswer, "%s%d", fileAnswer,getpid());
    strcat(fileAnswer,".txt");

    int fdAnswer = open(fileAnswer, O_RDONLY);
    char Answer[255], *byte_buf;
    readLineToArr(byte_buf,Answer,fdAnswer);
    printf("Answer is: %s\n", Answer);
    //remove(fileAnswer);



}

void alarmHandler(int sig){
    printf("Client closed because no response was received from the server for 30 seconds\n");
    exit(1);

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