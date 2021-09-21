#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*

Q8) Write a program that creates two children, and connects the standard output 
of one to the standard input of the other, using the pipe() system call

gcc -o out.bin q8.c -Wall
./out.bin

*/

#define wR      1       // STDOUT - write end of pipe - sender - child A
#define r       0       // STDIN - read end of pipe - receiver - child B
#define psize   2       // pipe size

int main() {

    // only need 1 pipe for this example

    // child A sends (wR) while child B receives (R)

    int comm[psize];
    pipe(comm);

    int childa = fork();
    int childb = fork();

    if(childa == 0) {

        // first close unused read end of pipe

        close(comm[r]);

        // create message with new line and null terminator

        char buffer[18] = "Hi! I am child A\n\0";

        // send message to child B

        write(comm[wR],buffer,18);

        // close write end of pipe since message is sent

        close(comm[wR]);
    
    } else if (childb == 0) {
        
        // first close unsued write end of pipe

        close(comm[wR]);

        // read the message sent by child A to a char array

        char buffer[18];

        read(comm[r], buffer, 18);

        // print that message to stdout - console

        printf("%s", buffer);

        // close read end of this pipe since now we are done

        close(comm[r]);

    } else {

        // parent does nothing excepts waits for both children to complete

        for(int i = 0; i < 2; i++) {
        int pid = wait(NULL);
            if( pid == (pid_t) -1)
                printf("Wait failed\n");  
            else if (pid > (pid_t) 0)
                printf("\nChild terminated success: %d\n", pid);
        }

    }

}