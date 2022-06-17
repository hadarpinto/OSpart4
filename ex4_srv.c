//
// Created by Hadar Pinto 316460146
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>


void signalHandler (int signal);

int main(int argc, char **argv) {
    signal(SIGUSR1, signalHandler);
    while(1) {
        pause();

    }
}


void signalHandler (int signal){

}
