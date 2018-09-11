#ifndef BLOCK
#define BLOCK

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Block{
public:
    Block(){};
    Block(string,double,double, bool);
    void add_net(int);
    void Rotate();
    string name;
    double w, h, size, p;
    double x, y;
    bool r;
    vector<int> net;
};

class Terminal{
public:
    Terminal();
    Terminal(string);
    void set_site(double,double);
    void add_net(int);
    string name;
    double x, y;
    vector<int> net;
};

#endif