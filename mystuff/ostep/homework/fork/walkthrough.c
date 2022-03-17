#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
  cc -o out.bin walkthrough.c -Wall -Werror -Wextra

  result:
   Parent
   hello
   hello
   hello
   hello
*/

int main() {
 printf("Parent\n");
 fork();
 fork();
 printf("hello\n");
}

/*
  Fay: Explain the result of this fork example 

  Fay: Answer
  
  +---------------------+
  | printf("Parent\n"); |  
  | fork();             | ----------->  +---------------------+
                                        | fork();             | -----------> | printf("hello\n");  |
                                        | printf("hello\n");  |
  +---------------------+
  | fork();             | -----------> | printf("hello\n");  |
  | printf("hello\n");  |
  +---------------------+

  As you can see there are 3 children processes and 'hello' gets printed 4 times. 'Parent' gets printed once only
  See 'Fork system call' YT video by Mehakk Kapur
*/