# chroniclist

DONE:
- push_back() function
- push_front() function
- pop_front() function
- pop_back() function
- erase() function
- insert() function
- Compact operation
- Implement sort - empty nodes are filtered towards the end
- Added custom comparator support (with a lambda wrapper)
- LIST_END will always be a sentinel node at index 0
- Implement iterators (concealed index)

TO-DO:
- Implement timer
- Compare with std::forward_list/std::list

FIXES:
- Fixed difficult-to-find bugs regarding the iterator
- Added sentinel node for end() instead of weird behaviour