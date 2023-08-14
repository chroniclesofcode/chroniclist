#include <iostream>
#include "chroniclist/fast_list.hpp"


#define NTESTS 15

int main(void) {
    chroniclist::fast_list<int> fl;
    for (int i = 0; i < NTESTS; i++) {
        fl.push_back(i);
    }
    int ct = -69;
    int ct2 = 900;
    for (auto it = fl.begin(); it != fl.end(); it++) {
        auto it2 = fl.insert(it, ct);
        ct += 10;
        (void)it2;
        fl.insert(it2, ct2);
        ct2 += 10;
    }
    fl.debug(1);
    fl.debug(0);

    fl.sort(std::less<int>());
    fl.debug(1);
    fl.debug(0);
    
    return 0;
}