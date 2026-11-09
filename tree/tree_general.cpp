#include "tree.h"

int NodeInit(TreeNode* node, TreeNode* parent, node_t data)
{
    assert(node);
    // parent is NULL -> node doesn't have a parent -> it is a root of the tree

    node->parent = parent;
    node->left = NULL;
    node->right = NULL;
    node->data = data;

    return 0;
}

int NodeDestroy(TreeNode* node)
{
    if (node == NULL) return 0;  // suppose it is destroyed

    node->data = POIZON;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    return 0;
}

int LinkLeft(TreeNode* parent, TreeNode* child)
{
    assert(parent);

    parent->left = child;

    return 0;
}

int LinkRight(TreeNode* parent, TreeNode* child)
{
    assert(parent);

    parent->right = child;

    return 0;
}

void PrintTree(TreeNode* root)
{
    assert(root);

    printf("(");
    printf("%d", root->data);

    if (root->left) PrintTree(root->left);
    else            printf("()");

    if (root->right) PrintTree(root->right);
    else            printf("()");

    printf(")");
}

int TreeInit(Tree* tree)
{
    assert(tree);

    tree->root = NULL;
    tree->total_nodes = 0;
 
    return 0;
}

TreeNode* InsertRecursive(TreeNode* curr_node, TreeNode* parent, node_t data, int* total_nodes)
{
    if (curr_node == NULL) // end of tree - no better place to put a new node in
    {
        TreeNode* new_node = (TreeNode*)calloc(1, sizeof(TreeNode));
        
        if (!new_node)
        {
            PRINT_RED("calloc failure in %s\n", __func__);
            return NULL;
        }

        NodeInit(new_node, parent, data);
        (*total_nodes)++;

        return new_node;
    }

    if (data < curr_node->data)  // then we must insert somewhere in left subtree
    {
        TreeNode* new_left = InsertRecursive(curr_node->left, curr_node, data, total_nodes);
        LinkLeft(curr_node, new_left);
    }

    else if (data > curr_node->data)  // then we must insert somewhere in right subtree
    {
        TreeNode* new_right = InsertRecursive(curr_node->right, curr_node, data, total_nodes);
        LinkRight(curr_node, new_right);
    }

    return curr_node;
}

int TreeInsert(Tree* tree, node_t data)
{
    assert(tree);

    if (tree->root == NULL)
    {
        tree->root = (TreeNode*)calloc(1, sizeof(TreeNode));
        NodeInit(tree->root, NULL, data);
        tree->total_nodes++;
        
        return 0;
    }

    InsertRecursive(tree->root, NULL, data, &(tree->total_nodes));

    return 0;
}