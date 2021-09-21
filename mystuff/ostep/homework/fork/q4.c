#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*

gcc -o out.bin q4.c -Wall
./out.bin

*/

int main() {

    int pid = fork();

    if(pid == 0) {

        execlp("/bin/ls", "-l", NULL);
    
    } else {

        printf("\ndone\n");

    } 

}