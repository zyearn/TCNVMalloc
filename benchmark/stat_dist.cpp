#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
using namespace std;

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("need args\n");
        return -1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("fopen err\n");
        return -1;
    }
    
    vector<size_t> v;
    void *p;
    while (fscanf(fp, "%p", &p) > 0) {
        v.push_back((size_t)p);
    }

    sort(v.begin(), v.end());

    map<size_t, int> mp;
    for (size_t i=0; i<v.size(); i++) {
        mp[v[i]]++;
    }

    for (map<size_t, int>::iterator it=mp.begin(); it!=mp.end(); it++) {
        printf("%zu %d\n", it->first, it->second);
    }
    return 0;
}
