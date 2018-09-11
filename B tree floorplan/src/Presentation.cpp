#include "Presentation.h"

Bs_tree_Node::Bs_tree_Node(Block *block){
    this->block = block;
    this->left = NULL;
    this->right = NULL;
    this->parent = NULL;
    this->degree = 0;
}

Bs_tree::Bs_tree(vector<Block*> block_vec, double W, double H){
    this->null_tag =  new Bs_tree_Node(new Block("NULLLLL", -1, -1, 0));
    int tmp_w = 0, tmp_h = 0;   
    unsigned seed = (unsigned)time(NULL); 
    srand(seed);
    this->contour.resize(W+20);
    Bs_tree_Node *headNode = new Bs_tree_Node(block_vec[0]);
    headNode->block->x = 0;
    headNode->block->y = 0;
    int tmp_width = headNode->block->w;
    this->head = headNode;
    for(int i = headNode->block->x; i <headNode->block->x + headNode->block->w; ++i)
        this->contour[i] = headNode->block->h;
    this->Width = headNode->block->w;
    this->Height = headNode->block->h;
    Bs_tree_Node *tmp_head = this->head;
    for(int i = 1; i < block_vec.size(); ++i){
        Bs_tree_Node *newNode = new Bs_tree_Node(block_vec[i]);
        if(rand()%2 == 0) newNode->block->Rotate();
        if(tmp_w + newNode->block->w > W){
            int x = tmp_head->block->x, y = -1;
            for(int i = x; i < x + newNode->block->w; ++i)
                if(this->contour[i] > y) y = this->contour[i];
            newNode->block->x = x;
            newNode->block->y = y;
            tmp_head->right = newNode;
            newNode->parent = tmp_head;
            for(int i = x; i < x + newNode->block->w; ++i)
                this->contour[i] = newNode->block->y + newNode->block->h;
            tmp_w = newNode->block->w;
            tmp_head = tmp_head->right;
            if(this->Height < newNode->block->y + newNode->block->h) this->Height = newNode->block->y + newNode->block->h;
        }
        else{
            Bs_tree_Node* tmp = tmp_head;
            while(1){
                if(tmp->left != NULL) tmp = tmp->left;
                else{
                    int x = tmp->block->x + tmp->block->w, y = -1;
                    for(int i = x; i < x + newNode->block->w; ++i)
                        if(this->contour[i] > y) y = this->contour[i];
                    newNode->block->x = x;
                    newNode->block->y = y;
                    tmp->left = newNode;
                    newNode->parent = tmp;
                    for(int i = x; i < x + newNode->block->w; ++i)
                        contour[i] = newNode->block->y + newNode->block->h;
                    tmp_w = newNode->block->x + newNode->block->w;
                    if(this->Height < newNode->block->y + newNode->block->h) this->Height = newNode->block->y + newNode->block->h;
                    break;
                }
            }
        }
        if(tmp_w > this->Width) this->Width = tmp_w;
    }
}

Bs_tree_Node* Bs_tree::traversal(Bs_tree_Node* cur, string name){
    if(cur != NULL){
        if(cur->block->name == name) return cur;
        Bs_tree_Node* left = traversal(cur->left, name);
        if(left->block->name == name) return left;
        Bs_tree_Node* right = traversal(cur->right, name);
        if(right->block->name == name) return right;
    }
    return this->null_tag;
}

void Bs_tree::rotate(string name){
    Bs_tree_Node* target = this->Node_map[name];//traversal(this->head, name);
    target->block->Rotate();
}

void Bs_tree::Swap_Subtree(string name){
    Bs_tree_Node* node = this->Node_map[name];//traversal(this->head, name);
    while(1){
        if((node->left == NULL || node->right == NULL) && node != this->head) node = node->parent;
        if((node->left == NULL || node->right == NULL) && node == this->head) return;
        Bs_tree_Node* left = node->left;
        node->left = node->right;
        node->right = left;
        return;
    }
}

string Bs_tree::del_insert(string move_block, string insert_block){
    if(move_block == insert_block) {return "FAIL";}
    Bs_tree_Node* move = this->Node_map[move_block];//traversal(this->head, move_block);
    Bs_tree_Node* ins = this->Node_map[insert_block];//traversal(this->head, insert_block);
    if(move->parent == ins) {return "FAIL";}
    Bs_tree_Node* tmp = move;
    Bs_tree_Node* newNode = new Bs_tree_Node(move->block);
    Bs_tree_Node* tmp_child = NULL;
    if(rand()%2 == 0){
        if(ins->left != NULL) tmp_child = ins->left;
        ins->left = newNode;
        newNode->parent = ins;
        if(tmp_child != NULL){
            if(rand()%2 == 0) newNode->left = tmp_child;
            else newNode->right = tmp_child;
            tmp_child->parent = newNode;
        }
    }
    else{
        if(ins->right != NULL) tmp_child = ins->right;
        ins->right = newNode;
        newNode->parent = ins;
        if(tmp_child != NULL){
            if(rand()%2 == 0) newNode->left = tmp_child;
            else newNode->right = tmp_child;
            tmp_child->parent = newNode;
        }
    }
    while(1){
        if(tmp->left != NULL) tmp = tmp->left;
        else if(tmp->right != NULL) tmp = tmp->right;
        else{
            swap(tmp->block, move->block);
            if(tmp->parent->left != NULL && tmp->parent->left->block->name == tmp->block->name) tmp->parent->left = NULL;
            else tmp->parent->right = NULL;
            tmp = NULL;
            delete tmp;
            break;
        }
        
    }
    return "SUCC";
}

void Bs_tree::Swap(string name1, string name2){
    Bs_tree_Node* block1 = this->Node_map[name1];//traversal(this->head, name1);
    Bs_tree_Node* block2 = this->Node_map[name2];//traversal(this->head, name2);
    swap(block1->block, block2->block);
}

void Bs_tree::Build_floorplan(){
    this->contour.clear();
    this->contour.resize(1000);
    this->Width = 0;
    this->Height = 0;
    this->DFS(this->head, 0);
    this->Size = this->Width * this->Height;
}


void Bs_tree::DFS(Bs_tree_Node* cur, int x){
    if(cur == NULL) return;
    int max_h = -1;
    if(x + cur->block->w > this->contour.size()) this->contour.resize(this->contour.size()*2);
    for(int i = x; i < x + cur->block->w; ++i){
        if(max_h < this->contour[i]) max_h = this->contour[i];
    }
    cur->block->x = x;
    cur->block->y = max_h;
    for(int i = x; i < x + cur->block->w; ++i) this->contour[i] = cur->block->y + cur->block->h;

    if(cur->block->x + cur->block->w > this->Width) this->Width = cur->block->x + cur->block->w;
    if(cur->block->y + cur->block->h > this->Height) this->Height = cur->block->y + cur->block->h;
    if(cur->left != NULL) DFS(cur->left, cur->block->x + cur->block->w);
    if(cur->right != NULL) DFS(cur->right, cur->block->x);
    this->Node_map[cur->block->name] = cur;
    return;
}

