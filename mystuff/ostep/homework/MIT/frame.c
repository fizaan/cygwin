#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// cd /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/ostep/homework/MIT/

// gcc -o out.bin frame.c

struct page {
    u_int frameNumber;
    bool isFree;
} __attribute__((packed));

// The freemem: struct page * freemem;
struct page freemem[655];

// The allocated pages
int allocatedPages = 0;

// Generate and return a random number b/w 0 - 655
int rando(int shuffle) {
    time_t t;
    srand((unsigned) time(&t) * shuffle);
    return rand() % 655;
}

// Return a random page/frame
struct page * getRandomPage(int shuffle) {
    return freemem + rando(shuffle);
}

// Show memory (page frames)
// Physical address (pa) is calculated as:
// p.frameNumber * 0x1000 (4096)
void showPageFrames(int totalFrames) {
    for(int index = 0; index < totalFrames; index++) {
        struct page p = freemem[index];
        u_int pa = p.frameNumber * 0x1000;
        printf("Frame %d: PA = %08x, VA = %x isFree: %d\n", 
            p.frameNumber, pa, 0xf0000000 + pa, p.isFree);
    }
}

// Allocated nPages for a process named 
// 'process', and returns a ptr which 
// stores the address 0xf0000000
u_int * allocPages(char * process, int nPages) {

    if (allocatedPages > 655) {
        printf("Out of memory");
        exit(-1);
    }
    else if(allocatedPages + nPages > 655) {
        printf("Not enough memory available");
        exit(-1);
    }
    else if(nPages <= 0) return NULL;

    // Thinking question: where should this page table be stored?
    printf("Page Table for process %s:\n",process);

    // All processes that request memory
    // will get this starting address
    u_int va = 0xf0000000;

    // In a real OS kernel I don't have random
    // number generator - only used here for demo.
    // It is also extremely slow
    for(int i = 0; i < nPages; i++) {
        struct page * p = getRandomPage((i+7)*5);
        while(!p->isFree) p = getRandomPage((i+3)*17);
        p->isFree = 0;
        allocatedPages++;

        // Just a printout. PTE should actually consist of:
        // 1 valid bit, 20 bits for page frame (even though 
        // we are using only some of the frames). The va here
        // is not stored! I am only showing it here for your
        // understanding.
        printf("\tVPN: %d VA = %08x FRAME = %d\n", i, va, p->frameNumber);
        va += 0x1000;
    }

    // Print count of ALL pages that we allocated UPTO and AFTER this request
    // allocatedPages has global scope
    printf("\t----\n\tAllocated %d pages\n",allocatedPages);

    // Finally, always return the same start address
    return (u_int *) 0xf0000000;

}

int main() {
   /*
		See C:\Users\Alifa\Desktop\osdev\sources\x86Map.txt
        
        4GB divides into 4096 pages
		= (4 * 1024 * 1024) / 4
		= 1,048,476 page frames! But not all are available

        This also means each page tabler (for each process)
        will store 2^20 PTEs (each of which is maybe 4 bytes).
        That's HUGE but I will store only requested (nPages).

		See C:\Users\Alifa\Desktop\osdev\sources\x86Map.txt
		600020h to 88F020h gives us 655 pages ~ 2.68 MB
        600020h - 88F020h = 28F000h
        28F000h / 1000h = 655
		
	*/

    // Can use this option (1) but in a primitive kernel I don't have malloc!
    // Instead I used struct page freemem[655] which is stored on the stack.
    // (1) freemem = (struct page *) malloc(655 * sizeof(struct page));
    // And so, we also don't need (2) below
		
    u_int pSize = 0x1000;
    u_int limit = 0xffffffff;
    u_int pageFrame = 0;
    u_int availableMem = 0;
    int totalFrames = 0;

    // Start from 0 to limit because we need the pageFrame number
    for(u_int i = 0; i < limit; i += pSize) {
        
        // Skip 0x600020 (start from 0x600021)
        if(i >= 0x600021 && i <= 0x88F020) {
            
            struct page newPage;
            newPage.isFree = 1;
            newPage.frameNumber = pageFrame;
            freemem[totalFrames++] = newPage;

            availableMem += pSize;
        }

        if(i >= 0x88F020) break;

        pageFrame++;
    }

    // Should show that all frames are free
    // showPageFrames(totalFrames); 

    // Should print 655!
    printf("Total allocatable page frames are %d\n", totalFrames);

    // Should print 28F000
    printf("Total ram allocated: %x\n", availableMem);

    printf("Size of struct page: %d bytes\n", sizeof(struct page));

    // Processes requesting pages:
    allocPages("p0", 8);
    allocPages("p1", 3);
    allocPages("p2", 6);
    allocPages("p3", 12);
    allocPages("p4", 13);
    allocPages("p5", 5);
    allocPages("p6", 55);
    //allocPages("p7", 433);

    // Now should show some frames as free since some
    // frames were allocated by processes above
    // showPageFrames(totalFrames);

    // (2) Not required
    // free(freemem);
}

