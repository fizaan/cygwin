#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#define wR  1 //write end of pipe - sender
#define r  0  //read end of pipe - receiver
#define psize  2 //pipe size

void errCheck(int ret);
void send(int pid, int p[]);
void receive(int pid, int p[]);

void errCheck(int ret) {
    if(ret<0) {
        printf("Error");
        exit(-1);
    }
}

void send(int pid, int p[]) {
    srand(pid);
    int * data = malloc(4); // allocate 4 * 4 bytes = 16 * 8 = 128bits 
    *data = rand() / 100;
    *(data + 1) = rand() / 100;
    *(data + 2) = rand() / 100;
    *(data + 3) = rand() / 100;
    printf("[%d] sent: ", pid);
    for(int i = 0; i < 4; i++) 
        printf("%d, ", *(data + i));
    printf("\n"); 
    write(p[wR], data, 16);
    free(data);
}

void receive(int pid, int p[]) {
    int * data = malloc(4);
    read(p[r], data, 16);
    printf("\t[%d] received: ", pid);
    for(int i = 0; i < 4; i++) 
        printf("%d, ", *(data + i));
    printf("\n"); 
    free(data);   
}

//Note: this won't work if you create multiple forks().
//I think it needs process synchronization.

int main() {
    int pipeA[psize], pipeB[psize], status;
    errCheck(pipe(pipeA));
    errCheck(pipe(pipeB));
    int pid = fork();
    errCheck(pid);
        
    if(pid == 0 ) { //child:
        /*
            child sends from pipeA
                - close unwanted read end of pipeA
                - send
                - close end write when sent
        */
        //printf("Child [%d] sending...\n", getpid());
        errCheck(close(pipeA[r]));
        send(getpid(), pipeA);
        errCheck(close(pipeA[wR]));

        /*
            child receives from pipeB
                - close unwanted write end of pipeB
                - read
                - close read end when done

        */
        //printf("\tChild [%d] receiving...\n", getpid());
        errCheck(close(pipeB[wR]));
        receive(getpid(), pipeB);
        errCheck(close(pipeB[r]));
        
        //must exit child process!
        exit(0);
    }

    //parent:

    /*
        parent sends from pipeB
        - close unwanted read end of pipeB
        - send
        - close write end when sent
    */
    errCheck(close(pipeB[r]));
    send(getpid(), pipeB);
    errCheck(close(pipeB[wR]));

    /*
        parent reads from pipeA
        - close unwanted write end of pipeA
        - send
        - close write end when sent
    */
    errCheck(close(pipeA[wR]));
    receive(getpid(), pipeA);
    errCheck(close(pipeA[r]));
    
    int temppid = wait(&status);
    errCheck(temppid);
    printf("Child terminated success: %d\n", temppid);
}


