#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <cmath>
#include <time.h>
#include<algorithm>
#include "BLOCK.h"
#include "Net.h"
#include "Presentation.h"
using namespace std;

/*********GLOBAL VARIABLE**********/
map<string, Block> Block_map;
map<string, Terminal> Ter_map;
map<int, Net> Net_map;
double W_fixed, H_fixed, Total_size, norm_size, norm_wl, Thermal_size, Thermal_wl;//, tmp_Max_W, tmp_Max_H, tmp_Max_WL, norm_size, norm_wl, best_cost;
int p_table[12] = {0, 1, 2, 3, 2, 3, 2, 3, 2, 2, 2, 3};
//Bs_tree_Node *best_tree;
/*********GLOBAL VARIABLE**********/

void input_parser(string block, string net, string pl, double wsr){
    ifstream fin1(block.c_str(), ios::in);
    string str;
    int block_num, ter_num, net_num, pin_num, net_count = 0;
    double total_block_size = 0.0;
    while(getline(fin1, str)){
        string token;
        istringstream delim(str);
        if(str == "") continue;
        getline(delim, token,' ');
        if(token == "NumHardRectilinearBlocks"){
            getline(delim, token,' ');
            getline(delim, token,' ');
            block_num = stoi(token);
        }
        else if(token == "NumTerminals"){
            getline(delim, token,' ');
            getline(delim, token,' ');
            ter_num = stoi(token);
        }
        else{
            string name = token;
            getline(delim, token,' ');
            if(token == "hardrectilinear"){
                int site_count = 0;
                double tmp_w, tmp_h;
                for(int i = 0; i < str.length(); ++i){
                    if(str[i] == '(') site_count++;
                    if(site_count == 3){
                        string wh[2];
                        bool check = 0;
                        for(int j = i+1; str[j] != ')'; ++j){
                            if(str[j] == ',') check = 1;
                            else wh[check] += str[j];
                        }
                        tmp_w = stoi(wh[0]);
                        tmp_h = stoi(wh[1]);
                        break;
                    }
                }
                Block tmp_block(name, tmp_w, tmp_h, 0);
                Block_map[name] = tmp_block;
                total_block_size += tmp_block.size;
            }
            else{
                Terminal tmp_t(name);
                Ter_map[name] = tmp_t;
            }
        }
    }
    ifstream fin2(pl.c_str(), ios::in);
    double x,y;
    map<string, Terminal>::iterator ter_iter;
    map<string, Block>::iterator block_iter;
    while(fin2>>str>>x>>y != NULL){
        Ter_map[str].set_site(x, y);
    }
    ifstream fin3(net.c_str(), ios::in);
    while(getline(fin3, str)){
        string token, flag;
        istringstream delim(str);
        getline(delim, token,' ');
        flag = token;
        getline(delim, token,' ');
        getline(delim, token,' ');
        if(flag == "NumNets") net_num = stoi(token);
        else if(flag == "NumPins") pin_num = stoi(token);
        else{
            int degree = stoi(token);
            vector<string> connection;
            for(int i = 0; i < degree; ++i){
                fin3>>str;
                block_iter = Block_map.find(str);
                if(block_iter != Block_map.end()) block_iter->second.add_net(net_count);
                ter_iter = Ter_map.find(str);
                if(ter_iter != Ter_map.end()) ter_iter->second.add_net(net_count);
                connection.push_back(str);
            }
            Net tmp_net(degree, connection);
            Net_map[net_count++] = tmp_net;
            fin3>>str;
        }
    }
    /********/
    cout<<total_block_size<<endl;
    Total_size = total_block_size;
    W_fixed = H_fixed = sqrt(total_block_size * (1+wsr));
    /********/
}

void clear_record(Bs_tree_Node *cur){
    if(cur == NULL) return;
    clear_record(cur->left);
    clear_record(cur->right);
    delete cur;
    return; 
}

Bs_tree_Node* record(Bs_tree_Node *cur){
    Bs_tree_Node *stored = new Bs_tree_Node(new Block(cur->block->name, cur->block->w, cur->block->h, cur->block->r));
    if(cur->left != NULL) stored->left = record(cur->left);
    if(cur->right != NULL) stored->right = record(cur->right);
    return stored;
}

void Back_DFS(Bs_tree_Node *cur){
    if(cur->left != NULL) Back_DFS(cur->left);
    if(cur->right != NULL) Back_DFS(cur->right);
    Block tmp_block = *cur->block;
    //cout<<tmp_block.name<<endl;
    Block_map[tmp_block.name] = tmp_block;
    return;
}

Bs_tree_Node* Build_DFS(Bs_tree_Node *best){
    Bs_tree_Node *cur = new Bs_tree_Node(&Block_map[best->block->name]);
    if(best->left != NULL){
        cur->left = Build_DFS(best->left);
        cur->left->parent = cur;
    }
    if(best->right != NULL){
        cur->right = Build_DFS(best->right);
        cur->right->parent = cur;
    }
    return cur;
}

Bs_tree_Node* Back_Best(Bs_tree_Node *best_head){
    Back_DFS(best_head);
    Bs_tree_Node* new_head;
    return Build_DFS(best_head);
}

