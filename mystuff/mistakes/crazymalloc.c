#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// cd /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/mistakes/

// gcc -o out.bin crazymalloc.c

/*
    Pointer 'a' is holding address 0xffffcc1c
    Pointer 'p' is also holding address 0xffffcc1c
    10

    vm stores address 00000430 and value at index 0 is 05
    vm2 stores address 00000430 and value at index 0 is 05
    vm2 now stores address 00000118 <--- WOW!
    Segmentation fault (core dumped)

*/

int main() {
    // stackoverflow.com/questions/66932822
    int val = 10;
    int * a = &val;
    int * p =(int *)0xffffcc1c; // ok!
  
    printf("Pointer 'a' is holding address %p\n",a);
    printf("Pointer 'p' is also holding address %p\n",p);
    printf("%d\n\n",*p);

    // Fay code:
    char * vm = (char *) calloc(100, sizeof(char));

    vm[0] = 5;

    printf("vm stores address %08x and value at index 0 is %02x\n", vm, vm[0]);

    char * vm2 = vm;

    printf("vm2 stores address %08x and value at index 0 is %02x\n", vm2, vm2[0]);

    // I can't do this. It changed my address! Why?
    // I think it's because vm is allocated on the 
    // heap and the compiler knows this so it won't
    // let you directly assign the pointer. Where as
    // pointers 'a' and 'p' are on the stack so it
    // doesn't care? 
    //
    // On a kernel it won't manipulate the address.
    vm2 = (char *) 00000430; 

    printf("vm2 now stores address %08x\n", vm2);
    printf(" and value at index 0 is %02x\n", vm2[0]);
}