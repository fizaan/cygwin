#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*

gcc -o out.bin q6.c -Wall
./out.bin

https://man7.org/linux/man-pages/man2/wait.2.html
"if WNOHANG was specified and one or more
child(ren) specified by pid exist, but have not yet changed
state, then 0 is returned.  On failure, -1 is returned."

 ./out.bin
0
-1


*/

int main() {

    int pid = fork();

    if(pid == 0) {

        int res = waitpid( (pid_t) -1, NULL, WNOHANG);

        printf("%d\n",res);
    
    } else {

        int res = waitpid( (pid_t) -1, NULL, WNOHANG);

        printf("%d\n",res);

    } 

}