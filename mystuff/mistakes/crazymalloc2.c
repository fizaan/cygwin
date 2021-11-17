#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
    cd /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/mistakes/
    gcc -o out.bin crazymalloc2.c -Wall -Wextra -pedantic
    
    I asked this question on stackoverflow.com/questions/69977548
    How does one align a pointer that is allocated on the heap (using malloc)?

    ---
    0x6fffffef0010
    0x6fffffef1000
    1be9f82c
*/

#define BY2PG 0x1000

// 'vm' is a mimication of either 
// virtual or physical memmory
char * vm;


static uintptr_t roundup(uintptr_t n, int align) {
    if(n % align == 0) ; 
    // is already aligned so
    // no action required

    // else, align it:
    else n += (align - (n % align));
    
    return n;
}

int main() {

    vm = (char *) calloc(0x100000, sizeof(char));

    printf("%p\n", (void *) vm);

    u_int * intp = (u_int *) roundup((uintptr_t)vm, BY2PG);

    printf("%p\n", (void *) intp);

    intp[0] = 0x1be9f82c;

    printf("%08x\n", intp[0]);
}