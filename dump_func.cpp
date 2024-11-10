#include "dump_file.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>


static void Write_before_body();
static void Write_body();
static void Write_html();
static void Generate_nodes(Node* node, FILE* file);

//--------------------------------------------------global--------------------------------------------------------------

void Write_html_mode(Dump_html mode)
{
    if ( mode = Dump_html_BEGIN) { Write_before_body(); Write_body(); }
    if ( mode = Dump_html_END)   { Write_body(); Write_html();        }
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

    fprintf(file_dump, INFO_GRAPH);

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

//--------------------------------------------------static--------------------------------------------------------------

int node_count = 0;

static void Generate_nodes(Node* node, FILE* file)
{
    if (node == NULL) { return; }

    char node_id[100];
    snprintf(node_id, sizeof(node_id), "node_%d", node_count++);

    fprintf(file, "%s [\n", node_id);
    fprintf(file, "shape=plaintext,\n");
    fprintf(file, "label=<\n");
    fprintf(file, "<table border=\"0\" cellpadding=\"4\" cellborder=\"1\" width=\"300\">\n");
    fprintf(file, "<tr><td align=\"center\" colspan=\"2\">Parent: %p</td></tr>\n", (void*)node->parent);
    fprintf(file, "<tr><td align=\"center\" colspan=\"2\">%s</td></tr>\n", node->elem);
    fprintf(file, "<tr>\n");
    fprintf(file, "<td align=\"center\" width=\"150\">Left: %p</td>\n", (void*)node->left);
    fprintf(file, "<td align=\"center\" width=\"150\">Right: %p</td>\n", (void*)node->right);
    fprintf(file, "</tr>\n");
    fprintf(file, "</table>\n");
    fprintf(file, ">\n");
    fprintf(file, "];\n");

    if (node->left != NULL || node->right != NULL)
    {
        if (node->left != NULL)
        {
            char left_id[100];
            snprintf(left_id, sizeof(left_id), "node_%d", node_count);
            fprintf(file, "    %s -> %s [color=\"#A600A6\", label=\"да\"];\n", node_id, left_id);
            Generate_nodes(node->left, file);
        }

        if (node->right != NULL)
        {
            char right_id[100];
            snprintf(right_id, sizeof(right_id), "node_%d", node_count);
            fprintf(file, "    %s -> %s [color=\"#A600A6\", label=\"нет\"];\n", node_id, right_id);
            Generate_nodes(node->right, file);
        }
    }
}

static void Write_before_body()
{
    FILE* html_file = fopen("dump.html", "a+");
    fprintf(html_file, INFO_HTML);
    fclose(html_file);
}

static void Write_body()
{
    FILE* html_file = fopen("dump.html", "a+");
    fprintf(html_file, "<body>\n");

    fclose(html_file);
}

static void Write_html()
{
    FILE* html_file = fopen("dump.html", "a+");
    fprintf(html_file, "</html>\n");
    
    fclose(html_file);
}