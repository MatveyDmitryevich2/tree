#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#include "tree.h" 

typedef int64_t tree_tipe;

struct Node
{
    tree_tipe elem;
    Node* left;
    Node* right;
};

void Insert_func(Node* node, tree_tipe argument);
Node* Create_node(tree_tipe argument);
void Print_tree(Node* node);
void Tree_dtor(Node* node);

#endif //TREE_H