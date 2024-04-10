#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  return p;
}

void inOrder_delete(rbtree*t, node_t *p) {
  if (p != t->nil) {
    inOrder_delete(t, p->left);
    inOrder_delete(t, p->right);
    free(p);
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *p = t->root;
  inOrder_delete(t, p);
  free(t->nil);
  free(t);
}

void left_rotate(rbtree *t, node_t *p) {
  node_t *y = p->right;
  p->right = y->left;
  if (y->left != t->nil) y->left->parent = p;
  y->parent = p->parent;
  if (p->parent == t->nil) t->root = y;
  else if (p->parent->left == p) p->parent->left = y;
  else p->parent->right = y;
  y->left = p;
  p->parent = y;
}

void right_rotate(rbtree *t, node_t *p) {
  node_t *y = p->left;
  p->left = y->right;
  if (y->right != t->nil) y->right->parent = p;
  y->parent = p->parent;
  if (p->parent == t->nil) t->root = y;
  else if (p->parent->left == p) p->parent->left = y;
  else p->parent->right = y;
  y->right = p;
  p->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *p) {
  while(p->parent->color == RBTREE_RED) {
    if (p->parent == p->parent->parent->left) {
      node_t *y = p->parent->parent->right;
      if (y->color == RBTREE_RED) {
        p->parent->color = y->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        p = p->parent->parent;
      }
      else {
        if (p == p->parent->right) {
          p = p->parent;
          left_rotate(t, p);
        }
        p->parent->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        right_rotate(t, p->parent->parent);
      }
    }
    else {
      node_t *y = p->parent->parent->left;
      if (y->color == RBTREE_RED) {
        p->parent->color = y->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        p = p->parent->parent;
      }
      else {
        if (p == p->parent->left) {
          p = p->parent;
          right_rotate(t, p);
        }
        p->parent->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        left_rotate(t, p->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *tmp = (node_t *)calloc(1, sizeof(node_t));
  tmp->key = key;
  tmp->color = RBTREE_RED;
  tmp->left = tmp->right = t->nil;

  if (t->root == t->nil) {
    tmp->parent = t->nil;
    t->root = tmp;
  }
  else {
    node_t *x = t->root;
    node_t *y;
    int z = 0;
    while(x != t->nil) {
      y = x;
      if (x->key >= key) {
        x = x->left;
        z = 1;
      }
      else {
        x = x->right;
        z = 0;
      }
    }
    if (z) {
      tmp->parent = y;
      y->left = tmp;
    }
    else {
      tmp->parent = y;
      y->right = tmp;
    }
  }
  rbtree_insert_fixup(t, tmp);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  if (t->root == t->nil) return NULL;
  else {
    node_t *tmp = t->root;
    while (tmp != t->nil) {
      if (key == tmp->key) return tmp;
      if (key < tmp->key) tmp = tmp->left;
      else tmp = tmp->right;
    }
    return NULL;
  }
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil) return NULL;
  else {
    node_t *tmp = t->root;
    while(tmp->left != t->nil) {
      tmp = tmp->left;
    }
    return tmp;
  }
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil) return NULL;
  else {
    node_t *tmp = t->root;
    while(tmp->right != t->nil) {
      tmp = tmp->right;
    }
    return tmp;
  }
}

node_t *subtree_min(rbtree *t, node_t *p) {
  while(p->left != t->nil) {
    p = p->left;
  }
  return p;
}

node_t *subtree_max(rbtree *t, node_t *p) {
  while(p->right != t->nil) {
    p = p->right;
  }
  return p;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil) t->root = v;
  else if (u == u->parent->left) u->parent->left = v;
  else u->parent->right = v;
  v->parent = u->parent;
}

void rb_delete_fixup(rbtree *t, node_t *x) {
  while(x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      node_t *w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else {
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *x;
  node_t *y = z;
  color_t y_original_color = y->color;
  if (z == NULL || z == t->nil) return 0;
  else {
    if (z->left == t->nil) {
      x = z->right;
      rb_transplant(t, z, z->right);
    }
    else if(z->right == t->nil) {
      x = z->left;
      rb_transplant(t, z, z->left);
    }
    else {
      y = subtree_min(t, z->right);
      y_original_color = y->color;
      x = y->right;
      if (y->parent == z) x->parent = y;
      else {
        rb_transplant(t, y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }
      rb_transplant(t, z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }
  }
  free(z);
  if (y_original_color == RBTREE_BLACK) rb_delete_fixup(t, x);
  return 0;
}

void inOrder(const rbtree *t, node_t *p, key_t *arr, size_t *index, size_t n) {
    if (p != t->nil && *index < n) {
        inOrder(t, p->left, arr, index, n);
        if (*index < n) {
            arr[*index] = p->key;
            (*index)++;
        }
        inOrder(t, p->right, arr, index, n);
    }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  size_t index = 0;
  node_t *p = t->root;
  inOrder(t, p, arr, &index, n);
  return 0;
}