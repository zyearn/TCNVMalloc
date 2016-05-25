#include "include/nvmalloc.h"
#include <stdio.h>

int main() {
    nvmalloc_init(100, 10);
    void *ptr = nvmalloc(1);
    printf("%p\n", ptr);
}
