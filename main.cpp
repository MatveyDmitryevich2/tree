#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "tree.h" 

int main()
{
    Node root = {};
    root.elem = 50;

    Insert_func(&root, 30);
    Insert_func(&root, 70);
    Insert_func(&root, 10);
    Insert_func(&root, 65);
    Insert_func(&root, 80);
    Insert_func(&root, 66);

    Print_tree(&root);
    Tree_dtor(&root);

    return 0;
}