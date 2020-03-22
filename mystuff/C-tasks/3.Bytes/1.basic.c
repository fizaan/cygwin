#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h> 

int main() {
    /*
        lu is long unsigned returned by sizeof
        1 byte = 8 bits.
    */    
    printf("Size of char in bytes: %lu \n",sizeof(char));
    printf("Size of int in bytes: %lu \n",sizeof(int));

    /*
        what is the size of c below?
        c is holding 3 chars each of which
        is 1 byte. So 3 bytes.
    */
    char c[] = { 'A', 'B' , 'C' }; 
    printf("Size of c in bytes: %lu \n",sizeof(c));

    int d[] = { 1, 2, 3 }; // 4 * 3 = 12 bytes
    printf("Size of d in bytes: %lu \n",sizeof(d));
    
    /*
        malloc returns a void pointer 
        to any data type so it needs a cast
    */

    char * e = (char * ) malloc(5); //allocates 5 bytes.
    printf("Size of pointer e (on a 64bit machine) in bytes: %lu \n",sizeof(e));
    printf("Size of 1st value in e in bytes: %lu \n",sizeof( *e ) ); //1 since this is a char
    printf("Size of 2nd value in e in bytes: %lu \n",sizeof( *(e + 1) ) ); //1 since this is a char
    printf("Size of 3rd value in e in bytes: %lu \n",sizeof( *(e + 2) ) ); //1 since this is a char
    printf("Size of 4th value in e in bytes: %lu \n",sizeof( *(e + 3) ) ); //1 since this is a char
    printf("Size of 5th value in e in bytes: %lu \n",sizeof( *(e + 4) ) ); //1 since this is a char
    printf("Size of 6th value in e in bytes: %lu \n",sizeof( *(e + 5) ) ); //Ooops!

    int * f = (int * ) malloc(5); //allocates 5 * 4 = 20 bytes.
    printf("Size of pointer f (on a 64bit machine) in bytes: %lu \n",sizeof(f));
    printf("Size of 1st value in f in bytes: %lu \n",sizeof( *f ) ); //4 since this is a int
    printf("Size of 2nd value in f in bytes: %lu \n",sizeof( *(f + 1) ) ); //4 since this is a int
    printf("Size of 3rd value in f in bytes: %lu \n",sizeof( *(f + 2) ) ); //4 since this is a int
    printf("Size of 4th value in f in bytes: %lu \n",sizeof( *(f + 3) ) ); //4 since this is a int
    printf("Size of 5th value in f in bytes: %lu \n",sizeof( *(f + 4) ) ); //4 since this is a int
    printf("Size of 6th value in f in bytes: %lu \n",sizeof( *(f + 5) ) ); //ooops

   

    //example to write EXACTLY three bytes to a file:
    int myOutFileDescriptor = open("out.txt", O_CREAT | O_WRONLY, 0666);
    write(myOutFileDescriptor, c, sizeof(c));
    close(myOutFileDescriptor);

    //or...excatly 5 bytes
    strcpy(e, "faizI");
    myOutFileDescriptor = open("out2.txt", O_CREAT | O_WRONLY, 0666);
    write(myOutFileDescriptor, e, 5); //5 = read only 5 bytes from e
    close(myOutFileDescriptor);

    srand(time(0));
    int * g = (int * ) malloc(4); // allocate 4 * 4 bytes = 16 * 8 = 128bits 
    *g = rand() / 100;
    *(g + 1) = rand() / 100;
    *(g + 2) = rand() / 100;
    *(g + 3) = rand() / 100;

    //print g
    for(int i = 0; i < 4; i++) 
        printf("%d\n", *(g + i));

    //write g to a new file: out-int.txt:
    myOutFileDescriptor = open("out-int.bin", O_CREAT | O_WRONLY, 0666);
    write(myOutFileDescriptor, g, 16);
    close(myOutFileDescriptor);

    char * h = (char * ) malloc(4); // allocate four bytes 
    *h = 'h';
    *(h + 1) = 'i';
    *(h + 2) = 'j';
    *(h + 3) = 'k';

    for(int i = 0; i < 4; i++) 
        printf("%c", *(h + i));

    free(e);
    free(f);
    free(g);
    free(h);


}


/*

EBC+Alifa@ON34C02750570 /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/C-tasks/3.bytes
$ gcc -o 1.bin 1.basic.c -Wall

EBC+Alifa@ON34C02750570 /cygdrive/c/Users/Alifa/Desktop/cygwin64-workspace/mystuff/C-tasks/3.bytes
$ ./1.bin
Size of char in bytes: 1
Size of int in bytes: 4
Size of c in bytes: 3
Size of d in bytes: 12
Size of pointer e (on a 64bit machine) in bytes: 8
Size of 1st value in e in bytes: 1
Size of 2nd value in e in bytes: 1
Size of 3rd value in e in bytes: 1
Size of 4th value in e in bytes: 1
Size of 5th value in e in bytes: 1
Size of 6th value in e in bytes: 1
Size of pointer f (on a 64bit machine) in bytes: 8
Size of 1st value in f in bytes: 4
Size of 2nd value in f in bytes: 4
Size of 3rd value in f in bytes: 4
Size of 4th value in f in bytes: 4
Size of 5th value in f in bytes: 4
Size of 6th value in f in bytes: 4
2407660
8471288
19230543
10129226
hijk



*/