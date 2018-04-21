//Atropos game AI
//BU CS 440/640 2018 Spring
//Wei Jiang and Yitian Lin

//compile command: make
//run command: 

#include <iostream>
#include "tree_node.h"
#include "board.h"

using namespace std;

int main (int argc, char *argv[])
{
    TreeNode test_tn2 = TreeNode(argv[1], 0, nullptr);

    ab_dfs(&test_tn2);

    // cout << "root node score provider is:" << endl;
    // printScorePath(&test_tn2);
    // cout << endl;
    printOutput(&test_tn2);
    // cout << "ab_dfs times: " << ab_counter << endl;

    //test_tn.score_provider->printNode();
    //test_b.printMoves();

    return 0;
}