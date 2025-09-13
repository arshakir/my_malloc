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

size_t round_up(size_t size, size_t pagesize) {
  return ((size + pagesize - 1) / pagesize) * pagesize;
}

void *my_malloc(size_t size) {

  size_t pagesize = sysconf(_SC_PAGESIZE);

  max_avail = max_val();

  if (size > max_avail) {

    size_t alloc_size = round_up(size, pagesize);

    Node *block = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED)
      return NULL;

    block->data = alloc_size;
    block->free = 1;
    block->prev = NULL;
    block->next = NULL;
    block->mmap_size = alloc_size;
    insertNode(block);
  }

  Node *closest = lower_bound(size);

  deleteNode(closest);
  closest->free = 0;

  // Either split node into 2 or allocate all
  if (closest->data - size >= tree_size + MIN_SIZE) {

    size_t total = closest->data;
    closest->data = size + tree_size;

    Node *node2 = (void *)closest + closest->data;
    node2->data = total - closest->data;
    node2->free = 1;
    node2->mmap_size = closest->mmap_size;
    node2->prev = closest;
    closest->next = node2;

    insertNode(node2);
    // printf("MAXVAL: %d\n", (int)max_val()->data);
    print_tree();
    return (void *)closest + tree_size;
  } else {
    // printf("MAXVAL: %d\n", (int)max_val()->data);
    print_tree();
    return (void *)closest + tree_size;
  }
}

void my_free(void *ptr) {

  Node *n = ptr - tree_size;
  Node *nforward = n->next;
  Node *nback = n->prev;

  if (nback && nback->free) {

    assert(deleteNode(nback) == nback);
    nback->free = 0;

    nback->data += n->data;

    if (nforward)
      nforward->prev = nback;

    nback->next = n->next;
    n = nback;
  }

  if (nforward && nforward->free) {
    assert(deleteNode(nforward) == nforward);
    nforward->free = 0;
    n->data += nforward->data;
    n->next = nforward->next;
    if (nforward->next)
      nforward->next->prev = n;
  }

  if (n->data == n->mmap_size) {
    munmap(n, n->data);
  } else {
    n->free = 1;
    insertNode(n);
  }

  printf("MAXVAL: %d\n", (int)max_val());
  print_tree();
}

void *my_calloc(size_t num, size_t size) {

  size_t total_size = num * size;
  void *ptr = my_malloc(total_size);

  if (ptr == NULL) {
    return NULL;
  }

  memset(ptr, 0, total_size);
  return ptr;
}

void *my_realloc(void *ptr, size_t size) {

  if (ptr == NULL) {
    return my_malloc(size);
  }

  if (size == 0) {
    my_free(ptr);
    return NULL;
  }

  void *ptr2 = my_malloc(size);
  if (ptr2 == NULL)
    return NULL;

  Node *n = ptr - tree_size;

  if (n->data - tree_size < size) {
    size = n->data - tree_size;
  }

  memcpy(ptr2, ptr, size);
  my_free(ptr);
  return ptr2;
}