bool cmp(const Block* a, const Block* b){return a->p < b->p;}

double HPWL(){
    double ans = 0.0;
    map<int, Net>::iterator net_iter;
    for(net_iter = Net_map.begin(); net_iter != Net_map.end(); ++net_iter){
        map<string, Block>::iterator block_iter;
        map<string, Terminal>::iterator ter_iter;
        int x_s = 100 * W_fixed, x_e = -1, y_s = 100 * H_fixed, y_e = -1;
        for(auto &name : net_iter->second.connection){
            block_iter = Block_map.find(name);
            ter_iter = Ter_map.find(name);
            if(block_iter != Block_map.end()){
                Block tmp_block = block_iter->second;
                int tmp_x = (int)(tmp_block.x + tmp_block.w/2.0), tmp_y = (int)(tmp_block.y + tmp_block.h/2.0);
                if(tmp_x < x_s) x_s = tmp_x;
                if(tmp_x > x_e) x_e = tmp_x;
                if(tmp_y < y_s) y_s = tmp_y;
                if(tmp_y > y_e) y_e = tmp_y;
            }
            if(ter_iter != Ter_map.end()){
                Terminal tmp_ter = ter_iter->second;
                if(tmp_ter.x < x_s) x_s = tmp_ter.x;
                if(tmp_ter.x > x_e) x_e = tmp_ter.x;
                if(tmp_ter.y < y_s) y_s = tmp_ter.y;
                if(tmp_ter.y > y_e) y_e = tmp_ter.y;
            }
        }
        ans += (x_e - x_s) + (y_e - y_s);
    }
    return ans;
}

double compute_cost_1(double size, double W, double H){
    double a = 0.2, b = 0.28, c = 0.32, d = 0.2;
    return a * size / norm_size + b * ((W > W_fixed) ? W/W_fixed : 1) + c * ((H > H_fixed) ? H/H_fixed : 1) + d * ((W>H)?W/H:H/W);
}

double compute_cost_2(double wl, double W, double H){
    double a = 0.6, b = 0.2, c = 0.2;
    return a * wl/norm_wl + b * ((W > W_fixed) ?100 : 1) + c * ((H > H_fixed) ? 100 : 1);
}

void output(string filename){
    fstream fp, freport;
    fp.open(filename, ios::out);
    double tmp_Max_WL = HPWL();
    fp<<"Wirelength "<<tmp_Max_WL<<endl;
    fp<<"Blocks"<<endl;
    map<string, Block>::iterator block_iter;
    for(block_iter = Block_map.begin(); block_iter != Block_map.end(); ++block_iter){
        fp<<block_iter->second.name<<" "<<block_iter->second.x<<" "<<block_iter->second.y<<" "<<block_iter->second.r<<endl;
    }
    freport.open(filename + ".rpt", ios::out);
    freport<<Block_map.size()<<" "<<H_fixed<<endl;
    for(block_iter = Block_map.begin(); block_iter != Block_map.end(); ++block_iter){
        freport<<block_iter->second.name.substr(2, block_iter->second.name.length()-2)<<" "<<block_iter->second.x<<" "<<block_iter->second.y<<" "<<block_iter->second.w<<" "<<block_iter->second.h<<" "<<0<<endl;
    }
}

void cooling(double &t, int &base){
    if(t >= 1.1) t *= 0.9;
    if(base > 4) base -= 4;
}

void heating(double &t, int &base, bool check){
    t = (check)?Thermal_size:Thermal_wl;
    cout<<"Thermal: "<<t<<endl;
    if(base < 12 && check) base += 4;
}

string find_neighbor(Bs_tree &TREE, int op, string b1, string b2){
    string ans;
    if(op == 0) TREE.rotate(b1);
    else if(op == 1) TREE.Swap(b1, b2);
    else if(op == 2) ans = TREE.del_insert(b1, b2);
    else if(op == 3) TREE.Swap_Subtree(b1);
    if(op == 2 && ans == "FAIL") return ans;
    return "SUC";
}

void reset2best(Bs_tree &TREE, Bs_tree_Node* &best_tree, double &H, double &W, double &wl, bool check){
    clear_record(TREE.head);
    TREE.head = Back_Best(best_tree);
    TREE.Build_floorplan();
    H = TREE.Height;
    W = TREE.Width;
}

void update_best(Bs_tree &TREE, Bs_tree_Node* &best_tree, double &H, double &W, double &wl, bool check){
    clear_record(best_tree);
    best_tree = record(TREE.head);
    H = TREE.Height;
    W = TREE.Width;
}

