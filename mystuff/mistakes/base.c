#include <stdio.h>
#include <stdlib.h>

// gcc -o out.bin base.c -Wall

int main() {
    unsigned int base = 0xf0000000;
    printf("Base is %09x\n", base);
    base = -base;
    printf("Base is %09x\n", base);
    base = -base;
    printf("Base is %09x\n", base);
    
    unsigned int newBase = 0xf0000000 - (2 * 0xf0000000);
    printf("New base is %09x\n", newBase);
    
    /*
        Base is 0f0000000
        Base is 010000000
        Base is 0f0000000
        New base is 010000000
    */

    /*
        Java version: (not saved in any java file or workspace)
        
        int base = 0xf0000000;
        System.out.printf("Base is %09x\n", base);
        base = -base;
        System.out.printf("Base is %09x\n", base);
        base = -base;
        System.out.printf("Base is %09x\n", base);
        
        base = 0xf0000000 - (2 * 0xf0000000);
        System.out.printf("New base is %09x\n", base);

        //output:
        Base is 0f0000000
        Base is 010000000
        Base is 0f0000000
        New base is 010000000
    */
}