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
- Implement timer
- Compare with std::forward_list/std::list
- Updated README with findings

TO-DO:
- Measure % cache misses (perhaps likwid-perfctr?)

FIXES:
- Fixed difficult-to-find bugs regarding the iterator
- Added sentinel node for end() instead of weird behaviour
- Copy constructor deleted since reference was used, changed to a pointer

# Findings

Our fast list (chroniclist) is a linked-list designed within a vector, as an
experiment to see if we can improve the runtime of a traditional linked list. It
is a template class, so we can use any data type we want - offers basic
functionality including sorting (given that a suitable comparator is provided...).

In /stats/fast_list* and /stats/std_list* we can see the comparisons between our
fast list, and the STL implementation of a linked list - which I believe is the
'standard' way of implementing a linked list. My linked list probably has bugs
in it, and does not fully cover all the features that the STL list has, however,
this linked list still has decent enough functionality to be used for a project
(assuming there are no bugs). This list was written to demonstrate the performance
difference between a linked list with optimal memory layout vs a linked list
scattered all about memory, with expensive pointer dereferences.

So the results show that my list is generally much faster in pretty much all
tests that I have done. This includes pushing back 7 million ish integers,
erasing in the middle of the list, inserting in the middle of the list, and
general traversal. 

The implementation of my list is a vector of struct nodes, each node containing
the template value. Since this is designed in a vector, we cannot use pointers
(nor would I want to) to the next value, since these memory address will change
when the vector gets reallocated. Thus, in the iterator, I use indexes to point
to prev, next and to the end. This also gives a pretty handy feature in that
our iterators do not get invalidated upon insertion and erasure. However, it will
get invalidated when sort gets run or our compact functionality is run. 

I'm going to be very brief about how the list works: we have a vector of nodes,
every time we insert a node, we push it back to the end of the vector. If a node
is erased, we don't actually erase it but we mark it as deleted, and add it
to an empty_nodes vector. If we are inserting anything from then on, we will
take from this empty_nodes vector. So if a lot of erasures/insertions happen,
each node will be all over the place and indexes will be very far apart. That is
why a compact() operation is provided, which will align the vector memory structure
in the same format as the linked list traversal. So basically, instead of jumping
around our memory, we are accessing indexes within our vector. We also have the
end() iterator always stored at index 0. Not sure if this is the best practice, but
there will need to be extra comparisons to be done during the sort() function to
make sure the end() iterator isn't going anywhere. 

It appears that our theory that a vector-based linked list is faster is correct. 
Reasons may include: much less cache misses due to data prefetching. I believe that
when the CPU fetches memory, it will fetch it in blocks - therefore if the memory
is contiguous, there is a much higher chance that the element you are fetching
will be a cache hit and will be loaded very fast. In a linked list, the data is
all over the place, which can result in many cache misses. There's also the issue of
instruction level parallelism, but I don't think my linked-list code is designed to be
very parallel either, since it functions fairly similarly to a traditional linked list
which means there are tons of dependencies anyways. However, the memory structure is
more optimal in most cases, so it will be faster regardless. Overall, a decent result.
I think my linked list can definitely be a LOT faster if I make optimizations, so
the current speed doesn't impress me despite it being faster than the standard library.