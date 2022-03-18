// gcc -o main.obj -c main.c -Wall -Werror -Wextra
// gcc -o out.bin ext.obj main.obj

extern void exfunc();

int main() {
    exfunc();
}