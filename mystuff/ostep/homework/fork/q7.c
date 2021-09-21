#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*

gcc -o out.bin q7.c -Wall
./out.bin

*/

int main() {

    int pid = fork();

    if(pid == 0) {

       close(STDOUT_FILENO);

       printf("This won't get printed");
    
    } else {


    } 

}