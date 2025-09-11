#include "mymalloc.h"
#include "tree.h"
#include <stddef.h>
#include <stdio.h>
int main() { 

  Node x;
  x.data = 25;
  Node* tree = insertNode(&x);

  Node y; y.data = 50;
  tree = insertNode(&y);

  Node z; z.data = 100;
  tree = insertNode(&z);

  deleteNode(&x);
  printf("%d",(int)lower_bound(1)->data);

  return 0;
}
