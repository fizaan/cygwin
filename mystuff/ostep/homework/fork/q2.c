#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

/*

gcc -o out.bin q2.c -Wall
./out.bin

Parent and child can both access fd and write to it concurrently

*/

int main() {

    system("rm temp.txt");
    
    sleep(1); //making sure out.txt is deleted.

    int fd = open("temp.txt", O_CREAT | O_WRONLY, 0666);

    int pid = fork();

    if(pid == 0) {

        char buffer[6] = "child\n";

        write(fd,buffer,6);
    
    } else {

        char buffer[7] = "parent\n";

        write(fd,buffer,7);

        wait(NULL);

    } 

    close(fd);

}