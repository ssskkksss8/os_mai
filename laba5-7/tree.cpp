#include "tree.hpp"

Tree::Tree(int root_id){
    id = root_id;
    size = 1;
    parent = nullptr;
}

Tree::Tree(int root_id, Tree * parent){
    id = root_id;
    size = 1;
    this->parent = parent;
}

Tree * Tree::find(int id){
    Tree * res;
    if (id == this->id){
        return this;
    }
    if (size > 1){
        for (int i = 0; i < children.size(); i++){
            res = children[i].find(id);
            if (res){
                return res;
            }
        }
    }
    return nullptr;
}

void Tree::push(int parent_id, int child_id){
    Tree * parent = find(parent_id);
    if (parent){    
        parent->children.push_back(Tree(child_id, parent));
        increaseSize(parent);
    }
}

int Tree::getSize(){
    return size;
}

int Tree::findRootChild(int id){
    Tree * child = find(id);
    if (child->parent == this){
        return child->id;
    }
    return child->upToRootChild(this);
}

int Tree::getID(){
    return id;
}

void Tree::printTree(){
    std::cout << id << ": { ";
    if (size > 1){
        for(int i = 0; i < children.size(); i++){
            std::cout << children[i].id << ' ';
        }
    }
    std::cout << '}' << std::endl;
    if (size > 1){
        for(int i = 0; i < children.size(); i++){
            children[i].printTree();
        }
    }
}

void Tree::traversal(std::vector<int> & res){
    res.push_back(id);
    if (size > 1){
        for (int i = 0; i < children.size(); i++){
            children[i].traversal(res);
        }
    }   
}

void Tree::increaseSize(Tree * t){
    t->size++;
    if (t->parent){
        increaseSize(t->parent);
    }
}

int Tree::upToRootChild(Tree * root){
    if (parent == root){
        return id;
    }
    return parent->upToRootChild(root);
}