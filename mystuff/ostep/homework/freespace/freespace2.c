#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// cd /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/ostep/homework/freespace/

// gcc -o out.bin freespace2.c

// In this example each 'chunk' takes up 21 bytes of the freespace 

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

    // Taken from C:\Users\Alifa\Desktop\osdev\batch\kernel\headers\heap\heaplite.h

    if(size + sizeof(struct chunk) > freespace->size) {
        printf("Out of memmory\n");
        return NULL;
    }

    struct chunk * cur = freespace;
    
    while(true) {
       if(cur->nxt == NULL) {
           struct chunk * newseg;
           if(cur == freespace) // This should happen
                                // only on first call to
                                // kmalloc
                newseg = cur + 1;
           else {
                // Don't do this - it won't work here. Not sure why - look into it
                // newseg = (struct chunk *) ((uint8_t)cur + cur->size);

                uint8_t * newaddr = (uint8_t *) cur;
                newaddr += cur->size;
                newseg = (struct chunk *) newaddr;
           }
           
           newseg->size = size + sizeof(struct chunk);
           newseg->id = id;
           newseg->nxt = NULL;
           newseg->isFree = false;
           cur->nxt = newseg;
           freespace->size -= newseg->size;
           return newseg;
       }
       else if(cur->nxt->isFree) {  // There is a bug here! I am not checking if this chunk's 
                                    // size is >= requested size + size of header. When p2 is 
                                    // freed we have 24 bytes available in that chunk (p2 = 3 
                                    // + 21 = 24 bytes) and when p4 requests 17 bytes (38) we
                                    // land here so p4 occupies the address of p2 and gets 24
                                    // bytes instead of 38!
            freespace->size -= cur->nxt->size;
            cur->nxt->isFree = false;
            cur->nxt->id = id;
            return cur->nxt;
       }

       cur = cur->nxt;
    }
    
} // end kmalloc

int kfree(struct chunk * c) { 
    c->isFree = true; 
    freespace->size += c->size;
} 


int main() {

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