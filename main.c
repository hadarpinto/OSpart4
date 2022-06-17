#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}
/**
 * server:
 * while(1) {
 *  pause()
 *  alarm with 60 seconds and stop
 *  when recieve client redo alarm to another 60 sec
 *  do fork
 *  }
 *
 *  while(wait(NULL) != -1 clear the zombies
 *
 *
 *
 *  client:
 *  use atoi
 *  do pause without while 1
 *  will try to open file, when receive -1 it means that someone writes and we will try it for 10 times
 *  using getrandom with modulo 5 + 1
 *
 */
