//Atropos game AI
//BU CS 440/640 2018 Spring
//Wei Jiang and Yitian Lin

//game board class

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <assert.h>

using namespace std;

class Board
{
  public:
    //board config
    vector< vector<int> > b;
    //int b_size; //b_size = b.size() - 2; white space

    //previous move
    vector<int> pre_move;

    //future moves, for building search tree
    vector< vector<int> > possible_moves;

    //score for this board config, for smart searching algorithm
    double score;

    //constructor
    Board(string input)
    {
        //cout << "input string: " << input << endl;
        score = 0;
        //assume the string is from stdin
        //parse the string
        //sample: [13][302][1003][31002][100003][3000002][121212]LastPlay:(1,3,1,3)

        //load board config
        bool start_flag = false;
        vector<int> temp;
        for (size_t i = 0; i < input.size(); i++)
        {
            if (input[i] == '[')
            {
                start_flag = true;
                continue;
            }
            if (input[i] == ']')
            {
                start_flag = false;
                b.push_back(temp);
                temp.clear();
                continue;
            }
            if (start_flag == true)
            {
                try
                {
                    if (!(input[i] >= '0' && input[i] <= '3'))
                    {
                        throw "wrong input color!";
                    }
                }
                catch (const char *msg)
                {
                    cerr << msg << endl;
                    terminate();
                }
                int cc = input[i] - '0';
                temp.push_back(cc);
                continue;
            }
        }
        temp.clear();
        //b_size = b.size() - 2;

        //load previous move
        start_flag = false;
        vector<int> temp_pre;
        for (size_t i = 0; i < input.size(); i++)
        {
            if (input[i] == ':')
            {
                start_flag = true;
                continue;
            }
            if (input[i] == ',' || input[i] == '(' || input[i] == ')')
            {
                continue;
            }
            if (start_flag == true && input[i]>='0' && input[i]<='9')
            {
                int cc = input[i] - '0';
                temp_pre.push_back(cc);
                continue;
            }
        }
        if (temp_pre.size() == 4)
        {
            pre_move.push_back(temp_pre[0]); //color
            pre_move.push_back(b.size()-1 - temp_pre[1]); //row
            pre_move.push_back(temp_pre[2]); //col
        }
        temp_pre.clear();

        //build possible moves
        buildPossibleMoves();
    }

    //print board to stdout
    void printBoard()
    {
        cout << "board config: " << endl;
        for (size_t row = 0; row < b.size(); row++)
        {
            for (size_t col = 0; col < b[row].size(); col++)
            {
                cout << b[row][col] << " ";
            }
            cout << endl;
        }

        cout << "previous move: " <<endl;
        if(pre_move.size() == 0)
        {
            cout << "null" << endl;
        }
        else
        {
            for (size_t i = 0; i < pre_move.size(); i++)
            {
                cout<< pre_move[i] << " ";
            }
        }

        cout << endl;
    }

    //print all possible moves
    void printMoves()
    {
        cout << "all " << possible_moves.size() <<" possible moves : " << endl;
        for (size_t i = 0; i < possible_moves.size(); i++)
        {
            cout << "possible move " << i << " : ("
                << possible_moves[i][0] << ", " 
                << possible_moves[i][1] << ", "
                << possible_moves[i][2] <<")" << endl;

        }
        cout << endl;
    }

    //score evaluater
    void evaluateBoard()
    {
        //very very simple strategy
        score = possible_moves.size();
    }

    //score getter
    double getScore()
    {
        evaluateBoard();
        return score;
    }

