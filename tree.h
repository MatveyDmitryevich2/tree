#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdbool.h>

#include "stack.h"

typedef char* tree_type;

const char YES[] = "да";
const char NO[] = "нет";
const char EXIT[] = "выход";
const int SIZE_ANSWER = 13;
const char NAME_FILE_STOR[] = "tree.txt"; 
const size_t MAX_SIZE_LINE = 500;
const size_t MAX_TREE_DEPTH = 50;

enum Game_modes
{
    Game_modes_ELEM_SEARCH                     = 1,
    Game_modes_MAKE_DEFENITION_ELEM            = 2,
    Game_modes_FIND_COMMONALITIES_BETWEEN_ELEM = 3,
    Game_modes_EXIT                            = 4,
};

enum To_build_definitions_elem
{
    LEFT_YES = 1,
    RIGHT_NO = 0,
    PARENT   = -1,
};

enum Is_found
{
    FOUND     = 1,
    NOT_FOUND = 0,
};

struct Node
{
    char* elem;
    Node* left;
    Node* right;
    Node* parent;
};

Node* Made_root(Node* root);
void Game_mode_selection(Node* node);
void Saving_tree(Node* node);
void Tree_dtor(Node* node);

#endif //TREE_H