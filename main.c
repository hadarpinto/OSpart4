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
 *
 */
