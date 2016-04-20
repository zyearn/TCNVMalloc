#include <sys/mman.h>
#include <stdio.h>

int main() {
    char *m = (char *)mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (m == (char *)-1) {
        printf("mmap error\n");
        return -1;
    } else {
        printf("mmap start at: %p", m);
    }

    int i = 0;
    for (; i<10; i++)
    m[0] = '0'; 
}
