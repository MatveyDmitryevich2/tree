#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>

#include "dump_file.h"
#include "stack.h"

enum User_response
{
    User_response_YES   = 1,
    User_response_NO    = 0,
    User_response_EXIT  = 2,
};

enum Array_sizes
{
    Array_sizes_EQUAL         = 0,
    Array_sizes_FIRST_BIGGER  = 1,
    Array_sizes_SECOND_BIGGER = 2,

};

static void Mode_insert(Node* node);
static void Up_to_root(Node** node);
static void Insert_new_property(Node* node);
static Node* Create_left_сhild(char* old_answer, Node* parent_node);
static Node* Insert_right_child(Node* old_node);
static char* Getting_info_from_user(Node* node);
static void Recursive_tree_entry(Node* node, FILE* file_stor);
static size_t Reads_file_size(FILE* file);
static Node* Decod_tree();
static void Parsing_line(char** buffer, char* line_buffer);
static Node* Parsing_tree(char** buffer, Node* parent);
static Node* New_node(char* argument, Node* parent_node);
static void Skip_parenthesis(char** buffer);
static void Selecting_an_element(char** elem_v);
static Game_modes User_interaction();
static Is_found Recursive_search_path_to_elem(Node* node, char* elem, Stack_t* Path_to_elem,
                                              To_build_definitions_elem Left_or_right);
static void Definition_output(size_t array_size, Info_about_unit_of_path* array_properties);
static bool Is_node_object(Node* node);
static User_response Analyzing_user_response(char response[SIZE_ANSWER]);
static void Search_similarities_between_elems(size_t array_size1, Info_about_unit_of_path* array_properties1,
                                              size_t array_size2, Info_about_unit_of_path* array_properties2);
static void Conclusion_similarities_case_equal(size_t array_size, Info_about_unit_of_path* array_properties1,
                                               Info_about_unit_of_path* array_properties2);
static Array_sizes Compar_array_sizes(size_t array_size1, size_t array_size2);
static void Conclusion_similarities_case_bigger(size_t array_size1, Info_about_unit_of_path* array_properties1,
                                              size_t array_size2, Info_about_unit_of_path* array_properties2);
static void Clean_buffer();

//-------------------------------------------------global---------------------------------------------------------------

Node* Made_root(Node* root)
{
    //root can be NULL
    root = Decod_tree();

    if (root == NULL)
    {
        root = (Node*)calloc(1, sizeof(Node));
        char* first_question = (char*)calloc(1, 30);
        strcpy(first_question, "1.5шка");
        root->elem = first_question;
    }

    return root;
}

void Game_mode_selection(Node* node)
{
    switch(User_interaction())
    {
        case Game_modes_ELEM_SEARCH: 
        {
            Mode_insert(node);
        }  
        break;

        case Game_modes_MAKE_DEFENITION_ELEM: //FIXME вынести case в функцию
        {
            Stack_t Path_to_elem = {};
            StackConstrtor(&Path_to_elem, MAX_TREE_DEPTH);

            char* elem = NULL;
            Selecting_an_element(&elem);

            Recursive_search_path_to_elem(node, elem, &Path_to_elem, PARENT);
            Definition_output(Path_to_elem.vacant_place - 1, Path_to_elem.array_data);

            free(elem);
            StackDtor(&Path_to_elem);
        }  
        break;

        case Game_modes_FIND_COMMONALITIES_BETWEEN_ELEM: 
        {
            Stack_t Path_to_elem1 = {};
            Stack_t Path_to_elem2 = {};
            StackConstrtor(&Path_to_elem1, MAX_TREE_DEPTH);
            StackConstrtor(&Path_to_elem2, MAX_TREE_DEPTH);

            char* elem1 = NULL;
            char* elem2 = NULL;
            Selecting_an_element(&elem1);
            Selecting_an_element(&elem2);

            Recursive_search_path_to_elem(node, elem1, &Path_to_elem1, PARENT);
            Recursive_search_path_to_elem(node, elem2, &Path_to_elem2, PARENT);
            
            Search_similarities_between_elems(Path_to_elem1.vacant_place - 1, Path_to_elem1.array_data,
                                              Path_to_elem2.vacant_place - 1, Path_to_elem2.array_data);

            free(elem1);
            free(elem2);
            StackDtor(&Path_to_elem1);
            StackDtor(&Path_to_elem2);
        }  
        break;

        case Game_modes_EXIT: 
        break;

        default:
        {
            assert(0);
        }
    }
}

