#include "mymalloc.h"
#include <stdio.h>
#include <string.h>
int main() { 

  char* x = my_malloc(16);
  char* a = my_malloc(20);
  char* b = my_malloc(50);
  char* c = my_malloc(70);
  char* s = my_malloc(10);
  strcpy(s, "hi");
  printf("%s", s);
  my_free(c);
  my_free(s);
  my_free(x);
  my_free(a);
  my_free(b);

  x = my_malloc(20);
  a = my_malloc(50);
  x = my_realloc(x, 20000);
  my_free(a);
  my_free(x);

  x = my_calloc(50, 50);
  a = my_calloc(1, 15000);
  my_free(x);
  my_free(a);
  b = my_malloc(20);
  my_free(b);
  return 0;
}