double SA(Bs_tree &TREE, bool state, double W, double H, vector<Block*> tmp_vec, double timeout){
    srand((unsigned)time(NULL));
    //Initialize tree & best tree
    Bs_tree_Node *best_tree;
    if(state) TREE = Bs_tree(tmp_vec, W, H);
    TREE.Build_floorplan();
    clear_record(best_tree);
    best_tree = record(TREE.head);

    //Initializa the parameters
    if(!state) norm_wl = HPWL();
    double best_cost = (state) ? compute_cost_1(TREE.Size, TREE.Width, TREE.Height) : compute_cost_2(norm_wl, TREE.Width, TREE.Height);
    double tmp_Max_H = TREE.Height, tmp_Max_W = TREE.Width, tmp_Max_WL = ((state)?0.0:norm_wl), T = ((state)?Thermal_size: Thermal_wl), acc_T = 0.9, total_time = 0.0, update_time = 0.0, size_cool = 1.2;
    norm_size = TREE.Size;
    int count = 0, tag = 1, base = ((state)?12:2), op_count[4] = {0, 0, 0, 0};
    clock_t time_s, time_e;

    string OP[4] = {"Rotate", "Swap" , "Del & Ins", "LR Switch"};
    if(state) cout<<endl<<"---------Set the boundary---------"<<endl;
    else cout<<endl<<"---------Set the HPWL---------"<<endl;
    //cout<<"W < "<<W<<" , H < "<<H<<"  Best cost = "<<best_cost<<endl;
    //cout<<"Initial W = "<<tmp_Max_W<<"  Initial H = "<<tmp_Max_H<<endl;
    while(++count){ 
        time_s = clock();
        if(state && tmp_Max_H < size_cool * H && tmp_Max_W < W * size_cool){ 
            cooling(T, base);
            size_cool *= 0.95;
        }
        int op = rand() % base, b1 = rand() % tmp_vec.size(), b2 = rand() % tmp_vec.size();
        string ans = find_neighbor(TREE, p_table[op], tmp_vec[b1]->name, tmp_vec[b2]->name);
        if(ans == "FAIL") continue;
        op_count[p_table[op]]++;
        TREE.Build_floorplan();
        double wl = (state)? 0.0: HPWL();
        double cost = (state)? compute_cost_1(TREE.Size, TREE.Width, TREE.Height): compute_cost_2(wl, TREE.Width, TREE.Height);
        if(cost > best_cost * T) reset2best(TREE, best_tree, tmp_Max_H, tmp_Max_W, tmp_Max_WL, state);
        else if(cost <= best_cost){
            //if(cost < best_cost) cout<<"TIME : "<<total_time<<" Count : "<<count<<"  Op : "<<OP[p_table[op]]<<"  W = "<<TREE.Width<<" H = "<<TREE.Height<<endl;
            best_cost = cost;
            update_best(TREE, best_tree, tmp_Max_H, tmp_Max_W, tmp_Max_WL, state);
            if(!state) tmp_Max_WL = wl;
            update_time = 0.0;
        }
        time_e = clock();
        total_time += (time_e-time_s)/(double)(CLOCKS_PER_SEC);
        update_time += (time_e-time_s)/(double)(CLOCKS_PER_SEC);
        if(total_time >= timeout) break; 
        if(update_time >= 15){
            update_time = 0.0;
            heating(T, base, state);
        } 
        if((state && tmp_Max_H <= H && tmp_Max_W <= W) || (!state && tmp_Max_WL < 0.7 * norm_wl)) break;
    }
    //SA
    //cout<<"BEST  TOTALTIME = "<<total_time<<"  W = "<<tmp_Max_W<<" H = "<<tmp_Max_H<<endl;
    clear_record(TREE.head);
    TREE.head = Back_Best(best_tree);
    TREE.Build_floorplan();
    //cout<<"HPWL = "<<HPWL()<<endl;
    if(state) total_time += SA(TREE, !state, W, H, tmp_vec, (600 - total_time > total_time)? total_time : 600 - total_time);
    return total_time;
}

int main(int argc, char **argv){
    input_parser(argv[1], argv[2], argv[3], stof(argv[5]));
    ////////
    Thermal_size = 1.5;
    Thermal_wl = 1.02;
    ////////
    map<string, Block>::iterator block_iter;
    vector<Block*> tmp_vec;
    for(block_iter = Block_map.begin(); block_iter != Block_map.end(); ++block_iter){
        Block* tmp_block = &block_iter->second;
        tmp_block->p = 0.7 * ((tmp_block->w * tmp_block->h) / (W_fixed * H_fixed)) + 0.3 * ((tmp_block->w + tmp_block->h)/(W_fixed + H_fixed));
        tmp_vec.push_back(tmp_block);
    } 
    sort(tmp_vec.rbegin(), tmp_vec.rend(), cmp);
    Bs_tree TREE;
    double total_time = 0.0, ratio, final_len;
    total_time = SA(TREE, true, W_fixed, H_fixed, tmp_vec, 600.0);
    cout<<endl<<"FINAL TIME = "<<total_time<<" W = "<<TREE.Width<<" H = "<<TREE.Height<<" HPWL = "<<HPWL()<<endl<<endl;
    final_len = ((TREE.Width > TREE.Height)?TREE.Width:TREE.Height);
    ratio = sqrt(pow(final_len, 2) / Total_size);
    cout<<"Ratio = "<<ratio<<" Final len = "<<final_len<<endl;
    output(string(argv[4]));
    return 0;
}

// Back to the Best Situation when the cost too large.
