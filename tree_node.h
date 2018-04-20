//Atropos game AI
//BU CS 440/640 2018 Spring
//Wei Jiang and Yitian Lin

#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "board.h"
#include <iostream>
#include <math.h>
#include <float.h>

const int depth_limit = 3;

class TreeNode
{
  public:
    //current depth
    int c_depth; // if c_depth%2 == 0 then this node is a max node, else it is mini node

    //node score, NOT board score, and a pointer to that score provider
    double node_score;
    TreeNode* score_provider;

    //board config
    Board current_b;

    //pointer list of child
    vector<TreeNode*> children;

    //pointer to parent
    TreeNode* parent;

    //alpha beta value
    double alpha;
    double beta;

    //constructor
    TreeNode(string input, int d, TreeNode* p) : current_b(input), c_depth(d), node_score(0)
    {
      if( c_depth%2 == 0 )//if this is a max node
        node_score = -DBL_MAX;
      else //if this is a mini node
        node_score = DBL_MAX;
      const vector< vector<int> >& possible_moves = current_b.getPossibleMoves();
      if(c_depth < depth_limit)
      {
        int next_depth = c_depth + 1;
        for(size_t i = 0; i < possible_moves.size(); i++)
        {
          TreeNode* temp = new TreeNode(current_b.fakeInput(possible_moves[i]), next_depth, this);
          children.push_back(temp);
        }
      }

      parent = p;

      score_provider = nullptr;
      // cout << "current depth: " << c_depth << ", num_children: " << children.size() 
      //       << ", current scores: " << current_b.getScore()<< endl;
    }

    void printNode()
    {
      cout << "current depth: " << c_depth << ", num_children: " << children.size() 
            << ", current scores: " << node_score << endl;
    }

    //for non-leaf nodes
    void updateScore(double input_score, TreeNode* input_provider)
    {
      //cout << "c_depth: " << c_depth << " node_score: " << node_score << endl;
      if( c_depth%2 == 0 )//if it is a max node
      {
        if(input_score > node_score)
        {
          node_score = input_score;
          score_provider = input_provider;
        }
        // node_score = input_score > node_score? input_score : node_score;
        // if(input_score > node_score)
      }
      else 
      {
        if(input_score < node_score)
        {
          node_score = input_score;
          score_provider = input_provider;
        }
        //node_score = input_score < node_score? input_score : node_score;
      }
    }

    //search algo
    //minimax, two player
    //first without alpha-beta prunning
    //using dfs to traverse the tree
};


void dfs(TreeNode* root)
{
  for(size_t i = 0; i < root->children.size(); i++)
  {
    dfs(root->children[i]);
  }
  if ( root->c_depth == depth_limit )
  {
    root->node_score = root->current_b.getScore();
  }
  //pass value to parent
  if(root->c_depth != 0)
  {
    root->parent->updateScore(root->node_score, root);
  }

  root->printNode();
}

void printScorePath(TreeNode* root)
{
  root->printNode();
  if(root->score_provider != nullptr)
    printScorePath(root->score_provider);
  else
  {
    root->current_b.printBoard();
  }
}

void printOutput(TreeNode* root)
{
  root->score_provider->current_b.printPreMove();
}

#endif
