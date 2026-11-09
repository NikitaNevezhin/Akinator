#include "tree.h"

#include <ctype.h>

int ParseData(char* data_buff, FILE* tree_file)
{
    assert(data_buff);
    assert(tree_file);

    int curr_data_idx = 0;
    char curr_ch = fgetc(tree_file);

    if (curr_ch == '-') 
    {
        data_buff[curr_data_idx++] = (char)curr_ch;
        curr_ch = fgetc(tree_file);
    }

    while (isdigit(curr_ch))
    {
        data_buff[curr_data_idx] = curr_ch;
        curr_data_idx++;
        curr_ch = fgetc(tree_file);
    }

    data_buff[curr_data_idx] = '\0';

    ungetc(curr_ch, tree_file);

    return 0;
}

TreeNode* ParseSubtree(TreeNode* parent, FILE* tree_file, int* total_nodes)
{
    assert(tree_file);

    char first_symb = fgetc(tree_file);

    if (first_symb == '(')
    {   
        char second_symb = fgetc(tree_file);
        if (second_symb == ')') return NULL;  // empty subtree
        ungetc(second_symb, tree_file);
        
        TreeNode* new_node = (TreeNode*)calloc(1, sizeof(TreeNode));

        if (!new_node) 
        {
            PRINT_RED("calloc failed for new_node\n");
            return NULL;
        }

        (*total_nodes)++;

        char* data_buff = (char*)calloc(256, sizeof(char));
        ParseData(data_buff, tree_file);

        const char* const_data_buff = (const char*)data_buff;

        node_t data = atoi(const_data_buff);

        NodeInit(new_node, parent, data);

        TreeNode* left_subtree = ParseSubtree(new_node, tree_file, total_nodes);
        TreeNode* right_subtree = ParseSubtree(new_node, tree_file, total_nodes);

        if (left_subtree) LinkLeft(new_node, left_subtree);
        if (right_subtree) LinkRight(new_node, right_subtree);

        int close_symb = fgetc(tree_file);

        if (close_symb != ')') {
            PRINT_RED("Parsing error: expected ')', got '%c'\n", close_symb);
        }

        free(data_buff);
        return new_node;
    }

    if (first_symb != EOF) ungetc(first_symb, tree_file);

    return NULL;
}

int ParseTree(Tree* tree, const char* tree_filename)
{
    assert(tree);
    assert(tree_filename);

    FILE* tree_file = fopen(tree_filename, "r");

    if (tree_file == NULL)
    {
        PRINT_RED("fopen failed in %s\n", __func__);
        return 1;
    }

    tree->root = ParseSubtree(NULL, tree_file, &tree->total_nodes);

    fclose(tree_file);

    return 0;
}