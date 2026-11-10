#ifndef AKINATOR_H

#define AKINATOR_H

#include <string.h>
#include <ctype.h>

#include "../tree/tree.h"
#include "../utils/StringTools.h"

#define ANSWER_SIZE 256
#define AKINATOR_DATA "akinator/akinator_data.txt"

char*   GetUserAnswer           (void);

void    AskQuestion             (TreeNode* curr_node);

bool    IsYesAnswer             (const char* answer);

bool    IsNoAnswer              (const char* answer);

int     GetYesNoAnswer          (void);

int     AddNewObject            (TreeNode* curr_node);

int     UpdateAkinatorData      (Tree* akinator_tree);

int     StartAkinator           (void);

#endif
