#include "tree/tree.h"
#include <time.h>

int main(void)
{
    srand(time(NULL)); // gonna generate random ints to test tree

    Tree best_tree = {};
    TreeInit(&best_tree);

    // for (int i = 0; i < 10; i++)
    // {
    //     int random_value = rand() % 1000;
    //     TreeInsert(&best_tree, random_value);
    //     TREE_GRAPH_DUMP(&best_tree, "graph_dump.html");
    // }

    // PrintTree(best_tree.root);

    ParseTree(&best_tree, "tree_file.txt");
    TREE_GRAPH_DUMP(&best_tree, "graph_dump.html");

    return 0;
}
