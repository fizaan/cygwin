#include <stdio.h>
#include <stdlib.h>

// To create an object file from a source file, the compiler is invoked with the -c flag
// gnu.org/software/libtool/manual/html_node/Creating-object-files.html 
//
// Use the -c option to generate .obj file
// gcc -o ext.obj -c ext.c -Wall -Werror -Wextra

void exfunc() {
    printf("External func");
}