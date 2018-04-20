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
    Board test_b = Board("[13][302][1003][30002][100003][3000002][121212]LastPlay:null");
    //Board test_b = Board("[13][302][1003][31002][100003][3000002][121212]LastPlay:(1,3,1,3)");
    test_b.printBoard();
    test_b.printMoves();
    return 0;
}