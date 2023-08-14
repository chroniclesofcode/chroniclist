#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

namespace chroniclist {

template <class T>
class fast_list {
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

public:
    static const int LNULL = -1;
    int LIST_END = -1;

    fast_list() {
        _size = 0;
        list.push_back({ -100, LNULL, LNULL });
        LIST_END = 0;
        first = LIST_END;
        last = LIST_END;
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

    class iterator {
        friend fast_list;
        friend struct Node;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using reference = const T&;
        using pointer =  std::vector<Node> &;
        using difference_type = std::ptrdiff_t;

        iterator(pointer ptr, int start, int end) : ptr(ptr), idx(start), _end(end) {}

        auto operator*() const noexcept -> T& {
            return ptr[idx].value;
        }

        auto operator++() noexcept -> iterator& {
            idx = ptr[idx].next;
            return *this;
        }
        auto operator++(int) noexcept -> iterator {
            auto self = *this;
            ++*this;
            return self;
        }
        auto operator--() noexcept -> iterator& {
            idx = ptr[idx].prev;
            return *this;
        }
        auto operator--(int) noexcept -> iterator {
            auto self = *this;
            --*this;
            return self;
        }

        friend auto operator==(const iterator &lhs, const iterator &rhs) noexcept -> bool {
            return lhs.idx == rhs.idx;
        }
        friend auto operator!=(const iterator &lhs, const iterator &rhs) noexcept -> bool {
            return lhs.idx != rhs.idx;
        }

    private:
        pointer ptr;
        int idx;
        int _end;
    };

    iterator begin() {
        return iterator(list, first, LIST_END);
    }

    iterator end() {
        return iterator(list, LIST_END, LIST_END);
    }

    using reverse_iterator = std::reverse_iterator<iterator>;

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    iterator insert(iterator it, T elem) {
        return insert(it.idx, elem);
    }

    iterator insert(int idx, T elem) {
        if (idx == LNULL || list[idx].is_empty) { 
            std::cout << "Invalid iterator\n";
            exit(1);
        }
        int retit = LIST_END;
        if (empty_nodes.size() == 0) {
            int to_add = list.size();
            int prev = list[idx].prev;
            retit = to_add;
            list.push_back(node_t{ elem, prev, idx });
            if (first == idx) {
                first = to_add;
            } else {
                list[prev].next = to_add;
            }
            list[idx].prev = to_add;
        } else {
            int to_fill = empty_nodes.back();
            empty_nodes.pop_back();
            if (!list[to_fill].is_empty) {
                std::cout << "Error: trying to remove node that is not empty\n";
                exit(1);
            }
            retit = to_fill;
            int prev = list[idx].prev;
            if (first == idx) {
                first = to_fill;
            } else {
                list[prev].next = to_fill;
            }
            list[to_fill].prev = prev;
            list[to_fill].next = idx;
            list[idx].prev = to_fill;

            list[to_fill].value = elem;
            list[to_fill].is_empty = false;
        }
        _size++;
        return iterator(list, retit, LIST_END);
    }

    void push_back(T elem) {
        insert(LIST_END, elem);
    }

    void push_front(T elem) {
        insert(first, elem);
    }

    void pop_back() {
        if (_size == 0) {
            std::cout << "Nothing to pop_back()!\n";
            exit(1);
        }
        erase(list[last].prev);
    }

    void pop_front() {
        if (_size == 0) {
            std::cout << "Nothing to pop_front()!\n";
            exit(1);
        }
        erase(first);
    }

    void erase(int idx) {
        if (idx == LNULL || idx == LIST_END || list[idx].is_empty) {
            return;
        }
        int prev = list[idx].prev;
        int next = list[idx].next;
        if (prev != LNULL) {
            list[prev].next = next;
        }
        list[next].prev = prev;
        if (first == idx) {
            first = next;
        }

        empty_nodes.push_back(idx);
        list[idx].is_empty = true;
        _size--;
    }

    void compact() {
        if (_size == 0) {
            list.clear();
            empty_nodes.clear();
            list.push_back({ -100, LNULL, LNULL });
            LIST_END = 0;
            first = LIST_END;
            last = LIST_END;
            return;
        }
        std::vector<node_t> redo;
        int head = first;
        redo.push_back({ -100, LNULL, LNULL });
        LIST_END = 0;
        int prev = 0;
        while (head != LIST_END) {
            redo.push_back(node_t{ list[head].value, prev, prev + 2 });
            prev++;
            head = list[head].next;
        }
        redo.back().next = LIST_END;
        first = 1;
        redo[first].prev = LNULL;
        last = LIST_END;
        empty_nodes.clear();
        list = std::move(redo);
    }
    
    template<class F>
    void sort(F comparator = std::less<T>()) {
        if (_size == 0) {
            return;
        }
        // Sorts depending on custom comparator
        std::sort(list.begin(), list.end(), [comparator](const node_t a, const node_t b) {
            if (!a.is_empty && b.is_empty) return true;
            if (a.is_empty && !b.is_empty) return false;
            if (a.next == LNULL) return true; 
            if (b.next == LNULL) return false;
            return comparator(a.value, b.value);
        });
        LIST_END = 0;
        int prev = 0;
        int empties = LNULL;
        int pend = list.size() - 1;
        for (int i = 1; i < list.size(); i++) {
            // Sets empties var at the point where the empty nodes begin
            if (list[i].is_empty) {
                list[i-1].next = LIST_END;
                empties = i;
                pend = i - 1;
                break;
            }
            list[i].prev = prev;
            list[i].next = prev + 2;
            prev = i;
        }
        first = 1; // LIST_END is always 0
        list[first].prev = LNULL; // Aligns first node
        list[prev].next = LIST_END; // Sets last elem to LIST_END
        list[LIST_END].prev = pend; // Aligns end node
        if (empties != LNULL) {
            empty_nodes.clear();
            for (int i = list.size() - 1; i >= empties; i--) {
                empty_nodes.push_back(i);
            }
        }
    }

    int size() {
        return _size;
    }

    bool empty() {
        return _size == 0;
    }

private:
    
    std::vector<node_t> list;
    std::vector<T> empty_nodes;

    int first;
    int last;
    int _size;
};

}