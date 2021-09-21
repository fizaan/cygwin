#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*

gcc -o out.bin q3.c -Wall
./out.bin

- "Can you do this w/o calling wait in the parent?"

- Nope.

*/

int main() {

    int pid = fork();

    if(pid == 0) {

       printf("hello\n");
    
    } else {

      wait(NULL);

      printf("goodbye\n");

    } 

}