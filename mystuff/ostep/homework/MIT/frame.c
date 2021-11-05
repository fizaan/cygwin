#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// cd /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/ostep/homework/MIT/

// gcc -o out.bin frame.c

struct page {
    u_int pa; // physical address
    u_int frameNumber;
    bool isFree;
} __attribute__((packed));

// The freemem
// struct page * freemem;
struct page freemem[655];

int main() {
   /*
		4GB divided into 4096 pages
		= (4 * 1024 * 1024) / 4
		= 1,048,476 page frames! But not all are available

		See C:\Users\Alifa\Desktop\osdev\sources\x86Map.txt

		600020h to 88F020h gives us 655 pages ~ 2.68 MB

        600020h - 88F020h = 28F000h

        28F000h / 1000h = 655
		
	*/

    //freemem = (struct page *) malloc(655 * sizeof(struct page));
		
    u_int pSize = 0x1000;
    u_int limit = 0xffffffff;
    u_int count = 0;
    u_int availableMem = 0;
    int availableFrames = 0;
    for(u_int i = 0; i < limit; i += pSize) {
        
        if(i >= 0x600020 && i <= 0x88F020) {
            
            if (i == 0x600020) continue;

            //printf("Frame %d: PA = %08x, VA = %x\n", count, i, 0xf0000000 + i);
            struct page newPage;
            newPage.isFree = 1;
            newPage.pa = i;
            newPage.frameNumber = count;
            freemem[availableFrames++] = newPage;

            availableMem += pSize;
        }

        if(i >= 0x88F020) break;

        count++;
    }

    for(int index = 0; index < availableFrames; index++) {
        struct page p = freemem[index];
        printf("Frame %d: PA = %08x, VA = %x isFree: %d\n", 
            p.frameNumber, p.pa, 0xf0000000 + p.pa, p.isFree);
    }

    // Should print 655!
    printf("Total allocatable page frames are %d\n", availableFrames);

    // Should print 28F000
    printf("Total ram allocated: %x\n", availableMem);

    printf("Size of struct page: %d bytes\n", sizeof(struct page));

    //free(freemem);
}

