# Paging and Replacement

In a computer operating system that uses paging for virtual memory management, page replacement algorithms decide which memory pages to page out, sometimes called swap out, or write to disk, when a page of memory needs to be allocated. Page replacement happens when a requested page is not in memory (page fault) and a free page cannot be used to satisfy the allocation, either because there are
none, or because the number of free pages is lower than some threshold.

## 1. Problem

It is required to simulate some of paging replacement algorithms. The required algorithms to be implemented are as follows:

- Optimal
- First In First Out (FIFO)
- Least Recently Used (LRU)
- Clock

## 2. Solution

1. Design

   1. Reading the input

      As specified in the problem, the input should contain cache size, selected replacement algorithm and the
      input stream (line separated). Reading the input would start by reading the cache size then allocating it,
      reading in the algorithm and finally getting the input stream until it’s terminated by a -1.

   2. Selecting the algorithm

      The algorithm selection would occur right after reading the input by specifying an array of the available
      algorithms options and starting the selected algorithm.

   3. Paging algorithms

      Each algorithm has its own body but all algorithms share utility functions such as specifying whether the
      input already exists in cache, print output state, find next empty index … etc.

   4. Printing the output

      To print the output that is exactly the same as the given output would be possible by printing the constant
      parts in the main and printing the variable part inside each algorithm after each insertion.

For more details, refer to the attached PDF Report.
