#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

/*

gcc -o out.bin measure.c -Wall
./out.bin

Keep running this program with the following param:

    NUMPAGES = 1
    NUMPAGES = 4
    NUMPAGES = 8
    NUMPAGES = 16
    NUMPAGES = 32
    NUMPAGES = 64
    NUMPAGES = 128
    NUMPAGES = 512
    NUMPAGES = 1024
    NUMPAGES = 2048
    NUMPAGES = 4096

Note: I did NOT use an outer loop as they suggested. It was taking too long and the nanoseconds
where too large.

*/

int main() {

    // https://blog.stalkr.net/2010/03/nanosecond-time-measurement-with.html
    struct timespec tpns;

    long long time;
    
    int NUMPAGES = 1;

    int PAGESIZE = 4096;

    // jump = 1024
    int jump = PAGESIZE / sizeof(int); 

    //long long spike = 0;

    //long long spikeDifference = 1000;

    printf("Number of Pages, Time per access (ns)\n");

    while(NUMPAGES <= 4096) {

        // give me an array which can hold NUMPAGES * PAGESIZE integers
        int * a = (int *) malloc(NUMPAGES * PAGESIZE); 

        clock_gettime(CLOCK_REALTIME, &tpns);

        long long start = tpns.tv_nsec;

        for (int i = 0; i < NUMPAGES * jump; i += jump) a[i] += 1;

        clock_gettime(CLOCK_REALTIME, &tpns);

        long long end = tpns.tv_nsec;

        time = end - start;

        // printf("%d page(s): %llu ns\n", NUMPAGES, time);

        printf("%d, %llu\n", NUMPAGES, time);

        NUMPAGES = NUMPAGES * 2;
        // NUMPAGES++;

        //if(time > spike && time - spike > spike * 2) {

        //    spike = time;

        //    printf(" <--- Spike");
        //}

        //printf("\n");

        free(a);
    }

}