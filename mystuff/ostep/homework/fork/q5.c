#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/*

gcc -o out.bin q5.c -Wall
./out.bin

If we put wait in the child, wait will return -1 aka failure.

./out.bin
-1
2254

*/

int main() {

    int pid = fork();

    if(pid == 0) {

        int res = wait(NULL);

        printf("%d\n",res);
    
    } else {

        int res = wait(NULL);

        printf("%d\n",res);

    } 

}