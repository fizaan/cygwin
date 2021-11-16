#ifndef FAY_HEADER
#define FAY_HEADER

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 'end' address is the LAST physical address 
// which the kernel code occupies in x86 physical 
// memory. It will change as the kernel size grows
#define end          0x10da00

#define BY2PG        0x1000
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

static u_int freemem;
static int fayPTCount;

// I should call this 'phymemory' 
// because it acts like a physical memory
static char * vm;   

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

static u_int alloc(u_int, int);
static void boot_map_segment(u_int *, u_int, int, u_int, u_int);
static void showKernelPDs(u_int * pgdir, int howManyPDsToPrint);
static u_int * boot_pgdir_walk(u_int *, u_int, int);
static u_int PADDR(u_int kva) { return kva - KERNBASE; }
static u_int KADDR(u_int pa) { return KERNBASE + pa; }
static u_int PDX(u_int va) { return (va >> 22) & 0x3ff; }
static u_int PTX(u_int va) { return (va >> 12) & 0x3ff; }
static u_int PT_ADDR(u_int pde) { return pde & 0xfff000; }

static u_int roundup(u_int n, int align) {
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

static u_int alloc(u_int n, int align) {
		
    u_int v;
    
    if (freemem == 0) freemem = end;
    
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
    
    return KERNBASE + v;
    
}

// Warning: throws seg fault when too few entries in vm
static void showKernelPDs(u_int * pgdir, int howManyPDsToPrint) {
    for(;;pgdir++)
		if(*pgdir != 0) {
			printf("PDE at VA %p: %08x (PgTbl address: %08x)\n", 
				(void *)pgdir, *pgdir, PT_ADDR(*pgdir));
			if(--howManyPDsToPrint == 0) break;
		}
}

static u_int * boot_pgdir_walk(u_int * pgdir, u_int va, int create) {
    
    pgdir += PDX(va);
    
    // pgdir is now the PDE! Well, technically 
    
    
    if(!(*pgdir & PTE_P)) {
        // Either PDE doesn't exist OR it's not valid
        if(create != 0) {
            // Save the current pde
            u_int * curPDE = pgdir;
            
            // ...then allocate a new Page Table (PT) in THAT NULL/INVALID spot
            u_int newPTAddr = alloc(BY2PG, BY2PG);

            // ...such that curPDE now points to this new PT.
            *curPDE = PADDR(newPTAddr)|PTE_P|PTE_W|PTE_U; // 4 bytes = 32 bits

            // Make sure the new PT is RELATIVE to my mimication
            // of x86 physical memory. Read the notes at the end of
            // alloc2.c
            pgdir = (u_int *) &vm[PADDR(newPTAddr)]; 
            
            // return the slot at the new page table + ptIndex
            return &pgdir[PTX(va)];
        }
        else return 0;
    }
    else {
        // get the page table address that can be found within this PDE
        int ptAddress = PT_ADDR(*pgdir);
        
        // return the PTE PA. Note, this is not the virtual address. Well
        // tenhnically it IS a VA in cygwin, but I'm interpretting it as a PA. 
        // 
        // So why am I not returning a KVA: 'return KADDR(ptAddress +  PTX(va))' ?
        // Ans: firstly, KADDR(ptAddress +  PTX(va)) is NOT a pointer which 
        // boot_map_segment is expecting. Secondly, boot_map_segment DOES NOT 
        // convert the VA returned by boot_pgdir_walk into PA using PADDR. 
        // *Alloc.java does that and x86 does that too implicitly when converting
        // logical to linear to physical addresses. See below (1a)
        //
        // *Desktop\lc3-backup\eclipse\Ostep\src\chapter\Alloc.java
        return &pgdir[ptAddress + PTX(va)];
    }
}

// (1a) boot_pgdir_walk should return a VA which gets converted to a PA by the 
// x86 but since this is running in cygwin (a virtual machine) I DO NOT have
// physical memory. My memory is mimiced by the vm array where all PDEs are 
// being placed and I am treating the returned pointer AS IS.
static void boot_map_segment(u_int * pgdir, u_int va, int size, u_int pa, u_int perm) {
    for (int i=0; i<size; i+=BY2PG)
        *boot_pgdir_walk(pgdir, va+i, 1) = (pa+i)|perm|PTE_P;
}

#endif