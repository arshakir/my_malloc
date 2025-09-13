# Malloc Implementation

## Overview

This project implements a custom memory allocater in C. Supports the usual memory allocation functions in C: `malloc`, `free`, `calloc`, `realloc`

## Running it

1. Clone repo

2. Build and run tests
```bash
make run
```


3. Using in your code
```c
#include "mymalloc.h"
```
+ Compile along with `mymalloc.c`
```bash
gcc -o yourprogram yourprogram.c mymalloc.c
```

## How it works

### Design:

The basic idea behind this allocater is that the memory blocks are held in the Nodes of a red black tree. Each block has a header containing the Node data and the rest of the block is the actual data associated with the block that is given to the user to use. Using the red-black tree allows the allocater to find the minimum block that can fit the requested size in O(log n) time and return it back. 

Additionally, when blocks are split, they contain pointers to the previous and next nodes that they were split from (doubly linked list) so that they can be merged back together


### Functions: 
#### `my_malloc`:

When the user requests a size we will check if the free tree currently has a node that is larger than that size by getting the max_val of the tree. If there is not a node we can allocate a block using `mmap` and add that into the tree

We can then find a node in the tree that is bigger than the requested size (lower_bound) and remove it from the tree. We either choose to split this node into 2 and add the remaining back into the tree or return the whole node back to the user. We then return a pointer to the data after the header data *(Node + size) which the user can then use

#### `my_free`:

When the user frees we can use the size of the header data to go back and get the actual node for that pointer. We can then use the linked list to check the forward and previous nodes and merge them with the current node. Once we merge if the whole block is equal to the block that was requested when we used `mmap` we can use `munmap` and return it back. If not we will just add that node back into the tree for later use.

#### `my_calloc`:

This can be implemented entirely by `my_malloc`. First get the total size by multiply the number of elements and size. Then use `my_malloc` to get a pointer to that size and zero it out and return it.

#### `my_realloc`:
Also implemented using `my_malloc` and `my_free`. Use `my_malloc` to get a pointer to the new size, then copy over the data from the old pointer into the new one making sure to only copy the min size of either blocks.

## TODO/Improvements
- Optimize by having different tree for different block sizes
- Hybrid approach like other implementations: lists for small blocks, tree for large blocks
- Improving edge cases and error handling
- Improving tree implementation


