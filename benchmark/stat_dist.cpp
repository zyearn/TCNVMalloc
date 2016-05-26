#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
using namespace std;

int main(int argc, char **argv) {
    void *p;
    map<size_t, int> mp;
    int total = 0;

    while (fscanf(stdin, "%p", &p) > 0) {
        mp[(size_t)p]++;
        total++;
    }

    /*
    for (map<size_t, int>::iterator it=mp.begin(); it!=mp.end(); it++) {
        printf("%08x %d\n", it->first, it->second);
    }
    */

    // printf("total=%d, avg=%lf\n", total, (double)total/mp.size());
    printf("%lf", (double)total/mp.size());

    return 0;
}
