#include <stdio.h>
#include <stdlib.h>

// gcc -o out.bin arrayptr.c -Wall

/*
    prints:

    p = ffffcc20
    a = &a = &a[0] = ffffcc20 ffffcc20 ffffcc20
    &p = ffffcc18

    p = ffffcc28


*/

int main() {

    int a[] = { 1, 2, 3, 4, 5 };

    int * p;

    p = a;

    printf("p = %08x\n", p);
    printf("a = &a = &a[0] = %08x %08x %08x\n", a, &a, &a[0]);
    printf("&p = %08x &p[0] = %08x &p[1] = %08x\n", &p, &p[0], &p[1]);
    printf("&p = %08x &p + 1 = %08x &p + 2 = %08x\n\n", &p, &p + 1, &p + 2);

    // Wrong! I'm screwed. I thought this would store
    // the address of a[2] into p but instead
    // it stored the value at a[2]. As in the
    // C book: p[i] is the same as *(p + i)

    p = a[2];   // p is no longer storing an address! FUCKED

    p = &a[2];  // ok!

    printf("p = %08x\n", p);
}