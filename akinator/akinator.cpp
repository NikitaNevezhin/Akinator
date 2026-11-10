#include "akinator.h"

char* GetUserAnswer()
{
    char* buff = (char*)calloc(ANSWER_SIZE, sizeof(char));
    if (buff == NULL)
    {
        PRINT_RED("Calloc failure in %s\n", __func__);
        return NULL;
    }

    char* fgets_res = fgets(buff, ANSWER_SIZE, stdin);

    if (fgets_res == NULL)
    {
        PRINT_RED("Failed to read info from user in %s\n", __func__);
        free(buff);
        return NULL;
    }

    int length = strlen(buff);
    if (length > 0 && buff[length - 1] == '\n')   buff[length - 1] = '\0';

    return buff;
}

bool IsYesAnswer(const char* answer)
{
    assert(answer);
    // printf("In IsYesAnswer I got %s as answer\n", answer);
    const char* yes_answers[] = {"Yes", "Y", "Yeah", "Absolutely", "Indeed", "Goddamn right", "Kinda"};

    for (size_t i = 0; i < sizeof(yes_answers) / sizeof(yes_answers[0]); i++)
    {
        if (IgnoreCaseStrcmp(answer, yes_answers[i]) == 0) return true;
    }
    // printf("I am about to return false\n");
    return false;
}

bool IsNoAnswer(const char* answer)
{
    assert(answer);

    const char* no_answers[] = {"No", "Nah", "Nope", "Nothing", "Absolutely not", "Nothing of the kind"};

    for (size_t i = 0; i < sizeof(no_answers) / sizeof(no_answers[0]); i++)
    {
        if (IgnoreCaseStrcmp(answer, no_answers[i]) == 0) return true;
    }

    return false;
}

int GetYesNoAnswer() // 1 is yes, 0 is no, -1 is wrong input
{
    char* answer = GetUserAnswer();

    if (IsYesAnswer(answer)) return 1;

    else if (IsNoAnswer(answer)) return 0;

    return -1;
}

void AskQuestion(TreeNode* curr_node)
{
    if (curr_node->left || curr_node->right) // has kids -> it is a question
        PRINT_BLUE("object %s?\n", curr_node->data);

    else
        PRINT_BLUE("My guess: the object is %s?\n", curr_node->data);

    return;
}

int AddNewObject(TreeNode* curr_node)
{
    assert(curr_node);

    PRINT_BLUE("That is not %s?! Who was it then?\n", curr_node->data);
    char* new_object = GetUserAnswer();

    if (new_object == NULL)
    {
        PRINT_RED("User interaction failed in %s\n", __func__);
        free(new_object);

        return 1;
    }

    PRINT_BLUE("How do you distinguish them? Unlike %s, %s...\n", curr_node->data, new_object);
    char* new_characteristic = GetUserAnswer();

    TreeNode* left = (TreeNode*)calloc(1, sizeof(TreeNode));
    NodeInit(left, curr_node, new_object);
    LinkLeft(curr_node, left);

    TreeNode* right = (TreeNode*)calloc(1, sizeof(TreeNode));
    NodeInit(right, curr_node, curr_node->data);  // answer no to new characteristic leads to the old object
    LinkRight(curr_node, right);

    curr_node->data = new_characteristic;

    PRINT_BLUE("You outsmarted me, Mr Smartass, my congratulations!\n");

    return 0;
}

int UpdateAkinatorData(Tree* akinator_tree)
{
    assert(akinator_tree);

    FILE* akinator_data = fopen(AKINATOR_DATA, "w");
    if (akinator_data == NULL)
    {
        PRINT_RED("Failed to open %s in %s\n", AKINATOR_DATA, __func__);
        return 1;
    }

    FprintTree(akinator_data, akinator_tree->root);

    fclose(akinator_data);

    return 0;
}

int PlayAkinatorRound(Tree* akinator_tree)
{
    TreeNode* curr_node = akinator_tree->root;

    #ifdef GRAPH_DUMP_EACH_ROUND
    TREE_GRAPH_DUMP(akinator_tree, AKINATOR_DUMP);
    #endif

    if (curr_node == NULL)
    {
        PRINT_RED("akinator_tree root is NULL. Probably, akinator_data is empty\n");
        return 1;
    }

    while (curr_node != NULL)
    {
        AskQuestion(curr_node);

        int answer = GetYesNoAnswer();

        if (answer == -1)
        {
            PRINT_YELLOW(
                "Dude, just write \"yes\" or \"no\", my developer is so fucking tired "
                "of checking if you send me your shit instead of a normal answer\n"
                "Lets try again\n"
            );

            continue;
        }

        else if (answer == 1)  // user entered "yes"
        {
            if (curr_node->left)
            {
                curr_node = curr_node->left;
                continue;
            }

            PRINT_BLUE("I knew it was %s from the very beginning! I won again, old man.\n", curr_node->data);
            return 0;
        }

        else if (answer == 0)  // user entered "no"
        {
            if (curr_node->right)
            {
                curr_node = curr_node->right;
                continue;
            }

            if(AddNewObject(curr_node) == 0) UpdateAkinatorData(akinator_tree);

            return 0;
        }
    }

    #ifdef GRAPH_DUMP_EACH_ROUND
    TREE_GRAPH_DUMP(akinator_tree, AKINATOR_DUMP);
    #endif
    
    return 0;
}

int StartAkinatorGame()
{
    Tree akinator_tree = {};
    TreeInit(&akinator_tree);
    ParseTree(&akinator_tree, "akinator/akinator_data.txt");

    PRINT_BLUE("Welcome to the ultimate game of Akinator by NN ltd.\n");
    PRINT_BLUE("Our NikitAI model is now capable of guessing any object a person can think of\n");
    PRINT_BLUE("Specifically developed algorithmes analyze user's answers and can accurately name the guessed object\n");
    PRINT_BLUE("Wanna check out?\n");

    int round_counter = 1;
    int answer = GetYesNoAnswer();

    while (answer == -1)
    {
        PRINT_YELLOW(
                    "Dude, just write \"yes\" or \"no\", my developer is so fucking tired "
                    "of checking if you send me your shit instead of a normal answer\n"
                    "Lets try again\n"
        );

        answer = GetYesNoAnswer();
    }

    while (answer == 1)
    {
        PRINT_GREEN("\n---------ROUND #%d-----------\n", round_counter);

        if (PlayAkinatorRound(&akinator_tree) != 0)
        {
            PRINT_RED("Akinator game failed in round %d\n", round_counter);
            return 1;
        }

        round_counter++;

        PRINT_BLUE("\nWanna play one more round?\n");

        answer = GetYesNoAnswer();
        while (answer == -1)
        {
            PRINT_BLUE(
                    "Dude, just write \"yes\" or \"no\", my developer is so fucking tired "
                    "of checking if you send me your shit instead of a normal answer\n"
                    "Lets try again\n"
            );

        answer = GetYesNoAnswer();

        }
    }

    PRINT_BLUE("Thank you for playing!\n");

    TreeDestroy(&akinator_tree);

    return 0;
}