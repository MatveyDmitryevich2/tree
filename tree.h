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

typedef char* tree_type;

const char YES[] = "да";
const char NO[] = "нет";

struct Node
{
    char* elem;
    Node* left;
    Node* right;
    Node* parent;
};

void Insert_func(Node* node);
void Up_to_root(Node** node);
Node* Create_node_left(char* old_answer, Node* old_node);
Node* Create_node_right(Node* old_node);
void Tree_dtor(Node* node);

#endif //TREE_H