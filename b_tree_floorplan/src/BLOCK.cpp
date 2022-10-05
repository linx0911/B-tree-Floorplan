#include "BLOCK.h"

Block::Block(string name, double w, double h, bool r){
    this->name = name;
    this->w = w;
    this->h = h;
    this->size = w * h;
    this->r = r;
}

void Block::add_net(int net_index){
    this->net.push_back(net_index);
}

void Block::Rotate(){
    this->r = !this->r;
    swap(this->w, this->h);
}

Terminal::Terminal(){}

Terminal::Terminal(string name){
    this->name = name;
}

void Terminal::set_site(double x, double y){
    this->x = x;
    this->y = y;
}

void Terminal::add_net(int net_index){
    this->net.push_back(net_index);
}
