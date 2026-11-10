#include "tree.h"

int GenerateNodeDump(FILE* dot_file, const TreeNode* node, int node_idx, int dump_counter) // rootGenerateDotFile helper func
{
    assert(dot_file);
    assert(node);

    const char* bgcolor = "#98FF98 "; // valid node

    fprintf(
            dot_file, 
            "        d%d_node_%d [label=<\n"
            "            <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" COLOR=\"#354733\" BGCOLOR=\"%s\" CELLPADDING=\"7\">\n"
            "                <TR><TD COLSPAN=\"2\" ALIGN=\"CENTER\"><FONT POINT-SIZE=\"3\"><BR/></FONT>%s<FONT POINT-SIZE=\"3\"><BR/></FONT></TD></TR>\n"
            "                <TR>\n"
            "                    <TD PORT=\"l\" WIDTH=\"70\" ALIGN=\"CENTER\"><FONT POINT-SIZE=\"3\"><BR/></FONT>%p<FONT POINT-SIZE=\"3\"><BR/></FONT></TD>\n"
            "                    <TD PORT=\"r\" WIDTH=\"70\" ALIGN=\"CENTER\"><FONT POINT-SIZE=\"3\"><BR/></FONT>%p<FONT POINT-SIZE=\"3\"><BR/></FONT></TD>\n"
            "                </TR>\n"
            "            </TABLE>\n"
            "        >];\n\n",
            dump_counter, node_idx, bgcolor, node->data, node, node->left, node->right
    );
        
    return 0;
}

int GenerateTreeDump(FILE* dot_file, const TreeNode* root, int* node_idx, int dump_counter)
{
    assert(dot_file);
    assert(root);

    int curr_idx = *(node_idx);

    GenerateNodeDump(dot_file, root, curr_idx, dump_counter);

    (*node_idx)++; // for children

    if (root->left)
    {
        int left_idx = *node_idx; // left child index (saving it as it's gonna be lost in the next line)
        GenerateTreeDump(dot_file, root->left, node_idx, dump_counter);
        fprintf(dot_file, "        d%d_node_%d:s -> d%d_node_%d:n [label=\"YES\"];\n", dump_counter, curr_idx, dump_counter, left_idx);
        // fprintf(dot_file, "        d%d_node_%d -> d%d_node_%d [color=\"#4169E1\", style=\"dashed\", constraint=false];\n", 
        //         dump_counter, left_idx, dump_counter, curr_idx);
    }

    // else 
    // {
    //     fprintf(dot_file, "        d%d_l_nill_%d [shape=point, color=\"#322e2e\", width=0.1, height=0.1];\n", dump_counter, curr_idx);
    //     fprintf(dot_file, "        d%d_node_%d:s -> d%d_l_nill_%d [color=\"#322e2e\", style=dashed];\n", dump_counter, curr_idx, dump_counter, curr_idx);
    // }

    if (root->right) 
    {
        int right_idx = *node_idx; // right child index (saving it as it's gonna be lost in the next line)
        GenerateTreeDump(dot_file, root->right, node_idx, dump_counter);
        fprintf(dot_file, "        d%d_node_%d:s -> d%d_node_%d:n [label=\"NO\"];\n", dump_counter, curr_idx, dump_counter, right_idx);
        // fprintf(dot_file, "        d%d_node_%d -> d%d_node_%d [color=\"#4169E1\", style=\"dashed\", constraint=false];\n", 
        //         dump_counter, right_idx, dump_counter, curr_idx);
    }

    // else 
    // {
    //     fprintf(dot_file, "        d%d_r_nill_%d [shape=point, color=\"#322e2e\", width=0.1, height=0.1];\n", dump_counter, curr_idx);
    //     fprintf(dot_file, "        d%d_node_%d:s -> d%d_r_nill_%d [color=\"#322e2e\", style=dashed];\n", dump_counter, curr_idx, dump_counter, curr_idx);
    // }

    return 0;
}

int GenerateDotFile(TreeNode* root, const char* dot_filename, int dump_counter)
{
    assert(root);
    assert(dot_filename);

    FILE* dot_file = fopen(dot_filename, "w");

    if (dot_file == NULL)
    {
        PRINT_RED("%s failed to open %s - cannot generate .dot file for graph dump\n", __func__, dot_filename);
        return 1;
    }

    fprintf(dot_file, "digraph d%d_G {\n", dump_counter);
    fprintf(dot_file, "    rankdir=TB;\n");
    fprintf(dot_file, "    nodesep=0.6;\n\n");
    fprintf(dot_file, "    ranksep=0.7;\n");
    fprintf(dot_file, "    graph [splines=line];\n");
    fprintf(dot_file, "    ratio=\"auto\";\n");
    fprintf(dot_file, "    graph [dpi=150];\n");
    fprintf(dot_file, "    node [shape=plain, fontcolor=\"#212529\", fontname=\"Arial\", fontsize=11];\n");
    fprintf(dot_file, "    edge [color=\"#354733\", arrowsize=0.6]\n\n");

    int start_idx = 0;
    int* node_idx = &start_idx;
    GenerateTreeDump(dot_file, root, node_idx, dump_counter);

    fprintf(dot_file, "}\n");

    fclose(dot_file);

    return 0;
}

