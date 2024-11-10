#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "tree.h"
#include "dump_file.h"

int main()
{
    Write_before_body();
    Write_body();

    Node* root = Decoding_tree();
    if (root == NULL)
    {
        root = (Node*)calloc(1, sizeof(Node));
        char* first_question = (char*)calloc(1, 30);
        strcpy(first_question, "1.5шка");
        root->elem = first_question;
    }
    Insert(root);

    Dump(root);
    Write_body();
    Write_html();

    Make_definition_elem(root, "дырка");

    Saving_tree(root);
    Tree_dtor(root);
    return 0;
}