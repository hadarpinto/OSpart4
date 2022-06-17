//
// Created by Hadar Pinto 316460146
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <signal.h>
//#include <sys/types.h>
//#include <unistd.h>
//#include <dirent.h>
//#include <sys/wait.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <sys/random.h>
//#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <linux/random.h>
#include <sys/random.h>

void test(int s);
void signalHandler(int sig);
void alarmHandler(int sig);
void readLineToArr(char* buf, char* arr, int fd);
int main(int argc, char **argv) {
    int serverID;
    serverID = atoi(argv[1]);
    printf("%d my pid\n", getpid());

    signal(SIGALRM, alarmHandler);
    signal(SIGUSR2,signalHandler);
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
    printf("waiting for SIGUSR2\n");

    printf("pausing............\n");
    pause();
    printf("after pausing............\n");

    exit(1);


}
void test(int s) {
  //  alarm(30);
    printf("been here............\n");
    char fileAnswer[255]="";
    char answer[255], *byte_buf;
    //strcpy(fileAnswer, "aa.txt");

    printf("filename is %s\n", fileAnswer);

    int fdAnswer = open("aa.txt", O_RDWR | O_APPEND | O_CREAT, 0777);
    char r = 'a';
    long kk;
    kk =  read(fdAnswer, &r, 1);

        while (r != '\n'){
            printf("%c", r);
            kk =  read(fdAnswer, &r, 1);
        }



    close(fdAnswer);
    remove(fileAnswer);
    exit(0);
}
void signalHandler(int sig){
    //alarm(30);
    printf("been here............\n");
    char fileAnswer[255];
    char answer[255], *byte_buf;
    strcpy(fileAnswer, "to_client_");
    sprintf(fileAnswer, "%s%d", fileAnswer,getpid());
    strcat(fileAnswer,".txt");
    printf("filename is %s\n", fileAnswer);

    int fdAnswer = open(fileAnswer, O_RDWR | O_APPEND | O_CREAT, 0777);
    char d = 'a';
    int kk;
    kk =  read(fdAnswer, &d, 1);
        while (d != '\n'){
            printf("%c", d);
            read(fdAnswer, &d, 1);
        }



    close(fdAnswer);
    remove(fileAnswer);
    exit(0);



//    printf("filename is %d\n", fdAnswer);
//    strcpy(answer,"hey\0");
//    printf("Answer is: %s\n", answer);
//    printf("im doing trouble when i equal too\n");
//    printf("filename is %s\n", fileAnswer);
//    //readLineToArr(byte_buf,answer,fdAnswer);
//
//    printf("im doing trouble when i equal too ");
//    int i=0, byte;
//    byte = read(fdAnswer,&byte_buf,1);
//
//    while (byte_buf != '\n' || byte == -1) {
//        answer[i++] = byte_buf;
//        byte = read(fdAnswer,&byte_buf,1);
//    }
//    answer[i] = '\0';
//
//
//    printf("Answer is: %s\n", answer);
//    close(fdAnswer);
    //remove(fileAnswer);

}

void alarmHandler(int sig){
    printf("Client closed because no response was received from the server for 30 seconds\n");
    exit(1);

}

void readLineToArr(char* buf, char* arr, int fd) {
    printf("im doing trouble when i equal too ");
    int i=0, byte;
    byte = read(fd,&buf,1);

    while (buf != '\n' || byte == -1) {
        arr[i++] = buf;
        byte = read(fd,&buf,1);
    }
    arr[i] = '\0';

}