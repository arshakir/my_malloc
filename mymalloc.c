#include "mymalloc.h"
#include "tree.h"
#include <assert.h>
#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

Node *tree;
size_t tree_size = sizeof(Node);
size_t max_avail = 0;

size_t round_up(size_t size, size_t pagesize) {
  return ((size + pagesize - 1) / pagesize) * pagesize;
}

void *my_malloc(size_t size) {

  size_t pagesize = sysconf(_SC_PAGESIZE);

  if (size > max_avail) {

    size_t alloc_size = round_up(size, pagesize);

    Node *block = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED)
      return NULL;

    block->data = alloc_size;
    insertNode(block);
    max_avail = alloc_size;
  }

  Node *closest = lower_bound(size);
  assert(deleteNode(closest) == closest);
  int redo_max = 0;
  if (max_avail == closest->data) {
    redo_max = 1;
  }

  // Either split node into 2 or allocate all
  if (closest->data - size >= tree_size + 4) {

    size_t total = closest->data;
    closest->data = size + tree_size;

    Node *node2 = (void *)closest + closest->data;
    node2->data = total - closest->data;

    insertNode(node2);
    if (redo_max) {
      max_avail = max_val();
    }
    print_tree();
    return (void *)closest + tree_size;
  } else {
    if (redo_max) {
      max_avail = max_val();
    }
    print_tree();
    return (void *)closest + tree_size;
  }
}

void my_free(void *ptr) {
  Node* n = ptr - tree_size;
  insertNode(n);
  print_tree();
}

void *my_calloc(size_t num, size_t size) {

  size_t total_size = num*size;
  void* ptr = my_malloc(total_size);

  if (ptr == NULL){
    return NULL;
  }

  memset(ptr, 0, total_size);
  return ptr;
}

void *my_realloc(void *ptr, size_t size) { return NULL; }