    //build all possible moves
    void buildPossibleMoves()
    {
        int num_alive_moves = 0;
        int num_dead_moves = 0;
        vector< vector<int> > alive_moves;
        vector< vector<int> > dead_moves;

        //potential bug when pre_move is null
        vector<int> pre_move_loc;// = {pre_move[1], pre_move[2]};
        if(pre_move.size() != 0)
        {
            pre_move_loc.push_back(pre_move[1]);
            pre_move_loc.push_back(pre_move[2]);
        }
        vector< vector<int> > adj_empty = findAdjacentEmpty(pre_move_loc);

        if(pre_move.size() == 0 || adj_empty.size() == 0)//test all position that is empty
        {
            //cout<<"pre_move.size() == 0 || adj_empty.size() == 0"<<endl;
            vector< vector<int> > all_empty = findAllEmpty();
            for(size_t i = 0; i < all_empty.size(); i++)
            {
                for (int c_i = 1; c_i < 4; c_i++)
                {
                    //try all color from 1 - 3
                    vector<int> c_move = {c_i, all_empty[i][0], all_empty[i][1]};
                    if(moveChecker(c_move))
                    {
                        num_alive_moves++;
                        alive_moves.push_back(c_move);
                    }
                    else
                    {
                        num_dead_moves++;
                        dead_moves.push_back(c_move);
                    }
                }
            }        
        }
        else//test all positions that is adjacent to previous move
        {
            //cout<<adj_empty.size()<<endl;
            for(size_t i = 0; i < adj_empty.size(); i++)
            {
                for (int c_i = 1; c_i < 4; c_i++)
                {
                    //try all color from 1 - 3
                    vector<int> c_move = {c_i, adj_empty[i][0], adj_empty[i][1]};
                    if(moveChecker(c_move))
                    {
                        num_alive_moves++;
                        alive_moves.push_back(c_move);
                    }
                    else
                    {
                        num_dead_moves++;
                        dead_moves.push_back(c_move);
                    }
                }
            }
        }

        if (num_alive_moves == 0)
        {
            //set possible_moves to dead_moves
            possible_moves = dead_moves;
        }
        else
        {
            possible_moves = alive_moves;
        }
    }

    //check if this move is alive based on 3-color-triangle rule
    bool moveChecker(vector<int> move)
    {
        //move = (color, row, col);
        try
        {
            if ((move.size() != 3) || (!(move[0]>=1 && move[0]<=3)))
                throw "wrong move vector!";
        }
        catch (const char *msg)
        {
            cerr << msg << endl;
            terminate();
        }
        
        bool alive = true; //alive: no 3-color-triangle formed after this move

        //check is this move alive based on current board setting
        //1. get adjacent nodes for this move
        //2. for from start to the end, test whether this 2 node from adjacent and 1 node of the center forming a dead tri
        vector<int> loc = {move[1], move[2]};
        vector< vector<int> > adjs = findAdjacent(loc);

        for(size_t i = 0; i < adjs.size()-1; i++)
        {
            //get 3 temp nodes
            vector<int> temp1 = adjs[i];
            vector<int> temp2 = adjs[i+1];
            vector<int> temp3 = loc;
            vector< vector<int> > temp_node_locs;
            temp_node_locs.push_back(temp1);
            temp_node_locs.push_back(temp2);
            temp_node_locs.push_back(temp3);

            //get 3 colors
            vector<int> colors;
            colors.push_back(b[temp1[0]][temp1[1]]);
            colors.push_back(b[temp2[0]][temp2[1]]);
            colors.push_back(move[0]);

            alive = alive && !isFormingDeadTri(temp_node_locs, colors);
        }

        //corner case:
        //get 3 temp nodes
        vector<int> temp1 = adjs[adjs.size()-1];
        vector<int> temp2 = adjs[0];
        vector<int> temp3 = loc;
        vector< vector<int> > temp_node_locs;
        temp_node_locs.push_back(temp1);
        temp_node_locs.push_back(temp2);
        temp_node_locs.push_back(temp3);

        //get 3 colors
        vector<int> colors;
        colors.push_back(b[temp1[0]][temp1[1]]);
        colors.push_back(b[temp2[0]][temp2[1]]);
        colors.push_back(move[3]);

        alive = alive && !isFormingDeadTri(temp_node_locs, colors);

        return alive;
    }

