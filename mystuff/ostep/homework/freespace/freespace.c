#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// cd /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/ostep/homework/freespace/

// gcc -o out.bin freespace.c

// In this example each chunk 'chunk' takes up 21 bytes of the freespace 

// This example is (I think..) first fit without the 'splitting' and 'collapsing'

struct chunk {
    bool isFree;
    char * id;
    int size;
    struct chunk * nxt;
} __attribute__((packed));

// The freespace
struct chunk * freespace;

void init(int size) {
    // Get 1 * 1000 bytes worth of "freespace". Don't name it free - that is a keyword

    // The book adds chunk header from the freespace size (but I think it makes more sense to subtract)

    freespace = (struct chunk *) malloc(sizeof(uint8_t) * (size + sizeof(struct chunk)));

    //freespace = (struct chunk *) mmap(NULL, size + sizeof(struct chunk), 
    //    PROT_READ|PROT_WRITE,MAP_ANON|MAP_PRIVATE, -1, 0);

    freespace->size = size + sizeof(struct chunk);

    // Setup
    freespace->isFree = true;

    freespace->id = "head";

    freespace->nxt = NULL;
}

void kprint() {

    struct chunk * temp = freespace;

    while(temp != NULL) {

        printf("\n+--------+ <--- %d id: %s\n %d %s\n+--------+\n\n", 
            temp, 
            temp->id, 
            temp->size, // size INCLUDES size of header (aka chunk)
            (temp->isFree ? "free" : temp->id));

        temp = temp->nxt;
    }
}

struct chunk * kmalloc(int size, char * id) {

    if(size > freespace->size) printf("Out of memory");

    struct chunk * temp = freespace;

    struct chunk * newc;

    while(temp->nxt != NULL) {

        if(temp->nxt->isFree && temp->nxt->size == size + sizeof(struct chunk)) { 

            // Here? Exact match was found

            temp->nxt->isFree = false;

            temp->nxt->id = id;

            return temp->nxt; 
        }
        
        else if(temp->nxt->isFree && temp->nxt->size >= size + sizeof(struct chunk)) {

            // Here? We have found a free chunk (that was previously occupied and then freed)

            // and has enough space to fulfill the newly requested chunck

            temp->nxt->isFree = false;

            temp->nxt->id = id;

            return temp->nxt; 

        }  
        
        temp = temp->nxt;

    } // end while

    if(temp == freespace) {

        // This 'if' case should happen once only. The very first request just came in

        newc = temp + 1; // pointer addition. Not "temp + sizeof(struct chunk)" !

        newc->size = size + sizeof(struct chunk);

        newc->id = id;

        newc->isFree = false;

        newc->nxt = NULL;

        temp->size -= newc->size;

        temp->nxt = newc;

    } else {

        // Here? We didn't find a free block so we create a new chunk and add it to the prev chunk's tail - 

        // its like going all the way to the end of the list.

        // Don't do this "newc = temp + temp->size" it's not correct pointer arithmetic 

        // Workaround for correct ptr arithmetic
        uint8_t * newaddr = (uint8_t *) temp;

        newaddr += temp->size;

        newc = (struct chunk *) newaddr;

        newc->size = size + sizeof(struct chunk);

        newc->id = id;

        newc->isFree = false;

        newc->nxt = NULL;

        freespace->size -= newc->size;

        temp->nxt = newc;
    }

    
    return newc;

} // end kmalloc

int kfree(struct chunk * c) { c->isFree = true; } 


int main() {

    // struct chunk * free = (struct chunk *) malloc(sizeof(struct chunk));

    // dos>cd C:\Users\Alifa\Desktop\ostep.org\ostep-homework\vm-freespace

    // dos>python malloc.py -S 1000 -H 21 -p FIRST -s 0 -A +15,+6,+3+29,-2,+17,+3,-3,+25,+81

    // or...

    // dos>python malloc.py -S 1000 -H 21 -p FIRST -s 0 -A +15,+6,+3,+29,-2,+17,+3,-3,+25,+81 -c

    init(1000 - sizeof(struct chunk));
    kprint();

    struct chunk * p0 = kmalloc(15, "p0");
    printf("%s requested %d (%d) bytes, returned: %d", p0->id, 15, 15+sizeof(struct chunk), p0);
    kprint();

    struct chunk * p1 = kmalloc(6, "p1");
    printf("%s requested %d (%d) bytes, returned: %d", p1->id, 6, 6+sizeof(struct chunk), p1);
    kprint();

    struct chunk * p2 = kmalloc(3, "p2");
    printf("%s requested %d (%d) bytes, returned: %d", p2->id, 3, 3+sizeof(struct chunk),p2);
    kprint();

    struct chunk * p3 = kmalloc(29, "p3");
    printf("%s requested %d (%d) bytes, returned: %d", p3->id, 29, 29+sizeof(struct chunk), p3);
    kprint();

    kfree(p2);
    printf("free p2");
    kprint();

    struct chunk * p4 = kmalloc(17, "p4");
    printf("%s requested %d (%d) bytes, returned: %d", p4->id, 17, 17+sizeof(struct chunk), p4);
    kprint();

    struct chunk * p5 = kmalloc(3, "p5");
    printf("%s requested %d (%d) bytes, returned: %d", p5->id, 3, 3+sizeof(struct chunk), p5);
    kprint();

    kfree(p3);
    printf("free p3");
    kprint();

    struct chunk * p6 = kmalloc(25, "p6");
    printf("%s requested %d (%d) bytes, returned: %d", p6->id, 25, 25+sizeof(struct chunk), p6);
    kprint();

    struct chunk * p7 = kmalloc(81, "p7");
    printf("%s requested %d (%d) bytes, returned: %d", p7->id, 81, 81+sizeof(struct chunk), p7);
    kprint();

    free(freespace);

}