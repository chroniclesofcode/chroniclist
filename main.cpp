#include <iostream>
#include "chroniclist/fast_list.hpp"


#define NTESTS 15

int main(void) {
    chroniclist::fast_list<int> fl;
    for (int i = 0; i < NTESTS; i++) {
        fl.push_back(i);
    }
    fl.erase(3);
    fl.erase(9);
    fl.erase(10);
    fl.erase(0);
    fl.push_back(1001);
    fl.push_back(1002);
    fl.push_back(1003);
    fl.push_front(0);
    fl.push_back(1004);
    fl.push_back(1005);
    fl.push_back(1006);
    fl.push_front(-1);
    fl.push_front(-2);
    fl.pop_back();
    fl.push_back(1009);
    fl.pop_front();
    fl.push_front(-3);

    fl.insert(0, 69);
    fl.insert(9, 70);
    fl.insert(12, 71);
    fl.insert(-1, 72);
    fl.debug(1);
    fl.debug(0);
    fl.erase(13);
    fl.erase(12);
    //fl.compact();
    fl.erase(19);
    fl.erase(15);
    fl.sort(std::greater<int>());
    fl.debug(1);
    fl.debug(0);

    return 0;
}