void Saving_tree(Node* node)
{
    assert(node != NULL);

    FILE* file_for_storing_tree = fopen(NAME_FILE_STOR, "r+");
    Recursive_tree_entry(node, file_for_storing_tree);
    fclose(file_for_storing_tree);
}

void Tree_dtor(Node* node)
{
    if (!node) { return; }

    if (node->left) { Tree_dtor(node->left); }
    if (node->right) { Tree_dtor(node->right); }

    free(node->elem);
    free(node);
}

//-----------------------------------------------------static-----------------------------------------------------------

static void Clean_buffer()
{
    int clean_buffer = 0;
    while ((clean_buffer = getchar()) != '\n' && clean_buffer != EOF) { }
}

static void Mode_insert(Node* node)
{
    assert(node != NULL);

    // NOTE
    fprintf(stderr, "Загаданный вами объект %s?\n", node->elem);

    // NOTE
    char answer[SIZE_ANSWER] = "";
    assert(SIZE_ANSWER == 13 /*поменять scanf %12s если упало*/);
    scanf("%12s", answer);
    Clean_buffer();

    switch (Analyzing_user_response(answer))
    {
        case User_response_YES:
        {
            if (Is_node_object(node))
            {
                fprintf(stderr, "Славно\n\n\n");
                Up_to_root(&node);
                Mode_insert(node);
            }
            else { Mode_insert(node->left); }
        } break;
        case User_response_NO:
        {
            if (Is_node_object(node)) { Insert_new_property(node); Up_to_root(&node); Mode_insert(node); }
            else                      { Mode_insert(node->right); }
        } break;
        case User_response_EXIT:
        {
            Up_to_root(&node);
        } break;
        default:
        {
            assert(0);
        } break;
    }
}

static User_response Analyzing_user_response(char response[SIZE_ANSWER])
{
    assert(response != NULL);

    if (strcmp(response, YES) == 0)       { return User_response_YES;   }
    else if (strcmp(response, NO) == 0)   { return User_response_NO;    }
    else                                  { return User_response_EXIT;  }
}

static bool Is_node_object(Node* node)
{
    assert(node != NULL);

    return node->left == NULL && node->right == NULL;
}

static void Insert_new_property(Node* node)
{
    assert(node != NULL);

    node->right = Insert_right_child(node);
    node->left = Create_left_сhild(node->elem, node);

    fprintf(stderr, 
            "Напишите, пожалуйста, вопрос(с маленькой буквы),"
            "по которму можно отличить %s и загаданный Вами объект\n", 
            node->elem);

    node->elem = Getting_info_from_user(node);

    Up_to_root(&node);
}

static Node* New_node(char* argument, Node* parent_node)
{
    assert(argument != NULL);

    Node* node = (Node*)calloc(1, sizeof(Node));
    node->parent = parent_node;

    if (argument != NULL) { node->elem = strdup(argument); }

    return node;
}

static Node* Create_left_сhild(char* old_answer, Node* parent_node)
{
    assert(old_answer != NULL);
    assert(parent_node != NULL);

    Node* node = New_node(NULL, parent_node);

    node->elem = old_answer;

    return node;
}

static Node* Insert_right_child(Node* parent_node)
{
    assert(parent_node != NULL);

    Node* node = New_node(NULL, parent_node);

    fprintf(stderr, "Напишите, пожалуйста, объект(с маленькой буквы) который Вы загадли\n");

    node->elem = Getting_info_from_user(node);

    return node;
}

static char* Getting_info_from_user(Node* node)
{
    assert(node != NULL);

    size_t size_new_condition = 0;
    long int size_line = getline(&node->elem, &size_new_condition, stdin);

    if(node->elem[size_line - 1] == '\n') { node->elem[size_line - 1] = '\0'; }

    return node->elem;
}

static void Up_to_root(Node** node_ptr)
{
    assert(node_ptr != NULL);

    while ((*node_ptr)->parent != NULL)
    {
        *node_ptr = (*node_ptr)->parent;
    }
}

static void Recursive_tree_entry(Node* node, FILE* file_stor)
{
    assert(file_stor != NULL);

    if (!node) { return; }

    fprintf(file_stor, "{");
    fprintf(file_stor, "%s", node->elem);
    if (node->left) { Recursive_tree_entry(node->left, file_stor); }
    if (node->right) { Recursive_tree_entry(node->right, file_stor); }

    fprintf(file_stor, "}");
}

static size_t Reads_file_size(FILE *file)
{
    assert(file != NULL);

    fseek(file, 0, SEEK_END);
    size_t size_file = (size_t)ftell(file);
    fseek(file, 0, SEEK_SET);

    return size_file;
}

