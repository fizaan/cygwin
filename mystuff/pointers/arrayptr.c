#include <stdio.h>
#include <stdlib.h>

// gcc -o out.bin arrayptr.c -Wall

int main() {

    int a[] = { 1, 2, 3, 4, 5 };

    int * p;

    p = a;

    printf("p = %08x\n", p);

    // Wrong! I'm screwed. I thought this would store
    // the address of a[2] into p but instead
    // it stored the value at a[2]. As in the
    // C book: p[i] is the same as *(p + i)

    p = a[2];   // p is no longer storing an address! FUCKED

    p = &a[2];  // ok!

    printf("p = %08x\n", p);
}