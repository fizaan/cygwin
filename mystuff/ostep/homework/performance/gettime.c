#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

/*

gcc -o out.bin gettime.c -Wall
./out.bin

Note: I didn't implement measuring performance of a context-switch for this 
excercise

$ ./out.bin
Tot Time taken: 10975.000000 ms
Avg Time taken: 0.109750 ms

*/

int main() {

    struct timeval current_time;

    int fd = open("gettime.c", O_WRONLY, 0666);

    char buffer[0];

    int iterations = 100000;

    double time;

    int loop = 0;

    while(loop++ < iterations) {

        gettimeofday(&current_time, NULL);

        long start = current_time.tv_usec;

        read(fd, buffer, 0);

        gettimeofday(&current_time, NULL);

        long end = current_time.tv_usec;

        time += end - start;

    }

    printf("Tot Time taken: %f ms\n", time);
    printf("Avg Time taken: %f ms", time / iterations);
}