static Node* Decod_tree()
{
    FILE* file_for_storing_tree = fopen(NAME_FILE_STOR, "r");

    size_t size_buffer = Reads_file_size(file_for_storing_tree);
    char* buffer = (char*)calloc(size_buffer + 1, sizeof(char));
    char* buffer_origin = buffer;

    fgetc(file_for_storing_tree);
    fread(buffer, sizeof(char), size_buffer, file_for_storing_tree);

    Node* root = Parsing_tree(&buffer, NULL);

    free(buffer_origin);
    fclose(file_for_storing_tree);

    return root;
}


static Node* Parsing_tree(char** buffer, Node* parent)
{
    assert(buffer != NULL);

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

static void Skip_parenthesis(char** buffer)
{
    (*buffer)++;
}

static void Parsing_line(char** buffer, char* line_buffer)
{
    assert(buffer != NULL);

    char* start = *buffer;
    char* end = *buffer;

    while((*end != '{') && (*end != '}') && (*end != '\0')) { end++; }

    size_t size_line = (size_t)(end - start);
    strncpy(line_buffer, start, size_line);
    line_buffer[size_line] = '\0';

    *buffer = end;
}

static Game_modes User_interaction()
{
    fprintf(stderr, "Выберите режим игры:\n1)Найти или добавить элемент\n2)Создать определение\n"
                    "3)Поиск общего между двумя элементамиn\n4)Выход\n\nНапишите цифру от 1 до 4\n");

    Game_modes game_mode = Game_modes_ELEM_SEARCH;
    scanf("%d", (int*)&game_mode);
    Clean_buffer();

    return game_mode;
}

static void Selecting_an_element(char** elem)
{
    assert(elem != NULL);

    fprintf(stderr, "Введите желаемый для работы элемент\n");
    size_t size_new_condition = 0;
    long int size_line = getline(elem, &size_new_condition, stdin);
    assert(size_line >= 0);

    if((*elem)[size_line - 1] == '\n') { (*elem)[size_line - 1] = '\0'; }
}

static Is_found Recursive_search_path_to_elem(Node* node, char* elem, Stack_t* Path_to_elem,
                                              To_build_definitions_elem Left_or_right)
{
    Info_about_unit_of_path node_info = {(char*)node, Left_or_right};
    StackPush(Path_to_elem, (StackElem_t)node_info);

    if ((!node->left) && (!node->right)) // залупа
    {
        if   (strcmp(elem, node->elem) == 0)                        { return FOUND; }
        else {StackElem_t trash = {}; StackPop(Path_to_elem, &trash); return NOT_FOUND; } 
    }

    Is_found Is_found_left  = Recursive_search_path_to_elem(node->left, elem, Path_to_elem, LEFT_YES);
    Is_found Is_found_right = Recursive_search_path_to_elem(node->right, elem, Path_to_elem, RIGHT_NO);

    if ((Is_found_left == FOUND) || (Is_found_right == FOUND)) { return FOUND; }

    StackElem_t trash = {}; StackPop(Path_to_elem, &trash);

    return NOT_FOUND;
}

static void Search_similarities_between_elems(size_t array_size1, Info_about_unit_of_path* array_properties1,
                                              size_t array_size2, Info_about_unit_of_path* array_properties2)
{
    assert(array_properties1 != NULL);
    assert(array_properties2 != NULL);

    fprintf(stderr, "\n%s ", ((Node*)array_properties1[array_size1].Adress)->elem);
    fprintf(stderr, "похож на ");
    fprintf(stderr, "%s", ((Node*)array_properties2[array_size2].Adress)->elem);
    fprintf(stderr, ", тем что они оба");

    switch(Compar_array_sizes(array_size1, array_size2))
    {
        case Array_sizes_FIRST_BIGGER: 
        {
            Conclusion_similarities_case_bigger(array_size1, array_properties1, array_size2, array_properties2);
        }  
        break;

        case Array_sizes_SECOND_BIGGER: 
        {
            Conclusion_similarities_case_bigger(array_size2, array_properties2, array_size1, array_properties1);
        }  
        break;

        case Array_sizes_EQUAL: 
        {
            Conclusion_similarities_case_equal(array_size1, array_properties1, array_properties2);
        }  
        break;
        default:
        {
            assert(0);
        } break;
    }
}

static void Conclusion_similarities_case_bigger(size_t array_size1, Info_about_unit_of_path* array_properties1,
                                              size_t array_size2, Info_about_unit_of_path* array_properties2)
{
    assert(array_properties1 != NULL);
    assert(array_properties2 != NULL);

    size_t current_index = 1;
    
    for ( ; current_index <= array_size2; current_index++)
    {
        if ((array_properties1[current_index - 1].Adress == array_properties2[current_index - 1].Adress)
            && (array_properties1[current_index].True_or_False == array_properties2[current_index].True_or_False))
        {
            const char* negation_or_not = array_properties1[current_index].True_or_False == LEFT_YES 
                                            ? " "
                                            : " не ";
            fputs(negation_or_not, stderr);  

            fprintf(stderr, "%s", ((Node*)array_properties1[current_index - 1].Adress)->elem);
        }
        else { break; }
    }

    fprintf(stderr, ", но ");
    fprintf(stderr, "%s", ((Node*)array_properties1[array_size1].Adress)->elem);

    size_t current_index1 = current_index;
    for ( ; current_index1 <= array_size1; current_index1++)
    {
        const char* negation_or_not = array_properties1[current_index].True_or_False == LEFT_YES 
                                            ? " "
                                            : " не ";
            fputs(negation_or_not, stderr);

        fprintf(stderr, "%s", ((Node*)array_properties1[current_index1 - 1].Adress)->elem);
    }

    fprintf(stderr, ", а ");
    fprintf(stderr, "%s", ((Node*)array_properties2[array_size2].Adress)->elem);
    size_t current_index2 = current_index;
    for ( ; current_index2 <= array_size2; current_index2++)
    {
        const char* negation_or_not = array_properties2[current_index].True_or_False == LEFT_YES 
                                            ? " "
                                            : " не ";
            fputs(negation_or_not, stderr);

        fprintf(stderr, "%s", ((Node*)array_properties2[current_index2 - 1].Adress)->elem);
    }

    fprintf(stderr, "\n");
}

static void Conclusion_similarities_case_equal(size_t array_size, Info_about_unit_of_path* array_properties1,
                                               Info_about_unit_of_path* array_properties2)
{
    assert(array_properties1 != NULL);
    assert(array_properties2 != NULL);

    size_t current_index = 1;
    
    for ( ; current_index <= array_size; current_index++)
    {
        if ((array_properties1[current_index - 1].Adress == array_properties2[current_index - 1].Adress)
            && (array_properties1[current_index].True_or_False == array_properties2[current_index].True_or_False))
        {
            const char* negation_or_not = array_properties1[current_index].True_or_False == LEFT_YES 
                                            ? " "
                                            : " не ";
            fputs(negation_or_not, stderr);

            fprintf(stderr, "%s", ((Node*)array_properties1[current_index - 1].Adress)->elem);
        }
        else { break; }
    }

    fprintf(stderr, ", но ");
    fprintf(stderr, "%s", ((Node*)array_properties1[array_size].Adress)->elem);

    size_t current_index1 = current_index;
    for ( ; current_index1 <= array_size; current_index1++)
    {
        const char* negation_or_not = array_properties1[current_index].True_or_False == LEFT_YES 
                                            ? " "
                                            : " не ";
        fputs(negation_or_not, stderr);
        
        fprintf(stderr, "%s", ((Node*)array_properties1[current_index1 - 1].Adress)->elem);
    }

    fprintf(stderr, ", а ");
    fprintf(stderr, "%s", ((Node*)array_properties2[array_size].Adress)->elem);
    size_t current_index2 = current_index;
    for ( ; current_index2 <= array_size; current_index2++)
    {
        const char* negation_or_not = array_properties2[current_index2].True_or_False == LEFT_YES 
                                            ? " "
                                            : " не ";
        fputs(negation_or_not, stderr);
        
        fprintf(stderr, "%s", ((Node*)array_properties2[current_index2 - 1].Adress)->elem);
    }

    fprintf(stderr, "\n");
}

static Array_sizes Compar_array_sizes(size_t array_size1, size_t array_size2)
{
    if (array_size1 > array_size2)      { return Array_sizes_FIRST_BIGGER;  }
    else if (array_size1 < array_size2) { return Array_sizes_SECOND_BIGGER; }
    else                                { return Array_sizes_EQUAL;         }
}

static void Definition_output(size_t array_size, Info_about_unit_of_path* array_properties)
{
    assert(array_properties != NULL);

    fprintf(stderr, "\n%s:", ((Node*)array_properties[array_size].Adress)->elem);
    for (size_t i = 1; i <= array_size; i++)
    {
        const char* negation_or_not = array_properties[i].True_or_False == LEFT_YES 
                                            ? " "
                                            : " не ";
        fputs(negation_or_not, stderr);

        fprintf(stderr, "%s", ((Node*)array_properties[i - 1].Adress)->elem);
    }
    
    fprintf(stderr, "\n");//fputc
}
















































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