#ifndef TREE_H

#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../utils/PrintColor.h"

#define POIZON -666666

#define TERM_CMD_MAX_SIZE 256  // idk, let it be 256

#define TREE_GRAPH_DUMP(tree, html_filename) TreeGraphDump(tree, html_filename, __FILE__, __LINE__, __func__);

typedef int node_t;

struct TreeNode
{
    node_t data;

    TreeNode* left;
    TreeNode* right;

    TreeNode* parent;
};

struct Tree
{
    TreeNode* root;
    int total_nodes;
};

// -----------------
//   GENERAL TOOLS
// -----------------

int          NodeInit            (TreeNode* node, TreeNode* parent, node_t data);
 
int          NodeDestroy         (TreeNode* node);
 
int          LinkLeft            (TreeNode* parent, TreeNode* child);
 
int          LinkRight           (TreeNode* parent, TreeNode* child);
 
void         PrintTree           (TreeNode* root);
 
int          TreeInit            (Tree* tree);
 
TreeNode*    InsertRecursive     (TreeNode* curr_node, TreeNode* parent, node_t data, int* total_nodes);
 
int          TreeInsert          (Tree* tree, node_t data);

// ------------------
//  TREE-FILE PARSER
// ------------------

int          ParseData           (char* data_buff, FILE* tree_file);

TreeNode*    ParseSubtree        (TreeNode* parent, FILE* tree_file, int* total_nodes);

int          ParseTree           (Tree* tree, const char* tree_filename);


// ------------------
// GRAPH DUMP SECTION
// ------------------

int     GenerateNodeDump        (FILE* dot_file, const TreeNode* node, int node_idx, int dump_counter);

int     GenerateTreeDump        (FILE* dot_file, const TreeNode* root, int* node_idx, int dump_counter);

int     GenerateDotFile         (TreeNode* root, const char* dot_filename, int dump_counter);

int     TreeGenerateHtmlFile    (Tree* tree,  const char* html_filename, 
                                 const char* file, int line, const char* func);

int     TreeGraphDump           (Tree* tree, const char* html_filename, 
                                 const char* file, int line, const char* func);




#endif
