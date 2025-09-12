/* Implementation taken from
 * https://www.eecs.umich.edu/courses/eecs380/ALG/niemann/s_rbt.txt */
/* red-black tree */
#include "tree.h"
#include <stddef.h>
#include <stdio.h>
Node sentinel = {NIL, NIL, 0, BLACK, 0, 1};

Node *root = NIL; /* root of Red-Black tree */

void rotateLeft(Node *x) {

  /**************************
   *  rotate node x to left *
   **************************/

  Node *y = x->right;

  /* establish x->right link */
  x->right = y->left;
  if (y->left != NIL)
    y->left->parent = x;

  /* establish y->parent link */
  if (y != NIL)
    y->parent = x->parent;
  if (x->parent) {
    if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
  } else {
    root = y;
  }

  /* link x and y */
  y->left = x;
  if (x != NIL)
    x->parent = y;
}

void rotateRight(Node *x) {

  /****************************
   *  rotate node x to right  *
   ****************************/

  Node *y = x->left;

  /* establish x->left link */
  x->left = y->right;
  if (y->right != NIL)
    y->right->parent = x;

  /* establish y->parent link */
  if (y != NIL)
    y->parent = x->parent;
  if (x->parent) {
    if (x == x->parent->right)
      x->parent->right = y;
    else
      x->parent->left = y;
  } else {
    root = y;
  }

  /* link x and y */
  y->right = x;
  if (x != NIL)
    x->parent = y;
}

void insertFixup(Node *x) {

  /*************************************
   *  maintain Red-Black tree balance  *
   *  after inserting node x           *
   *************************************/

  /* check Red-Black properties */
  while (x != root && x->parent->color == RED) {
    /* we have a violation */
    if (x->parent == x->parent->parent->left) {
      Node *y = x->parent->parent->right;
      if (y->color == RED) {

        /* uncle is RED */
        x->parent->color = BLACK;
        y->color = BLACK;
        x->parent->parent->color = RED;
        x = x->parent->parent;
      } else {

        /* uncle is BLACK */
        if (x == x->parent->right) {
          /* make x a left child */
          x = x->parent;
          rotateLeft(x);
        }

        /* recolor and rotate */
        x->parent->color = BLACK;
        x->parent->parent->color = RED;
        rotateRight(x->parent->parent);
      }
    } else {

      /* mirror image of above code */
      Node *y = x->parent->parent->left;
      if (y->color == RED) {

        /* uncle is RED */
        x->parent->color = BLACK;
        y->color = BLACK;
        x->parent->parent->color = RED;
        x = x->parent->parent;
      } else {

        /* uncle is BLACK */
        if (x == x->parent->left) {
          x = x->parent;
          rotateRight(x);
        }
        x->parent->color = BLACK;
        x->parent->parent->color = RED;
        rotateLeft(x->parent->parent);
      }
    }
  }
  root->color = BLACK;
}

Node *insertNode(Node *x) {
  Node *current, *parent; //, *x;

  /***********************************************
   *  allocate node for data and insert in tree  *
   ***********************************************/

  /* find where node belongs */
  current = root;
  parent = 0;
  while (current != NIL) {
    // if (compEQ(data, current->data)) return (current);
    parent = current;
    current = compLT(x->data, current->data) ? current->left : current->right;
  }

  /*
  setup new node
  if ((x = malloc (sizeof(*x))) == 0) {
      printf ("insufficient memory (insertNode)\n");
      exit(1);
  }
  x->data = data;
  */
  x->parent = parent;
  x->left = NIL;
  x->right = NIL;
  x->color = RED;

  /* insert node in tree */
  if (parent) {
    if (compLT(x->data, parent->data))
      parent->left = x;
    else
      parent->right = x;
  } else {
    root = x;
  }

  insertFixup(x);
  return (x);
}

void deleteFixup(Node *x) {

  /*************************************
   *  maintain Red-Black tree balance  *
   *  after deleting node x            *
   *************************************/

  while (x != root && x->color == BLACK) {
    if (x == x->parent->left) {
      Node *w = x->parent->right;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        rotateLeft(x->parent);
        w = x->parent->right;
      }
      if (w->left->color == BLACK && w->right->color == BLACK) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->right->color == BLACK) {
          w->left->color = BLACK;
          w->color = RED;
          rotateRight(w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->right->color = BLACK;
        rotateLeft(x->parent);
        x = root;
      }
    } else {
      Node *w = x->parent->left;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        rotateRight(x->parent);
        w = x->parent->left;
      }
      if (w->right->color == BLACK && w->left->color == BLACK) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->left->color == BLACK) {
          w->right->color = BLACK;
          w->color = RED;
          rotateLeft(w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->left->color = BLACK;
        rotateRight(x->parent);
        x = root;
      }
    }
  }
  x->color = BLACK;
}

Node *deleteNode(Node *z) {
  Node *x, *y;

  /*****************************
   *  delete node z from tree  *
   *****************************/

  if (!z || z == NIL)
    return NULL;

  if (z->left == NIL || z->right == NIL) {
    /* y has a NIL node as a child */
    y = z;
  } else {
    /* find tree successor with a NIL node as a child */
    y = z->right;
    while (y->left != NIL)
      y = y->left;
  }

  /* x is y's only child */
  if (y->left != NIL)
    x = y->left;
  else
    x = y->right;

  /* remove y from the parent chain */
  x->parent = y->parent;
  if (y->parent)
    if (y == y->parent->left)
      y->parent->left = x;
    else
      y->parent->right = x;
  else
    root = x;

  if (y != z)
    z->data = y->data;

  if (y->color == BLACK)
    deleteFixup(x);

  // free (y);
  return y;
}

Node *findNode(T data) {

  /*******************************
   *  find node containing data  *
   *******************************/

  Node *current = root;
  while (current != NIL)
    if (compEQ(data, current->data))
      return (current);
    else
      current = compLT(data, current->data) ? current->left : current->right;
  return (0);
}

/* Added lower bound to find min node that can fit the size */
Node *lower_bound(T data) {
  Node *current = root;
  Node *res = NULL;
  while (current != NIL) {
    if (data <= current->data) {
      res = current;
      current = current->left;
    } else {
      current = current->right;
    }
  }

  return res;
}

T max_val() {
  Node *cur = root;
  size_t res = 0;
  while (cur != NIL) {
    res = cur->data;
    cur = cur->right;
  }

  return res;
}

void p(Node* r, int d){
  if (r == NIL){
    return;
  }

  p(r->right, d+1);
  for (int i=0; i < d; i++){
    printf("    ");
  }
  printf("%zu\n", r->data);
  p(r->left, d+1);
}

void print_tree(){
  p(root, 0);
  printf("\n\n\n");
}
