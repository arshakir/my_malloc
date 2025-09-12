#include "mymalloc.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
int main() { 

  char* x = my_malloc(16384);
  char* a = my_malloc(20);
  char* b = my_malloc(50);
  char* c = my_malloc(70);
  char* s = my_malloc(10);
  strcpy(s, "hi");
  printf("%s", s);
  my_free(a);
  my_free(b);
  my_free(c);
  my_free(s);
  my_free(x);
  x = my_calloc(50, 50);
  a = my_calloc(1, 15000);
  my_free(x);
  my_free(a);
  b = my_malloc(20);
  return 0;
}
