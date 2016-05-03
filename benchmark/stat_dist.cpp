#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
using namespace std;

int main(int argc, char **argv) {
    /*
    if (argc < 2) {
        printf("need args\n");
        return -1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("fopen err\n");
        return -1;
    }
    */
    
    void *p;
    map<size_t, int> mp;

    while (fscanf(stdin, "%p", &p) > 0) {
        mp[(size_t)p]++;
    }

    for (map<size_t, int>::iterator it=mp.begin(); it!=mp.end(); it++) {
        printf("%08x %d\n", it->first, it->second);
    }
    return 0;
}
