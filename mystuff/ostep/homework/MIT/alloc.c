#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// cd /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/ostep/homework/MIT/
// gcc -o out.bin alloc.c -Wall -Wextra -pedantic

#define BY2PG 0x1000
#define KERNBASE 	 0xf0000000
#define PDMAP 		 4 * 1024 * 1024
#define VPT 		 KERNBASE - PDMAP
#define KSTACKTOP 	 VPT
#define ULIM 		 KSTACKTOP - PDMAP
#define UVPT 		 ULIM - PDMAP
#define KSTKSIZE 	 8 * BY2PG
#define UPAGES 		 UVPT - PDMAP
#define UENVS 		 UPAGES - PDMAP
#define bootstack   0xf0105000
#define npage       8192
#define sizeOfStructPage 12

// Permissions
#define PTE_P		0x001	// Present
#define PTE_W		0x002	// Writeable
#define PTE_U		0x004	// User
#define PTE_PWT		0x008	// Write-Through
#define PTE_PCD		0x010	// Cache-Disable
#define PTE_A		0x020	// Accessed
#define PTE_D		0x040	// Dirty
#define PTE_PS		0x080	// Page Size
#define PTE_MBZ		0x180	// Bits must be zero
#define PTE_AVAIL	0xe00	// Available for software use
#define PTE_USER	0xe07	// All flags that can be used in system calls

static uintptr_t freemem;
static char * vm;   // I should call this phymemory
                     // because it acts like a 
                     // physical memory
static u_int * vmptr;
static int fayPTCount;

static uintptr_t * alloc(int n, int align);
static u_int PDX(u_int va) { return (va >> 22) & 0x3ff; }
static u_int PTX(u_int va) { return (va >> 12) & 0x3ff; }
static u_int PT_ADDR(u_int pde) { return pde & 0xfff000; }

static uintptr_t roundup(uintptr_t n, int align) {
    if(n % align == 0) ; 
    // is already aligned so
    // no action required, else
    // align it:
    else n += (align - (n % align));
    
    return n;
}
	
static bool fayIsPowerOfTwo(int x) {
	return (x != 0) && ((x & (x - 1)) == 0);
}

static u_int * boot_pgdir_walk(u_int * pgdir, u_int va, int create) {
    
    pgdir += PDX(va);
    
    // pgdir is now the PDE! Well, technically 
    // pde is VM[pgdir] or *pde in c
    
    
    if(!(*pgdir & PTE_P)) {
        // Either PDE doesn't exist OR it's not valid
        if(create != 0) {
            // Save the current pde
            u_int * curPDE = pgdir;
            
            // ...then allocate a new Page Table (PT) in THAT NULL/INVALID spot
            pgdir = (u_int *) alloc(BY2PG, BY2PG);
            
            // such that curPDE now points to this new PT.
            *curPDE = (u_int)pgdir|PTE_P|PTE_W|PTE_U; // 4 bytes = 32 bits
            
            // return the slot at the new page table + ptIndex
            return pgdir + PTX(va);
        }
        else return 0;
    }
    else {
        // get the page table address that can be found within this PDE
        int ptAddress = PT_ADDR(*pgdir);
        
        // return the PTE VA
        return pgdir + ptAddress + PTX(va);
    }
}

static void boot_map_segment(u_int * pgdir, u_int va, int size, u_int pa, u_int perm) {
    for (int i=0; i<size; i+=BY2PG)
        *boot_pgdir_walk(pgdir, va+i, 1) = pa+i|perm|PTE_P;
}

static void showKernelPDs(u_int * pgdir, int howManyPDsToPrint) {
    for(;;)
		if(*pgdir++ != 0) {
			printf("PDE at VA %08x: %08x (PgTbl address: %08x)\n", 
				pgdir, *pgdir, PT_ADDR(*pgdir));
			if(--howManyPDsToPrint == 0) break;
		}
}

static uintptr_t * alloc(int n, int align) {
		
    uintptr_t * v;
    
    if (freemem == 0) freemem = vm;
    
    if(!fayIsPowerOfTwo(align))
        printf("Fay: Align %d is not a ^2", align);
    else n = roundup(n, align);
    
    // Step 1: roundup freemem up to be aligned properly
    if(freemem % BY2PG == 0) ; 
    // is already aligned properly
    // so no action required. Else
    // align it:
    else freemem = roundup(freemem, align);
    
    v = freemem;
    
    freemem += n;

    // Update page table count. I could simply 
    // do fayPTCount++, but this is more accurate:
    fayPTCount += n / BY2PG;
    
    return v;
    
}

int main() {

    // 10000000f * 1
    vm = (char *) calloc(-KERNBASE, sizeof(char));

    /*
        If vm is a char pointer:
        
        vm = (char *) calloc(-KERNBASE, sizeof(char));
        printf("%08x %08x %08x %d %d %d\n\n", vm, &vm[0], &vm, vm[0], vm[1], vm[2]);
        prints: efff0010 efff0010 00407008 0 0 0

        +----------+---------------+
        | 00407008 | vm = efff0010 |
        |  &vm     |               |    
        +----------+---------------+

        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^

        +----------+---------------+
        | efff0010 |    0          |  vm[0] = 0 = *vm 
        |  &vm[0]  |               |
        |  ..or    |               |
        |  vm      |               |
        +----------+---------------+
        | efff0011 |    0          |  vm[1] = 0 = *(vm+1)
        |  &vm[1]  |               |
        +----------+---------------+
        | efff0012 |    0          |  vm[2] = 0 = *(vm+2)
        +----------+---------------+
    */
    printf("%08x %08x %08x %d %d %d\n\n", vm, &vm[0], &vm, vm[0], vm[1], vm[2]);
	
    u_int * pgdir = (u_int *) alloc(BY2PG, BY2PG);
    printf("pgdir VA is %08x, freemem (vm) starts at VA %08x\n", pgdir, vm);

    pgdir[PDX(VPT)] = (u_int)pgdir|PTE_W|PTE_P;
    pgdir[PDX(UVPT)] = (u_int)pgdir|PTE_U|PTE_P;

    printf("First PDE (%04x) at VA %08x is %08x\n", 
        PDX(VPT), &pgdir[PDX(VPT)], pgdir[PDX(VPT)]);
    printf("Second PDE (%04x) at VA %08x is %08x\n", 
        PDX(UVPT), &pgdir[PDX(UVPT)], pgdir[PDX(UVPT)]);

    boot_map_segment(pgdir, KSTACKTOP-KSTKSIZE, KSTKSIZE,
            bootstack, PTE_W);
    
    boot_map_segment(pgdir, KERNBASE, -KERNBASE, 0, PTE_W);
    
    int n = npage * sizeOfStructPage;
    u_int * pages = (u_int*) alloc(n, BY2PG);
    boot_map_segment(pgdir, UPAGES, n, (u_int)pages, PTE_U);
    
    showKernelPDs(pgdir, 50);

    printf("%d pages allocated\n", fayPTCount);
    //printf("freemem is now at VA %08x, end is %08x\n", freemem, end);
    //u_int diff = freemem - end;
    //printf("freemem - end = %08x, %d bytes\n", diff, diff);*/

    free(vm);

}