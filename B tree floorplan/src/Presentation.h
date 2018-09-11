#ifndef Presentation
#define Presentation

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <time.h>
#include<algorithm>
#include <unordered_map>
#include <vector>
#include "BLOCK.h"
using namespace std;



class Bs_tree_Node{
public:
    Bs_tree_Node* left, *right, *parent;
    Block *block;
    int degree;
    Bs_tree_Node(){};
    Bs_tree_Node(Block*);
};

class Bs_tree{
public:
    Bs_tree_Node *head, *null_tag;
    vector<int> contour;
    unordered_map<string, Bs_tree_Node*> Node_map;
    Bs_tree(){};
    Bs_tree(vector<Block*>);
    Bs_tree(vector<Block*>, double, double);
    int Tree_height, Width, Height, Size;
    int initial_traversal(Bs_tree_Node*);
    Bs_tree_Node* traversal(Bs_tree_Node*,string);
    void rotate(string);
    string del_insert(string, string);
    void Swap(string, string);
    void Swap_Subtree(string);
    void Build_floorplan();
    void initial_DFS(Bs_tree_Node*, Bs_tree_Node*, int);
    void DFS(Bs_tree_Node*, int);
    void Return_best(Bs_tree_Node*, Bs_tree_Node*);
};

#endif