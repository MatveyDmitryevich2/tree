#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "tree.h"
#include "dump_file.h"

int main()
{
    Write_html_mode(Dump_html_BEGIN);

    Node* root = NULL;
    root = Made_root(root);

    Game_mode_selection(root);

    Dump(root); 
    Write_html_mode(Dump_html_END);

    Saving_tree(root);
    Tree_dtor(root);

    return 0;
}