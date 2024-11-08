#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>

#include "dump_file.h"

int node_count = 0;

void Generate_nodes(Node* node, FILE* file)
{
    if (node == NULL) {
        return;
    }

    char node_id[100];
    snprintf(node_id, sizeof(node_id), "node_%d", node_count++);

    fprintf(file, "    %s [style=filled, fillcolor=lightyellow, label=\"%s\"];\n", node_id, node->elem);

    if (node->left != NULL || node->right != NULL) {
        if (node->left != NULL) {
            char left_id[100];
            snprintf(left_id, sizeof(left_id), "node_%d", node_count);
            fprintf(file, "    %s -> %s [color=\"#A600A6\", label=\"да\"];\n", node_id, left_id);
            Generate_nodes(node->left, file);
        }

        if (node->right != NULL) {
            char right_id[100];
            snprintf(right_id, sizeof(right_id), "node_%d", node_count);
            fprintf(file, "    %s -> %s [color=\"#A600A6\", label=\"нет\"];\n", node_id, right_id);
            Generate_nodes(node->right, file);
        }
    }
}

void Dump(Node* root) 
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    long seconds = tv.tv_sec;
    long microseconds = tv.tv_usec;
    
    char filename[100] = {0};
    snprintf(filename, sizeof(filename), "file_%ld_%06ld.dot", seconds, microseconds);
    FILE* file_html = fopen("dump.html", "a+");
    FILE* file_dump = fopen(filename, "a+");

    fprintf(file_dump, "digraph structs {\n");
    fprintf(file_dump, "    charset = \"UTF-8\";\n");
    fprintf(file_dump, "    rankdir=TB;\n");
    fprintf(file_dump, "    bgcolor = \"#BFBA30\";\n");
    fprintf(file_dump, "    fontcolor = black;\n");
    fprintf(file_dump, "    fontsize = 18;\n");
    fprintf(file_dump, "    style = \"rounded\";\n");
    fprintf(file_dump, "    margin = 0.3;\n");
    fprintf(file_dump, "    splines = ortho;\n");
    fprintf(file_dump, "    ranksep = 1.0;\n");
    fprintf(file_dump, "    nodesep = 0.9;\n");
    fprintf(file_dump, "    edge [color=\"#A600A6\", style=solid, penwidth=2];\n");

    Generate_nodes(root, file_dump);

    fprintf(file_dump, "}\n");

    fclose(file_dump);

    char command[256];
    snprintf(command, sizeof(command), "dot -Tpng %s -o %.22s.png", filename, filename);
    system(command);

    snprintf(filename, sizeof(filename), "file_%ld_%06ld.png", seconds, microseconds);
    fprintf(file_html, "<img src=\"%s\"/>\n", filename);
    fclose(file_html);
}



void Write_before_body()
{
    FILE* html_file = fopen("dump.html", "a+");
    fprintf(html_file, "<!DOCTYPE html>\n<html lang=\"en\">\n\n<head>\n<meta charset=\"utf-8\">\n"
                       "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
                       "<title>list dump</title>\n<link rel=\"stylesheet\" href=\"styles.css\">\n</head>\n\n");
    fclose(html_file);
}

void Write_body()
{
    FILE* html_file = fopen("dump.html", "a+");
    fprintf(html_file, "<body>\n");

    fclose(html_file);
}

void Write_html()
{
    FILE* html_file = fopen("dump.html", "a+");
    fprintf(html_file, "</html>\n");
    
    fclose(html_file);
}