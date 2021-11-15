#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// cd /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/mistakes/
// gcc -o out.bin crazymalloc2.c -Wall -Wextra -pedantic
// I asked this question on stackoverflow.com/questions/69977548

#define BY2PG 0x1000
char * vm;


static uintptr_t roundup(uintptr_t n, int align) {
    if(n % align == 0) ; 
    // is already aligned so
    // no action required, else
    // align it:
    else n += (align - (n % align));
    
    return n;
}

int main() {

    vm = (char *) calloc(0x100000, sizeof(char));

    printf("%08x\n", vm);

    vm = (char *) roundup((uintptr_t)vm, BY2PG);

    printf("%08x\n", vm);

    vm[0] = 5;

    printf("%d\n", vm[0]);
}