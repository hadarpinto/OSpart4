//
// Created by hadar on 6/17/22.
//


#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    char r = 'a';
    int clientFD = open("aa.txt", O_RDWR | O_APPEND | O_CREAT, 0777);
    int kk = read(clientFD, &r, 1);
    printf("%d", kk);



    int fdAnswer = open("aa.txt", O_RDWR | O_APPEND | O_CREAT, 0777);
    char d = 'a';
    long jj;
    jj =  read(fdAnswer, &d , 1);
    if(kk !=-1) {
        while (r != '\n'){
            kk =  read(fdAnswer, &r, 1);
            printf("%d read\n", kk);
            printf("%c", r);
        }
    }


    close(fdAnswer);
//    remove(fileAnswer);
//    exit(0);
}