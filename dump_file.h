#ifndef DUMP_FILE_H
#define DUMP_FILE_H

#include <stdio.h>

#include "tree.h"

void Dump(Node* root);
void Write_before_body();
void Write_body();
void Write_html();
void Generate_nodes(Node* node, FILE* file);

#endif //DUMP_FILE_H