    bool isFormingTri(vector< vector<int> > three_nodes)
    {
        assert(three_nodes.size() == 3);
        assert(three_nodes[0].size() == 2);
        //bool res;

        //is these 3 nodes forming a triangle?
        //must have 2 nodes in the same row and adjacent with each other
        //the third node must have one row offset
        //if the third node is above, the col is equal to the former node of lower two nodes
        //if the third node is below, the col is equal to the latter node of lower two nodes

        //corner case:
        //1. if 2 nodes share same row as b.size()-2,
        //then 
        //if the third node is above, the col is equal to the former node of lower two nodes
        //if the third node is below, the col is equal to the former node of lower two nodes

        //2. if 2 nodes share same row as b.size()-1,
        //then
        //the third node must be above, the col is equal to the former node of lower two nodes

        //implementation:
        int hi_row = -1;
        int lo_row = int(b.size());
        int num_nodes_hi_row = 0;
        int num_nodes_lo_row = 0;

        for(size_t i = 0; i < three_nodes.size(); i++)
        {
            if(three_nodes[i][0] < lo_row)
                lo_row = three_nodes[i][0];
            if(three_nodes[i][0] > hi_row)
                hi_row = three_nodes[i][0];
        }

        for(size_t i = 0; i < three_nodes.size(); i++)
        {
            if(three_nodes[i][0] == lo_row)
                num_nodes_lo_row++;
            if(three_nodes[i][0] == hi_row)
                num_nodes_hi_row++;
        }

        int dup_row = num_nodes_hi_row > num_nodes_lo_row ? hi_row : lo_row;

        if(hi_row - lo_row != 1)
            return false;

        assert(hi_row - lo_row == 1);

        int dup_col[2] = {0, 0}; 
        int dup_col_ind = 0;
        int sing_row;
        int sing_col;

        for(size_t i = 0; i < three_nodes.size(); i++)
        {
            if(three_nodes[i][0] == dup_row)
            {
                dup_col[dup_col_ind] = three_nodes[i][1];
                dup_col_ind++;
            }
            else
            {
                sing_row = three_nodes[i][0];
                sing_col = three_nodes[i][1];
            }
        }

        int n = sizeof(dup_col)/sizeof(dup_col[0]);
        sort(dup_col, dup_col+n);

        assert(dup_col_ind == 2);

        if(dup_col[1] - dup_col[0] != 1)
            return false;

        if(dup_row == b.size()-2)
        {
            //corner case:
            //1. if 2 nodes share same row as b.size()-2,
            //then 
            if(dup_row == hi_row) //if the third node is above, the col is equal to the former node of lower two nodes
            {
                if(sing_col == dup_col[0])
                    return true;
                else
                    return false;
            }
            else //if the third node is below, the col is equal to the former node of lower two nodes
            {
                if(sing_col == dup_col[0])
                    return true;
                else
                    return false;
            }
        }
        else if (dup_row == b.size()-1)
        {
            //2. if 2 nodes share same row as b.size()-1,
            //then
            if(sing_row == dup_row-1 && sing_col == dup_col[1])//the third node must be above, the col is equal to the latter node of lower two nodes
                return true;
            else
                return false;

        }
        else
        {
            //if the third node is above, the col is equal to the former node of lower two nodes
            if(sing_row < dup_row)
            {
                if(sing_col == dup_col[0])
                    return true;
                else
                    return false;
            }
            //if the third node is below, the col is equal to the latter node of lower two nodes
            else
            {
                if(sing_col == dup_col[1])
                    return true;
                else
                    return false;
            }
        }

        //return res;
    }

    bool isFormingDeadTri(vector< vector<int> > three_nodes, vector<int> in_colors)
    {
        assert(three_nodes.size() == 3);
        assert(three_nodes[0].size() == 2);

        bool res;
        if(!isFormingTri(three_nodes))
            res = false;
        else
        {
            int colors[3] = {in_colors[0],
                             in_colors[1],
                             in_colors[2]}; 
            int n = sizeof(colors)/sizeof(colors[0]);
            sort(colors, colors+n);
            if(colors[0] == 1 && colors[1] == 2 && colors[2] == 3)
                res = true;
            else
                res = false;
        }
        return res;
    }

