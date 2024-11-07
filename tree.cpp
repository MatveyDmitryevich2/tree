#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#include "tree.h"

void Insert_func(Node* node, tree_tipe argument)
{
    if (argument < node->elem)
    {
        if (node->left == NULL) { node->left = Create_node(argument); }
        else { Insert_func(node->left, argument); }
    }
    else
    {
        if (node->right == NULL) { node->right = Create_node(argument); }
        else { Insert_func(node->right, argument); }
    }
}

Node* Create_node(tree_tipe argument)
{
    Node* node = (Node*)calloc(1, sizeof(Node));
    assert(node != NULL);

    node->elem = argument;

    return node;
}


void Print_tree(Node* node)
{
    if (!node) { return; }
    fprintf(stderr, "(%ld", node->elem);
    if (node->left) { Print_tree(node->left); }
    if (node->right) { Print_tree(node->right); }
    fprintf(stderr, ")");
}

// void Tree_dtor(Node* node)
// {
//     if (!node) { return; }
//     free(node->left);
//     node->left = NULL;
//     free(node->right);
//     node->right = NULL;
//     if (node) { Tree_dtor(node); }
// }

void Tree_dtor(Node* node)
{
    if (!node) { return; }

    if (node->left) { Tree_dtor(node->left); }
    if (node->right) { Tree_dtor(node->right); }

    free(node->left);
    node->left = NULL;

    free(node->right);
    node->right = NULL;
}