int TreeGenerateHtmlFile(Tree* tree, const char* html_filename, 
                         const char* file, int line, const char* func)
{
    assert(tree);
    assert(html_filename);


    static int dump_counter = 1;
    static bool is_first_dump = true;


    const char* dot_filename = "temp_dot_file.dot"; // this file's gonna be destroyed
    const char* svg_filename = "temp_svg_file.svg"; // this file's gonna be destroyed


    GenerateDotFile(tree->root, dot_filename, dump_counter);


    // creating svg
    char temp_term_command[TERM_CMD_MAX_SIZE] = {};


    sprintf(temp_term_command, "dot -Tsvg %s -o %s", dot_filename, svg_filename);


    const char* terminal_command = temp_term_command;


    system(terminal_command);


    FILE* html_file = NULL;


    if (is_first_dump)   html_file = fopen(html_filename, "w");


    else html_file = fopen(html_filename, "a");


    FILE* svg_file = fopen(svg_filename, "r");


    assert(html_file);
    assert(svg_file);
    
    if (is_first_dump)
    {
        fprintf(html_file,
            "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n"
            "    <meta charset=\"UTF-8\">\n"
            "    <title>Tree Dump History</title>\n"
            "    <style>\n"
            "        body { font-family: Arial, sans-serif; background: #f1f5f9; padding: 30px; color: #1e293b; }\n"
            "        \n"
            "        h2 { text-align: center; color: #0f172a; margin-bottom: 30px; font-size: 28px; }\n"
            "        \n"
            "        .dump-section {\n"
            "            max-width: 1200px;\n"
            "            margin: 0 auto 40px auto;\n"
            "            background: white;\n"
            "            padding: 24px;\n"
            "            border-radius: 8px;\n"
            "            border: 1px solid #e2e8f0;\n"
            "            box-shadow: 0 4px 6px -1px rgba(0,0,0,0.05), 0 2px 4px -1px rgba(0,0,0,0.03);\n"
            "            text-align: center;\n"
            "        }\n"
            "        \n"
            "        .dump-section h3 { margin-top: 0; margin-bottom: 15px; font-size: 18px; color: #1e40af; }\n"
            "        \n"
            "        .dash-line {\n"
            "            border: none;\n"
            "            border-top: 2px dashed #cbd5e1;\n"
            "            margin: 20px 0;\n"
            "            width: 100%%;\n"
            "        }\n"
            "        \n"
            "        .light-log {\n"
            "            display: inline-block;\n"
            "            font-family: 'Courier New', monospace;\n"
            "            font-size: 24px;\n"
            "            line-height: 1.6;\n"
            "            color: #334155;\n"
            "            text-align: left;\n"
            "            margin: 0 auto;\n"
            "            padding: 0;\n"
            "        }\n"
            "        \n"
            "        .graph-container { overflow-x: auto; padding: 15px; border: 1px solid #f1f5f9; background: #fafafa; border-radius: 6px; }\n"
            "        svg { max-width: 100%%; height: auto; display: block; margin: 0 auto; }\n"
            "    </style>\n</head>\n<body>\n"
            "    <h2>TREE DUMP HISTORY</h2>\n"
        );

        is_first_dump = false;
    }

    fprintf(html_file, "    <div class=\"dump-section\">\n");

    fprintf(html_file, 
        "        <pre class=\"light-log\">\n"
        "TreeDump #%d was called in %s:%d in function %s()\n"
        "Root address: %p\n"
        "Total nodes:  %d\n"
        "        </pre>\n\n",
        dump_counter, file, line, func, tree->root, tree->total_nodes
    );

    fprintf(html_file, 
        "        <hr class=\"dash-line\">\n"
        "        <div class=\"graph-container\">\n"
    );

    int ch = 0;
    while ((ch = fgetc(svg_file)) != EOF)  fputc(ch, html_file);


    fprintf(html_file, "        </div>\n");
    fprintf(html_file, "    </div>\n\n");   


    fclose(svg_file);
    fclose(html_file);


    remove(dot_filename);
    remove(svg_filename);

    dump_counter++;
    return 0;
}

int TreeGraphDump(Tree* tree, const char* html_filename, 
                  const char* file, int line, const char* func)
{
    assert(tree);

    TreeGenerateHtmlFile(tree, html_filename, file, line, func);

    return 0;
}