    vector< vector<int> > findAdjacent(vector<int> center)
    {
        vector< vector<int> > res;
        if(center.size() == 0)//if the center is null
        {
            return res;
        }
        
        //test
        //cout<<center.size()<<endl;

        assert(center.size() == 2);
        if(center[0] == 0 || center[0] == b.size()-1 || center[1] == 0 || center[1] == b[center[0]].size()-1)
        {
            cerr << "can not find adjacent nodes for the border!" << endl;
            terminate();
        }
        else if(center[0] == b.size()-2) //if center node is on the b.size()-2 line
        {
            int i = center[0];
            int j = center[1];

            //north, west, east, north-west, south, south-west
            vector<int> north = {i-1, j};
            vector<int> south = {i+1, j};
            vector<int> west = {i, j-1};
            vector<int> east = {i, j+1};
            vector<int> north_west = {i-1, j-1};
            vector<int> south_west = {i+1, j-1};

            //push by the clockwise order, start from north
            res.push_back(north);
            res.push_back(east);
            res.push_back(south);
            res.push_back(south_west);
            res.push_back(west);
            res.push_back(north_west);
        }
        else//if center node is NOT on the b.size()-2 line
        {
            int i = center[0];
            int j = center[1];

            //north, south, west, east, north-west, south-east
            vector<int> north = {i-1, j};
            vector<int> south = {i+1, j};
            vector<int> west = {i, j-1};
            vector<int> east = {i, j+1};
            vector<int> north_west = {i-1, j-1};
            vector<int> south_east = {i+1, j+1};

            //push by the clockwise order, start from north
            res.push_back(north);
            res.push_back(east);
            res.push_back(south_east);
            res.push_back(south);
            res.push_back(west);
            res.push_back(north_west);
        }

        return res;
    }

    vector< vector<int> > findAdjacentEmpty(vector<int> center)
    {
        vector< vector<int> > res;
        if(center.size() == 0)//if the center is null
        {
            return res;
        }
        assert(center.size() == 2);

        vector< vector<int> > all = findAdjacent(center);

        for(size_t i = 0; i < all.size(); i++)
        {
            if(b[all[i][0]][all[i][1]] == 0)
                res.push_back(all[i]);
        }

        return res;
    }

    vector< vector<int> > findAllEmpty()
    {
        vector< vector<int> > res;

        for (size_t row = 0; row < b.size(); row++)
        {
            for (size_t col = 0; col < b[row].size(); col++)
            {
                if (b[row][col] == 0 /*this position is empty*/)
                {
                    vector<int> temp = {static_cast<int>(row), static_cast<int>(col)};
                    res.push_back(temp);
                }
            }
        }

        return res;
    }

    //possible moves getter
    const vector< vector<int> >& getPossibleMoves()
    {
        return possible_moves;
    }

    //for child nodes
    string fakeInput(vector<int> move)
    {
        assert(move.size() == 3);

        ostringstream os;

        for (size_t row = 0; row < b.size(); row++)
        {
            os << "[";
            for (size_t col = 0; col < b[row].size(); col++)
            {
                if(row == move[1] && col == move[2])
                {
                    os << move[0];
                }
                else
                    os << b[row][col];
            }
            os << "]";
        }
        
        os << "LastPlay:";
        int c = move[0];
        int h = b.size()-1 - move[1];
        int ld = move[2];
        int rd = b.size() - h - ld;
        os << "("<<c<<","<<h<<","<<ld<<","<<rd<<")";

        string s = os.str();
        return s;
    }

    //for final output
    //this output move is the pre_move
    //sample out: std::cout << "(1,2,2,2)";
    void printPreMove()
    {
        int c = pre_move[0];
        int h = b.size()-1 - pre_move[1];
        int ld = pre_move[2];
        int rd = b.size() - h - ld;
        cout << "(" << c << "," << h << "," << ld << "," << rd << ")" << endl;
    }
};

#endif

/*
sample board:
[1 3]
[3 0 2]
[1 0 0 3]
[3 0 0 0 2]
[1 0 0 0 0 3]
[3 0 0 0 0 0 2]
[1 2 1 2 1 2]

if the board is valid,
all the nodes except the last line have at most adjacent 6 nodes: north, south, west, east, north-west, south-east
say center node is [i][j]: [i-1][j], [i+1][j], [i][j-1], [i][j+1], [i-1][j-1], [i+1][j+1]

the nodes on the last line have at most 6 nodes: north, west, east, north-west, south, south-west
[i][j]: [i-1][j], [i][j-1], [i][j+1], [i-1][j-1], [j+1][i], [j+1][i-1]

NOTE: "the last line" means row = b.size()-2

what are the possible triangles if you have the valid adjacent nodes?
can have a clockwise check.


*/
