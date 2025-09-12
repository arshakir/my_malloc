#include "mymalloc.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
int main() { 

  char* a = my_malloc(20);
  char* b = my_malloc(50);
  char* c = my_malloc(70);
  char* s = my_malloc(10);
  my_free(s);
  my_free(c);
  my_free(b);
  my_free(a);
  strcpy(s, "hi");
  printf("%s", s);
  return 0;
}
