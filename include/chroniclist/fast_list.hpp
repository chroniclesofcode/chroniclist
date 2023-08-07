#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

namespace chroniclist {

template <class T>
class fast_list {
public:
    fast_list() {
        first = -1;
        last = -1;
        _size = 0;
    }

    ~fast_list() = default;

    void debug(bool ordered) {
        std::cout << "\n**** DEBUG START ****\n\n";
        if (ordered) {
            int head = first;
            while (head != LIST_END) {
                std::cout << "list[" << head << "]: " << list[head].value << " prev: " << list[head].prev << " next: " << list[head].next  << '\n';
                head = list[head].next;
            }
        } else {
            for (int i = 0; i < list.size(); i++) {
                std::cout << "list[" << i << "]: " << list[i].value << " prev: " << list[i].prev << " next: " << list[i].next << '\n';
            }
        }
        std::cout << "\n\n**** DEBUG END ****\n";
    }

    void insert(int idx, T elem) {
        if (idx == LIST_END) {
            push_back(elem);
            return;
        } else if (idx == first) {
            push_front(elem);
            return;
        } else if (list[idx].is_empty) { 
            std::cout << "Invalid iterator\n";
            exit(1);
        }
        if (empty_nodes.size() == 0) {
            if (list.size() == 0) {
                std::cout << "Cannot insert behind element for a list of size 0\n";
                exit(1);
            } else {
                int to_add = list.size();
                int prev = list[idx].prev;
                list.push_back(node_t{ elem, prev, idx });
                list[prev].next = to_add;
                list[idx].prev = to_add;
            }
        } else {
            int to_fill = empty_nodes.back();
            empty_nodes.pop_back();
            if (!list[to_fill].is_empty) {
                std::cout << "Error: trying to remove node that is not empty\n";
                exit(1);
            }
            int prev = list[idx].prev;
            list[prev].next = to_fill;
            list[to_fill].prev = prev;
            list[to_fill].next = idx;
            list[idx].prev = to_fill;

            list[to_fill].value = elem;
            list[to_fill].is_empty = false;
        }
        _size++;
    }

    void push_back(T elem) {
        if (empty_nodes.size() == 0) {
            if (list.size() == 0) {
                list.push_back(node_t{ elem, LIST_END, LIST_END });
                first = 0;
                last = 0;
            } else {
                list[last].next = list.size();
                int tmp = last;
                last = list.size();
                list.push_back(node_t{ elem, tmp, LIST_END });
            }
        } else {
            int idx = empty_nodes.back();
            empty_nodes.pop_back();
            if (!list[idx].is_empty) {
                std::cout << "Error: trying to remove node that is not empty\n";
                exit(1);
            }
            list[last].next = idx;
            int tmp = last;
            last = idx;
            list[last].value = elem;
            list[last].prev = tmp;
            list[last].next = LIST_END;
            list[last].is_empty = false;
        }
        _size++;
    }

    void push_front(T elem) {
        if (empty_nodes.size() == 0) {
            if (list.size() == 0) {
                list.push_back(node_t{ elem, LIST_END, LIST_END });
                first = 0;
                last = 0;
            } else {
                list[first].prev = list.size();
                int tmp = first;
                first = list.size();
                list.push_back(node_t{ elem, LIST_END, tmp });
            }
        } else {
            int idx = empty_nodes.back();
            empty_nodes.pop_back();
            if (!list[idx].is_empty) {
                std::cout << "Error: trying to remove node that is not empty\n";
                exit(1);
            }
            list[first].prev = idx;
            int tmp = first;
            first = idx;
            list[first].value = elem;
            list[first].prev = LIST_END;
            list[first].next = tmp;
            list[first].is_empty = false;
        }
        _size++;
    }

    void pop_back() {
        if (_size == 0) {
            std::cout << "Nothing to pop_back()!\n";
            exit(1);
        }
        erase(last);
    }

    void pop_front() {
        if (_size == 0) {
            std::cout << "Nothing to pop_front()!\n";
            exit(1);
        }
        erase(first);
    }

    void erase(int idx) {
        if (idx == LIST_END || list[idx].is_empty) {
            return;
        }
        int prev = list[idx].prev;
        int next = list[idx].next;
        if (prev != LIST_END) {
            list[prev].next = next;
        }
        if (next != LIST_END) {
            list[next].prev = prev;
        }
        if (first == idx) {
            first = next;
        }
        if (last == idx) {
            last = prev;
        }
        empty_nodes.push_back(idx);
        list[idx].is_empty = true;
        _size--;
    }

    void compact() {
        if (_size == 0) {
            list.clear();
            empty_nodes.clear();
            return;
        }
        std::vector<node_t> redo;
        int head = first;
        int prev = LIST_END;
        while (head != LIST_END) {
            redo.push_back(node_t{ list[head].value, prev, prev + 2 });
            prev++;
            head = list[head].next;
        }
        redo.back().next = LIST_END;
        first = 0;
        last = redo.size() - 1;
        empty_nodes.clear();
        list = std::move(redo);
    }
    
    template<class F>
    void sort(F comparator = std::less<T>()) {
        if (_size == 0) {
            return;
        }
        std::sort(list.begin(), list.end(), [comparator](const node_t a, const node_t b) {
            if (!a.is_empty && b.is_empty) return true;
            if (a.is_empty && !b.is_empty) return false;
            return comparator(a.value, b.value);
        });

        int prev = LIST_END;
        int empties = LIST_END;
        for (int i = 0; i < list.size(); i++) {
            if (list[i].is_empty) {
                list[i-1].next = LIST_END;
                last = i-1;
                empties = i;
                break;
            }
            list[i].prev = prev;
            list[i].next = prev + 2;
            prev = i;
        }
        first = 0;

        if (empties != LIST_END) {
            empty_nodes.clear();
            for (int i = list.size() - 1; i >= empties; i--) {
                empty_nodes.push_back(i);
            }
        } else {
            last = list.size() - 1;
        }
    }

    int size() {
        return _size;
    }

    bool empty() {
        return _size == 0;
    }

private:
    typedef struct Node {
        T value;
        int prev;
        int next;
        bool is_empty;

        /*
        bool operator<(const Node o) const {
            return value < o.value;
        }
        */

        Node(T val, int prv, int nxt) : value{ val }, prev{ prv }, next{ nxt } { is_empty = false; }
    } node_t;

    
    std::vector<node_t> list;
    std::vector<T> empty_nodes;

    const int LIST_END = -1;

    int first;
    int last;
    int _size;
};

}