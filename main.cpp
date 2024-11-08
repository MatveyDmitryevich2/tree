#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "tree.h" 

int main()
{
    Node root = {};
    root.parent != NULL;
    char* first_question = (char*)calloc(1, 30);
    strcpy(first_question, "полторашка");
    root.elem = first_question;

    Insert_func(&root);

    Print_tree(&root);
    Tree_dtor(&root);

    return 0;
}