#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "tree.h"
#include "dump_file.h"

int main()
{
    Write_before_body();
    Write_body();

    Node root = {};
    root.parent != NULL;
    char* first_question = (char*)calloc(1, 30);
    strcpy(first_question, "полторашка");
    root.elem = first_question;

    Insert_func(&root);


    Dump(&root);
    Write_body();
    Write_html();

    Tree_dtor(&root);

    return 0;
}