//
// Created by Hadar Pinto 316460146
//
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <linux/random.h>
#include <sys/random.h>


void signalHandler(int sig);
void alarmHandler(int sig);
void readLineToArr(char* buf, char* arr, int fd);
int main(int argc, char **argv) {
    int serverID;
    serverID = atoi(argv[1]);

    signal(SIGALRM, alarmHandler);
    signal(SIGUSR2,signalHandler);
    alarm(30);

    int j;
    for(j = 1; j <= 10; j++){
        int randomWait = rand() % 6;
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
    printf("waiting for SIGUSR2\n");
    pause();
    exit(1);


}

void signalHandler(int sig){
    alarm(30);
    char fileAnswer[255];
    char answer[255];
    strcpy(fileAnswer, "to_client_");
    sprintf(fileAnswer, "%s%d", fileAnswer,getpid());
    strcat(fileAnswer,".txt");

    int fdAnswer = open(fileAnswer, O_RDWR | O_APPEND | O_CREAT, 0777);
    char d = 'a';
    int kk;
    kk =  read(fdAnswer, &d, 1);
    while (d != '\n') {
        printf("%c", d);
        read(fdAnswer, &d, 1);
    }
    printf("\n");

    close(fdAnswer);
    remove(fileAnswer);
    exit(1);
}

void alarmHandler(int sig){
    printf("Client closed because no response was received from the server for 30 seconds\n");
    exit(1);

}
