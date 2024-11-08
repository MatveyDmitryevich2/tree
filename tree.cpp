#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#include "tree.h"

void Insert_func(Node* node)
{
    fprintf(stderr, "Загаданный вами объект %s?\n", node->elem);

    char answer[8] = "";
    scanf("%s", answer);

    if (strcmp(answer, YES) == 0)
    {
        if (node->left == NULL)
        {
            fprintf(stderr, "Славно\n\n\n");
            Up_to_root(&node);
            Insert_func(node);
        }
        else { Insert_func(node->left); }
    }
    else if (strcmp(answer, NO) == 0)
    {
        if (node->right == NULL) 
        {
            node->right = Create_node_right(node);
            node->left = Create_node_left(node->elem, node);
            fprintf(stderr, "Напишите, пожалуйста, вопрос(с маленькой буквы),"
            "по которму можно отличить %s и загаданный Вами объект\n", node->elem);
            size_t a = 0;
            long int size_line = getline(&node->elem, &a, stdin);
            if(node->elem[size_line - 1] == '\n') { node->elem[size_line - 1] = '\0'; }
            Up_to_root(&node);
            Insert_func(node);
        }
        else { Insert_func(node->right); }
    }
    else
    {
        Up_to_root(&node);
        return;
    }
}

void Up_to_root(Node** node_ptr)
{
    while ((*node_ptr)->parent != NULL)
    {
        *node_ptr = (*node_ptr)->parent;
    }
}

Node* Create_node_left(char* old_answer, Node* parent_node)
{
    Node* node = (Node*)calloc(1, sizeof(Node));
    node->parent = parent_node;

    node->elem = old_answer;

    return node;
}

Node* Create_node_right(Node* parent_node)
{
    Node* node = (Node*)calloc(1, sizeof(Node));
    node->parent = parent_node;

    fprintf(stderr, "Напишите, пожалуйста, объект(с маленькой буквы) который Вы загадли\n");

    int b = 0;
    size_t a = 0;
    while ((b = getchar()) != '\n' && b != EOF) { }
    long int size_line = getline(&node->elem, &a, stdin);
    if(node->elem[size_line - 1] == '\n') { node->elem[size_line - 1] = '\0'; }

    return node;
}

void Tree_dtor(Node* node)
{
    if (!node) { return; }

    if (node->left) { Tree_dtor(node->left); }
    if (node->right) { Tree_dtor(node->right); }

    free(node->left);
    node->left = NULL;

    free(node->elem);
    node->left = NULL;

    free(node->right);
    node->right = NULL;
}