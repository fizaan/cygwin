#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// gcc -o out.bin whatthef.c -Wall

//  https://www.youtube.com/watch?v=xVSPv-9x3gk

// "Fork makes two identical copies of the address spaces; 
// one for the child and one for the parent" - that is incorrect. 
// If that were true we would see THREE pids, not two.

int main() {
    // parent pid
    printf("Parent: %d\n", getpid());

    int newp = fork(); // fork returns ZERO to the child process
                       // and a POSITIVE value (the child PID) to the
                       // parent process
                       // www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/create.html

    if(newp < 0 ) {
        //error
    }
    else if(newp == 0) { // child
        printf("Child: %d\n", getpid());
    }
    else {  // parent
        printf("Parent (%d) of child %d\n", getpid(), newp);

        // Interactive shell (BASH) is the parent of this parent
        // cygwin>echo $$
        printf("Parent of THIS Parent is BASH (interactive shell) %d\n", getppid());
    }
}

/*
 Fay questions:
 - When fork is called, how does it know which address space to copy, i.e., 
   how does it know the start and end addresses? The kernel probaly knows
   which is the currently running process that called fork, and so it knows
   which address space is calling it, maybe. When fork takes over, the 
   calling process stops execution, and handles over control to kernel.

 - How does fork distinguish b/w the parent and child process when 
   returning a value to newp variable?

 - Google search: "memory management can be a bitch" fork
   
*/