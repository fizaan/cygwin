#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
    gcc -o out.bin q3.c -Wall
    ./out.bin

*/

int main() {

    // An array of 100 4-byte entries 
    int * p = malloc(sizeof(int) * 100);

    // fill 'em
    for(int i = 0; i < 100; i++) *(p+i) = i;

    // print 'em
    for(int i = 0; i < 100; i++) printf("%d ", *(p+i));

    free(p);

    // 1MB = 1024 x 1024 = 1,048,576 bytes
    // 1 int = 4 bytes
    // 1MB = 262,144 integers
    // 5MB = 1,310,720 integers

    int oneMB = 1024 * 1024;

    // allocation 5 MB

    p = malloc(5 * oneMB);

    // how many Integers is that? 
    // 1,310,720 since each Integer is 4-bytes
    for(int i = 0; i < 1310720; i++)  {
        int y = *(p+i);
        y++;
    }

    free(p);
}