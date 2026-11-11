#ifndef AKINATOR_H

#define AKINATOR_H

#include <string.h>
#include <ctype.h>

#include "../tree/tree.h"
#include "../utils/StringTools.h"

#define ANSWER_SIZE 256

#define AKINATOR_DATA "akinator/breaking_bad_data.txt"
#define AKINATOR_DUMP "akinator_dump.html"


char*   GetUserAnswer           (void);

bool    IsYesAnswer             (const char* answer);

bool    IsNoAnswer              (const char* answer);

int     GetYesNoAnswer          (void);

void    AskQuestion             (TreeNode* curr_node);

int     AddNewObject            (TreeNode* curr_node);

int     UpdateAkinatorData      (Tree* akinator_tree);

int     PlayAkinatorRound       (Tree* akinator_tree);

int     StartAkinatorGame       (void);


#endif
