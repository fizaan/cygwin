#include "include/fay.h"

// cd /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/ostep/homework/MIT/
// gcc -o out.bin alloc2.c -Wall -Wextra -pedantic

/*
Nov 18 10AM (osdev/sources/notes.txt)
		
What's the difference between each of these programs?

- C:\Users\Alifa\Desktop\lc3-backup\eclipse\Ostep\src\chapter\Alloc.java
- C:\Users\Alifa\Desktop\cygwin64-workspace\mystuff\ostep\homework\MIT\alloc.c 
- C:\Users\Alifa\Desktop\cygwin64-workspace\mystuff\ostep\homework\MIT\alloc2.c 

Firstly, they are all trying to demo the same concept - allocating page tables within another
page table (Page directory). The result of alloc2.c and Alloc.java are very similar but at 
first they are verify different (first few outputs). Secondly, ignore alloc.c because although
I feel it is working correcly, the freemen is NOT initialized to end as it should (0x10da00 - 
instead it is initialized to vm which is the pointer to the start of vm[0], which os why its 
results are so way off). So IGNORE alloc.c! And thirdly, carry on with alloc2.c (leave 
Alloc.java alone) to compare with the results in bochs. 
*/

int main() {
    // 10000000f * 1, which is -KERNBASE * 1
    // Can also do this:
    //      vm = (char *) calloc(0x0fffffff, sizeof(char));

    vm = (char *) calloc(-KERNBASE, sizeof(char));
    printf("vm loaded at %p\n", (void *)vm);
    u_int start = alloc(BY2PG, BY2PG);
    u_int * pgdir = (u_int *) &vm[PADDR(start)]; 
    printf("pgdir loaded at %p (&vm[%08x])\n", (void *)pgdir, PADDR(start));

    pgdir[PDX(VPT)] = PADDR(start)|PTE_W|PTE_P;
    pgdir[PDX(UVPT)] = PADDR(start)|PTE_U|PTE_P;

    printf("First PDE (%04x) at VA %p is %08x\n", 
        PDX(VPT), (void *)&pgdir[PDX(VPT)], pgdir[PDX(VPT)]);
    printf("Second PDE (%04x) at VA %p is %08x\n", 
        PDX(UVPT), (void *)&pgdir[PDX(UVPT)], pgdir[PDX(UVPT)]);

    boot_map_segment(pgdir, KSTACKTOP-KSTKSIZE, KSTKSIZE,bootstack, PTE_W);
    boot_map_segment(pgdir, KERNBASE, -KERNBASE, 0, PTE_W);

    int n = npage * sizeOfStructPage;
    u_int pages = alloc(n, BY2PG);
    boot_map_segment(pgdir, UPAGES, n, PADDR(pages), PTE_U);

    printf("\n");
    showKernelPDs(pgdir, 50);

    printf("%d pages allocated\n", fayPTCount);
    printf("freemem is now at VA %08x, end is %08x\n", KADDR(freemem), KADDR(end));
    u_int diff = freemem - end;
    printf("freemem - end = %08x, %d bytes\n", diff, diff);

    // 91 pages allocated
    // freemem is now at VA f0169000, end is f010da00
    // freemem - end = 0005b600, 374272 bytes
}

/*
    My vm which mimics the 10000000f (256m) physical space between
    the kernel end address and the 0x0fffffff mark.

    vm starts at 0x6fffefff0010. The u_int returned by alloc is 
    a PA to which the KERNBASE is added to produce a VA). Address of
    pgdir is therefore the start address of va + whatever was returned
    by alloc minus the KERNBASE.

        u_int start = alloc(BY2PG, BY2PG);
        
        &vm[PADDR(start)]

    That becomes out pgdir (page directory) pointer!

        u_int * pgdir = (u_int *) &vm[PADDR(start)];

    Then I added the first two entries into the pgdir:

        pgdir[PDX(VPT)] = PADDR(start)|PTE_W|PTE_P;
        pgdir[PDX(UVPT)] = PADDR(start)|PTE_U|PTE_P;

    Meaning,

        pgdir[PDX(VPT)]  = 0010e003 // a PDE
        pgdir[PDX(UVPT)] = 0010e005 // a PDE

        PTX(0010e003) = page table 0010e000

    Warning. You CANNOT DO THIS (1b). It's ok for MIT to do it 
    because their pgdir is actually a physical address. Mine is 
    a MIMICATION of the physical address (using an allocated 
    array (aka vm))

    (1b)
        pgdir[PDX(VPT)] = PADDR(pgdir)|PTE_W|PTE_P;
        pgdir[PDX(UVPT)] = PADDR(pgdir)|PTE_U|PTE_P;
*/

