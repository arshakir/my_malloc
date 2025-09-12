/* Implementation taken from https://www.eecs.umich.edu/courses/eecs380/ALG/niemann/s_rbt.txt */
#include <stddef.h>
typedef size_t T; /* type of item to be stored */
#define compLT(a, b) (a < b)
#define compEQ(a, b) (a == b)

/* Red-Black tree description */
typedef enum { BLACK, RED } nodeColor;

typedef struct Node_ {
  struct Node_ *left;   /* left child */
  struct Node_ *right;  /* right child */
  struct Node_ *parent; /* parent */
  nodeColor color;      /* node color (BLACK, RED) */
  T data; // Size of Node + space in block
  int free;
  struct Node_ *prev;
  struct Node_ *next;
} Node;

#define NIL &sentinel /* all leafs are sentinels */

Node *insertNode(Node *x);
Node *deleteNode(Node *z);
Node *lower_bound(T data);
Node* max_val();
void print_tree();
