#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#include "tree.h"
#include "dump_file.h"
#include "stack.h"

void Insert(Node* node)
{
    assert(node != NULL);

    fprintf(stderr, "Загаданный вами объект %s?\n", node->elem);

    char answer[SIZE_ANSWER] = "";
    scanf("%s", answer);

    if (strcmp(answer, YES) == 0)
    {
        if (node->left == NULL)
        {
            fprintf(stderr, "Славно\n\n\n");
            Up_to_root(&node);
            Insert(node); 
        }
        else { Insert(node->left); }
    }
    else if (strcmp(answer, NO) == 0)
    {
        if (node->right == NULL) 
        {
            Insert_new_property(node);
        }
        else { Insert(node->right); }
    }
    else
    {
        Up_to_root(&node);
        return;
    }
}

void Insert_new_property(Node* node)
{
    assert(node != NULL);

    node->right = Insert_right_child(node);
    node->left = Create_left_сhild(node->elem, node);

    fprintf(stderr, "Напишите, пожалуйста, вопрос(с маленькой буквы),"
    "по которму можно отличить %s и загаданный Вами объект\n", node->elem);

    node->elem = Getting_info_from_user(node);

    Up_to_root(&node);
    Insert(node);
}

Node* Create_node(Node* parent_node)
{
    assert(parent_node != NULL);

    Node* node = (Node*)calloc(1, sizeof(Node));
    node->parent = parent_node;

    return node;
}

Node* Create_left_сhild(char* old_answer, Node* parent_node)
{
    assert(old_answer != NULL);
    assert(parent_node != NULL);

    Node* node = Create_node(parent_node);

    node->elem = old_answer;

    return node;
}

Node* Insert_right_child(Node* parent_node)
{
    assert(parent_node != NULL);

    Node* node = Create_node(parent_node);

    fprintf(stderr, "Напишите, пожалуйста, объект(с маленькой буквы) который Вы загадли\n");
    int clean_buffer = 0;
    while ((clean_buffer = getchar()) != '\n' && clean_buffer != EOF) { }

    node->elem = Getting_info_from_user(node);

    return node;
}

char* Getting_info_from_user(Node* node)
{
    size_t size_new_condition = 0;
    long int size_line = getline(&node->elem, &size_new_condition, stdin);

    if(node->elem[size_line - 1] == '\n') { node->elem[size_line - 1] = '\0'; }

    return node->elem;
}

void Up_to_root(Node** node_ptr)
{
    assert(node_ptr != NULL);

    while ((*node_ptr)->parent != NULL)
    {
        *node_ptr = (*node_ptr)->parent;
    }
}

void Tree_dtor(Node* node)
{
    if (!node) { return; }

    if (node->left) { Tree_dtor(node->left); }
    if (node->right) { Tree_dtor(node->right); }

    free(node->elem);
    free(node);
}

void Recursive_tree_entry(Node* node, FILE* file_stor)
{
    if (!node) { return; }

    fprintf(file_stor, "{");
    fprintf(file_stor, "%s", node->elem);
    if (node->left) { Recursive_tree_entry(node->left, file_stor); }
    if (node->right) { Recursive_tree_entry(node->right, file_stor); }

    fprintf(file_stor, "}");
}

void Saving_tree(Node* node)
{
    assert(node != NULL);

    FILE* file_for_storing_tree = fopen(NAME_FILE_STOR, "r+");
    Recursive_tree_entry(node, file_for_storing_tree);
    fclose(file_for_storing_tree);
}

size_t Reads_file_size(FILE *file)
{
    assert(file != NULL);

    fseek(file, 0, SEEK_END);
    size_t size_file = (size_t)ftell(file);
    fseek(file, 0, SEEK_SET);

    return size_file;
}

Node* Decoding_tree()
{
    FILE* file_for_storing_tree = fopen(NAME_FILE_STOR, "r");

    size_t size_buffer = Reads_file_size(file_for_storing_tree);
    char* buffer = (char*)calloc(size_buffer + 1, sizeof(char));
    char* trash = buffer;

    fseek(file_for_storing_tree, 1, SEEK_SET);
    fread(buffer, sizeof(char), size_buffer, file_for_storing_tree);

    Node* root = Parsing_tree(&buffer, NULL);

    free(trash);
    fclose(file_for_storing_tree);

    return root;
}

Node* New_node(char* argument, Node* parent)
{
    Node* node = (Node*)calloc(1, sizeof(Node));
    node->elem = strdup(argument);
    node->parent = parent;

    return node;
}

