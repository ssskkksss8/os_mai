#pragma once 

#include <unistd.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <vector>


class Tree{
    int id;
    std::vector<Tree> children;
    int size;
    Tree * parent;
    void increaseSize(Tree * t);
    int upToRootChild(Tree * root);
public:
    Tree(int root_id);
    Tree(int root_id, Tree * parent);
    Tree * find(int id);
    int findRootChild(int id);
    void push(int parent_id, int child_id);
    int getSize();
    int getID();
    void printTree();
    void traversal(std::vector<int> &res);
};