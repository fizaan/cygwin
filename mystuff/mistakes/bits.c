#include <stdio.h>
#include <stdlib.h>

// gcc -o out.bin bits.c -Wall

int main() {
    unsigned int va = 0xfc2de338;
    unsigned int vpn = va & 0xfffff000;
    unsigned int pdIndex = (va >> 22) & 0x3ff;
    unsigned int ptIndex = (va >> 12) & 0x3ff;
    printf("vpn = %08x\n",vpn);
    printf("pdIndex = %08x\n",pdIndex);
    printf("ptIndex = %08x\n\n",ptIndex);
    
    vpn = va & 0xfffff000;  // Wrong! Do not do this because  bottom 0 bits are counted.
                            
    vpn = va >> 12;         // OK
    pdIndex = vpn >> 10;
    ptIndex = vpn & 0x3ff;
    printf("pdIndex = %08x\n",pdIndex);
    printf("ptIndex = %08x\n",ptIndex);
}