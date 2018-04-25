//Atropos game AI
//BU CS 440/640 2018 Spring
//Wei Jiang

//tree node class and minimax search algo

#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "board.h"
#include <iostream>
#include <math.h>
#include <float.h>

const int depth_limit = (int)5;
int tree_depth = 0;

class TreeNode
{
public:
  //current depth
  int c_depth; // if c_depth%2 == 0 then this node is a max node, else it is mini node

  //node score, NOT board score, and a pointer to that score provider
  double node_score;
  TreeNode *score_provider;

  //board config
  Board current_b;

  //pointer list of child
  vector<TreeNode *> children;

  //pointer to parent
  TreeNode *parent;

  //alpha beta value
  double alpha;
  double beta;

  //constructor
  TreeNode(string input, int d, TreeNode *p) : current_b(input), c_depth(d), node_score(0),
                                               alpha(-DBL_MAX), beta(DBL_MAX)
  {
    //update tree_depth
    tree_depth = c_depth > tree_depth? c_depth : tree_depth;

    if (c_depth % 2 == 0) //if this is a max node
      node_score = -DBL_MAX;
    else //if this is a mini node
      node_score = DBL_MAX;
    const vector< vector<int> > &possible_moves = current_b.getPossibleMoves();
    if (c_depth < depth_limit && current_b.alive_before_move)
    {
      int next_depth = c_depth + 1;
      for (size_t i = 0; i < possible_moves.size(); i++)
      {

        TreeNode *temp = new TreeNode(current_b.fakeInput(possible_moves[i]), next_depth, this);
        children.push_back(temp);
      }
    }

    parent = p;

    score_provider = nullptr;
  }

  void printNode()
  {
    cout << "current depth: " << c_depth << ", num_children: " << children.size()
         << ", current scores: " << node_score
         << ", alpha: " << alpha
         << ", beta: " << beta << endl;
  }

  //for non-leaf nodes
  void updateScore(double input_score, TreeNode *input_provider)
  {
    //cout << "c_depth: " << c_depth << " node_score: " << node_score << endl;
    if (c_depth % 2 == 0) //if this is a max node
    {
      if (input_score > node_score)
      {
        node_score = input_score;
        score_provider = input_provider;
      }
    }
    else //if this is a mini node
    {
      if (input_score < node_score)
      {
        node_score = input_score;
        score_provider = input_provider;
      }
    }
  }

  void updateScoreAB(double input_score, TreeNode *input_provider)
  {
    this->updateScore(input_score, input_provider);
    if (c_depth % 2 == 0) //if this is a max node
    {
      if (node_score > alpha)
      {
        alpha = node_score;
      }
    }
    else //if this is a mini node
    {
      if (node_score < beta)
      {
        beta = node_score;
      }
    }
  }

  vector<double> getAB()
  {
    vector<double> res = {alpha, beta};
    return res;
  }

  void setAB(vector<double> input_ab)
  {
    assert(input_ab.size() == 2);
    alpha = input_ab[0];
    beta = input_ab[1];
  }
};

//minimax search algo
//without alpha-beta prunning
//using dfs to traverse the tree

// int dfs_counter = 0;

// void dfs(TreeNode *root)
// {
//   dfs_counter++;
//   for (size_t i = 0; i < root->children.size(); i++)
//   {
//     //if(root->children[i]->current_b.alive_after_move)
//     dfs(root->children[i]);
//   }
//   if (root->c_depth == tree_depth || root->current_b.alive_after_move == false)
//   {
//     root->current_b.evaluateBoard();
//     root->node_score = root->current_b.getScore();
//   }
//   //pass value to parent
//   if (root->c_depth != 0)
//   {
//     root->parent->updateScore(root->node_score, root);
//   }
//   //root->printNode();
// }

//minimax search algo
//with alpha-beta prunning

int ab_counter = 0;

void ab_dfs(TreeNode *root) //, double pre_alpha, double pre_beta)
{
  ab_counter++;
  for (size_t i = 0; i < root->children.size(); i++)
  {
    //get parent alpha/beta values;
    root->children[i]->setAB(root->getAB());
    ab_dfs(root->children[i]);
    if (root->alpha > root->beta)
      break;
  }
  if (root->c_depth == tree_depth || root->children.size() == 0)
  {
    root->current_b.evaluateBoard();
    root->node_score = root->current_b.getScore();
  }
  //pass value to parent
  if (root->c_depth != 0)
  {
    root->parent->updateScoreAB(root->node_score, root);
  }
  //root->printNode();
}

void printScorePath(TreeNode *root)
{
  root->printNode();
  if (root->score_provider != nullptr)
    printScorePath(root->score_provider);
  else
  {
    root->current_b.printBoard();
  }
}

void printOutput(TreeNode *root)
{
  root->score_provider->current_b.printPreMove();
}

#endif