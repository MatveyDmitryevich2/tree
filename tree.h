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
const int SIZE_ANSWER = 13;
const char NAME_FILE_STOR[] = "tree.txt"; 
const size_t MAX_SIZE_LINE = 500;

struct Node
{
    char* elem;
    Node* left;
    Node* right;
    Node* parent;
};

void Insert(Node* node);
void Up_to_root(Node** node);
void Insert_new_property(Node* node);
Node* Create_node(Node* parent_node);
Node* Create_left_сhild(char* old_answer, Node* parent_node);
Node* Insert_right_child(Node* old_node);
char* Getting_info_from_user(Node* node);
void Tree_dtor(Node* node);
void Recursive_tree_entry(Node* node, FILE* file_stor);
void Saving_tree(Node* node);
size_t Reads_file_size(FILE *file);
Node* Decoding_tree();
void Parsing_line(char** buffer, char* line_buffer);
Node* Parsing_tree(char** buffer, Node* parent);
Node* New_node(char* argument, Node* parent);
void Skip_parenthesis(char** buffer);

#endif //TREE_H