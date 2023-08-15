#include <iostream>
#include <list>
#include "chroniclist/fast_list.hpp"
#include "chroniclist/Timer.h"

#define TIMER 1
#define CHRONICLIST 0
#define NTESTS 50

int main(void) {

    if (TIMER) {
        const int COUNT = 5 * 1e7;

        if (CHRONICLIST) {
            Timer mine("../stats/fast_list_insert.md");
            Timer mine_erase("../stats/fast_list_erase.md");
            Timer mine_randinsert("../stats/fast_list_randinsert.md");
            Timer mine_traverse("../stats/fast_list_traverse.md");

            for (int num = 0; num < NTESTS; num++) {
                chroniclist::fast_list<int> fl;

                // Insert
                mine.start();
                for (int i = 0; i < COUNT; i++) {
                    fl.push_back(i);
                }
                mine.stop();
                
                // Erase every 5th iterator
                int ct = 0;
                mine_erase.start();
                for (auto it = fl.begin(); it != fl.end(); it++) {
                    if (ct % 5 == 0) {
                        it = fl.erase(it);
                        if (it == fl.end()) break;
                    }
                    ct++;
                }
                mine_erase.stop();

                // Randomly insert every 4th iterator
                ct = 0;
                mine_randinsert.start();
                for (auto it = fl.begin(); it != fl.end(); it++) {
                    if (ct % 4 == 0) {
                        fl.insert(it, ct);
                    }
                }
                mine_randinsert.stop();

                // Traverse the entire list again
                long long ans = 0;
                mine_traverse.start();
                for (auto it = fl.begin(); it != fl.end(); it++) {
                    ans += *it / 50;
                }
                mine_traverse.stop();
            }

            mine.printStats();
            mine_erase.printStats();
            mine_randinsert.printStats();
            mine_traverse.printStats();

        } else {
            Timer standard("../stats/std_list_insert.md");
            Timer standard_erase("../stats/std_list_erase.md");
            Timer standard_randinsert("../stats/std_list_randinsert.md");
            Timer standard_traverse("../stats/std_list_traverse.md");

            for (int num = 0; num < NTESTS; num++) {
                std::list<int> stdl;

                // Insert elements
                standard.start();
                for (int i = 0; i < COUNT; i++) {
                    stdl.push_back(i);
                }
                standard.stop();

                // Erase every 5th iterator
                int ct = 0;
                standard_erase.start();
                for (auto it = stdl.begin(); it != stdl.end(); it++) {
                    if (ct % 5 == 0) {
                        it = stdl.erase(it);
                        if (it == stdl.end()) break;
                    }
                    ct++;
                }
                standard_erase.stop();

                // Randomly insert every 4th iterator
                ct = 0;
                standard_randinsert.start();
                for (auto it = stdl.begin(); it != stdl.end(); it++) {
                    if (ct % 4 == 0) {
                        stdl.insert(it, ct);
                    }
                }
                standard_randinsert.stop();

                // Traverse the entire list again
                long long ans = 0;
                standard_traverse.start();
                for (auto it = stdl.begin(); it != stdl.end(); it++) {
                    ans += *it / 50;
                }
                standard_traverse.stop();
            }
            standard.printStats();
            standard_erase.printStats();
            standard_randinsert.printStats();
            standard_traverse.printStats();
        }

    } else {
        chroniclist::fast_list<int> fl;
        fl.push_front(69);
        fl.erase(fl.begin());
        fl.push_back(69000);
        fl.push_front(69000000);
        for (int i = 0; i < 15; i++) {
            fl.push_back(i);
        }
        int ct = -69;
        int ct2 = 900;
        for (auto it = fl.begin(); it != fl.end(); it++) {
            auto it2 = fl.insert(it, ct);
            ct += 10;
            fl.insert(it2, ct2);
            fl.erase(it);
            it = it2;
            ct2 += 10;
        }
        fl.push_front(6900000);
        fl.debug(1);
        fl.debug(0);

        fl.sort(std::less<int>());
        auto beg = fl.begin();
        beg++;
        for (auto it = beg; it != fl.end(); it++) {
            it = fl.erase(it);
            if (it == fl.end()) break;
        }
        fl.push_front(-999);
        fl.push_back(999);
        fl.debug(1);
        fl.debug(0);
    }
    
    return 0;
}