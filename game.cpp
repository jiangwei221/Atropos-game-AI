//Atropos game AI
//BU CS 440/640 2018 Spring
//Wei Jiang and Yitian Lin

//compile command: make
//run command: 

#include <iostream>
#include "tree_node.h"
#include "board.h"

using namespace std;

int main (int argc, char **argv)
{
    //board class test
    //Board test_b = Board("[13][302][1003][30002][100003][3000002][121212]LastPlay:null");
    Board test_b = Board("[13][302][1003][31002][100003][3000002][121212]LastPlay:(1,3,1,3)");
    test_b.printBoard();
    test_b.printMoves();

    for(size_t i = 0; i < test_b.possible_moves.size(); i++)
    {
        cout<<test_b.fakeInput(test_b.possible_moves[i])<<endl;
    } 
    cout << endl;

    //TreeNode test_tn = TreeNode("[13][302][1003][30002][100003][3000002][121212]LastPlay:null", 0, nullptr);
    TreeNode test_tn = TreeNode("[13][302][1003][31002][100003][3000002][121212]LastPlay:(1,3,1,3)", 0, nullptr);
    cout << endl;     

    dfs(&test_tn);

    cout << "root node score provider is:" << endl;
    printScorePath(&test_tn);
    cout << endl;
    printOutput(&test_tn);
    cout << "dfs times: " << dfs_counter << endl;

    TreeNode test_tn2 = TreeNode("[13][302][1003][31002][100003][3000002][121212]LastPlay:(1,3,1,3)", 0, nullptr);
    //TreeNode test_tn2 = TreeNode("[13][302][1003][30002][100003][3000002][121212]LastPlay:null", 0, nullptr);

    cout << endl;     

    ab_dfs(&test_tn2);

    cout << "root node score provider is:" << endl;
    printScorePath(&test_tn2);
    cout << endl;
    printOutput(&test_tn2);
    cout << "ab_dfs times: " << ab_counter << endl;

    //test_tn.score_provider->printNode();
    //test_b.printMoves();

    return 0;
}