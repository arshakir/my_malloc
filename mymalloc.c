/*
 * Custom malloc implementation
 *
 * Each free block that can be allocated is held in a red and black tree
 * a new block can be found by finding the lower bound of the tree for that size
 * Nodes also held in a linkedlist to its neighboring blocks so that they can be merged when freed
 *
 */
#include "mymalloc.h"
#include "tree.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

Node *tree;
size_t tree_size = sizeof(Node);
size_t max_avail = 0;
#define MIN_SIZE 4 // Minimum size for a block to be created for

// Rounds the value size to nearest pagesize to pass to mmap
size_t round_up(size_t size, size_t pagesize) {
  return ((size + pagesize - 1) / pagesize) * pagesize;
}

void *my_malloc(size_t size) {

  size_t pagesize = sysconf(_SC_PAGESIZE);
  max_avail = max_val();

  // Check if the largest value in tree can fit the requested size
  if (size > max_avail) {

    // Request the rounded page size through mmap
    size_t alloc_size = round_up(size, pagesize);
    Node *block = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (block == MAP_FAILED)
      return NULL;

    // Node setup
    block->data = alloc_size;
    block->free = 1;
    block->prev = NULL;
    block->next = NULL;
    block->mmap_size = alloc_size;

    insertNode(block);
  }

  // Find lower bound of free tree, remove it and set to unfree
  Node *closest = lower_bound(size);
  deleteNode(closest);
  closest->free = 0;

  // If the block can support another tree node + some minium size of the
  // pointer split or else allocate all to that node
  if (closest->data - size >= tree_size + MIN_SIZE) {

    // Restrict closest data to the requested size
    size_t total = closest->data;
    closest->data = size + tree_size;

    // Create a new node with the remaining bytes
    Node *node2 = (void *)closest + closest->data;
    node2->data = total - closest->data;
    node2->free = 1;
    node2->mmap_size = closest->mmap_size;

    // Set up linkedlist
    node2->prev = closest;
    closest->next = node2;

    // Add it back to tree
    insertNode(node2);
    print_tree();
    return (void *)closest + tree_size;
  } else {
    // Just return entire block
    print_tree();
    return (void *)closest + tree_size;
  }
}

void my_free(void *ptr) {

  // Get the node from ptr, and the front and back using linkedlist
  Node *n = ptr - tree_size;
  Node *nforward = n->next;
  Node *nback = n->prev;

  // Merge nback with n
  if (nback && nback->free) {

    // Remove from tree, set to unfree and add data together
    assert(deleteNode(nback) == nback);
    nback->free = 0;
    nback->data += n->data;

    // Fix linkedlist
    if (nforward)
      nforward->prev = nback;

    nback->next = n->next;

    // Set n to nback and stop using old n (merged with nback)
    n = nback;
  }

  // Same thing but with forward node
  if (nforward && nforward->free) {

    assert(deleteNode(nforward) == nforward);
    nforward->free = 0;
    n->data += nforward->data;

    // Fix linkedlist
    n->next = nforward->next;
    if (nforward->next)
      nforward->next->prev = n;
  }

  // If all the data is merged into the same block that was requested when mmap
  // we can return it back
  //  Else just add it back into the free tree
  if (n->data == n->mmap_size) {
    munmap(n, n->data);
  } else {
    n->free = 1;
    insertNode(n);
  }

  printf("MAXVAL: %d\n", (int)max_val());
  print_tree();
}

/* return a pointer to the total size and zero it out */
void *my_calloc(size_t num, size_t size) {

  size_t total_size = num * size;
  void *ptr = my_malloc(total_size);

  if (ptr == NULL) {
    return NULL;
  }

  memset(ptr, 0, total_size);
  return ptr;
}

/* returns the a new ptr that holds the same value but in the requested size */
void *my_realloc(void *ptr, size_t size) {

  // No pointer given same as malloc
  if (ptr == NULL) {
    return my_malloc(size);
  }

  // No size given same as free
  if (size == 0) {
    my_free(ptr);
    return NULL;
  }

  //Get a pointer to new size
  void *ptr2 = my_malloc(size);
  if (ptr2 == NULL)
    return NULL;

  //Get the minimum size to copy over
  Node *n = ptr - tree_size;
  if (n->data - tree_size < size) {
    size = n->data - tree_size;
  }

  //Copy over size, free old pointer and return new pointer
  memcpy(ptr2, ptr, size);
  my_free(ptr);

  return ptr2;
}