Node* Parsing_tree(char** buffer, Node* parent)
{
    if (**buffer == '\0') { return NULL; }

    char line_buffer[MAX_SIZE_LINE] = {};

    Parsing_line(buffer, line_buffer);
    Node* node = New_node(line_buffer, parent);

    if (**buffer == '{')
    {
        Skip_parenthesis(buffer);
        node->left = Parsing_tree(buffer, node);
        Skip_parenthesis(buffer);
    }
    if (**buffer == '{')
    {
        Skip_parenthesis(buffer);
        node->right = Parsing_tree(buffer, node);
        Skip_parenthesis(buffer);
    }

    return node;
}

void Skip_parenthesis(char** buffer)
{
    (*buffer)++;
}

void Parsing_line(char** buffer, char* line_buffer)
{
    char* start = *buffer;
    char* end = *buffer;

    while((*end != '{') && (*end != '}') && (*end != '\0')) { end++; }

    size_t size_line = end - start;
    strncpy(line_buffer, start, size_line);
    line_buffer[size_line] = '\0';

    *buffer = end;
}
//---------------------------------------------finding definitions------------------------------------------------------

void Make_definition_elem(Node* node, char* elem)
{
    Stack_t Path_to_elem = {};
    StackConstrtor(&Path_to_elem, MAX_TREE_DEPTH);

    Recursive_search_path_to_elem(node, elem, &Path_to_elem, PARENT);

    for (long long i = 0; i <= (long long)Path_to_elem.vacant_place - 1; i++)
    {
        fprintf(stderr, "   [%lld] = ", i);
        fprintf(stderr, "%p", Path_to_elem.array_data[i].Adress);
        if (Path_to_elem.array_data[i].True_or_False == LEFT_YES) { fprintf(stderr, "    YES\n"); }
        else { fprintf(stderr, "    NO\n"); }
    }

    Definition_output(Path_to_elem.vacant_place - 1, Path_to_elem.array_data);
    
    StackDtor(&Path_to_elem);
}

IS_FOUND Recursive_search_path_to_elem(Node* node, char* elem, Stack_t* Path_to_elem, bool Left_or_right)
{
    Info_about_unit_of_path node_info = {(char*)node, Left_or_right};
    StackPush(Path_to_elem, (StackElem_t)node_info);

    if ((!node->left) && (!node->right))
    {
        if   (strcmp(elem, node->elem) == 0)                        { return FOUND; }
        else {StackElem_t trash = {}; StackPop(Path_to_elem, &trash); return NOT_FOUND; } 
    }

    IS_FOUND is_found_left  = Recursive_search_path_to_elem(node->left, elem, Path_to_elem, LEFT_YES);
    IS_FOUND is_found_right = Recursive_search_path_to_elem(node->right, elem, Path_to_elem, RIGHT_NO);

    if ((is_found_left == FOUND) || (is_found_right == FOUND)) { return FOUND; }

    StackElem_t trash = {}; StackPop(Path_to_elem, &trash);

    return NOT_FOUND;
}

void Definition_output(size_t array_size, Info_about_unit_of_path* array_properties)
{
    fprintf(stderr, "\n%s:", ((Node*)array_properties[array_size].Adress)->elem);
    for(size_t i = 1; i <= array_size; i++)
    {
        if (array_properties[i].True_or_False == LEFT_YES)
        {
            fprintf(stderr, " ");
        }
        else
        {
            fprintf(stderr, " не ");
        }
        fprintf(stderr, "%s", ((Node*)array_properties[i - 1].Adress)->elem);
    }
    fprintf(stderr, "\n");
}

// void Comparison_elem_definitions(Node* node, char* elem1, char* elem2)
// {
//     Make_definition_elem(node, elem1);
//     Make_definition_elem(node, elem2);

//     //Info_about_unit_of_path* array_properties_1 = (Info_about_unit_of_path*)calloc(Path_to_elem.vacant_place, sizeof(Info_about_unit_of_path));
// }

















































// void Recursive_tree_entry(Node* node, FILE* file_stor, int level)
// {
//     if (!node) { return; }

//     for (int i = 0; i < level; ++i) { fprintf(file_stor, "    "); }
//     fprintf(file_stor, "{\n");

//     for (int i = 0; i < level; ++i) { fprintf(file_stor, "    "); }
//     fprintf(file_stor, "%s\n", node->elem);

//     if (node->left) { Recursive_tree_entry(node->left, file_stor, level + 1); }
//     if (node->right) { Recursive_tree_entry(node->right, file_stor, level + 1); }

//     for (int i = 0; i < level; ++i) { fprintf(file_stor, "    "); }
//     fprintf(file_stor, "}\n");
// }