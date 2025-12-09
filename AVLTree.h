#ifndef AVLTREE_H
#define AVLTREE_H

#include "Constants.h"
#include "Aircraft.h"
#include <fstream>

struct AVLNode {
    Aircraft aircraft;
    int height;
    AVLNode* left;
    AVLNode* right;
    char status[20];

    AVLNode(const Aircraft& a, const char* s);
};

class AVLTree {
private:
    AVLNode* root;

    int height(AVLNode* node);
    int balanceFactor(AVLNode* node);
    void updateHeight(AVLNode* node);
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* balance(AVLNode* node);
    AVLNode* insert(AVLNode* node, const Aircraft& aircraft, const char* status);
    void inOrder(AVLNode* node, int& count) const;
    void saveInOrder(AVLNode* node, std::ofstream& file) const;
    void destroyTree(AVLNode* node);

public:
    AVLTree();
    ~AVLTree();
    void insert(const Aircraft& aircraft, const char* status);
    void printLog() const;
    void saveToFile(const char* filename) const;
    void loadFromFile(const char* filename);
    bool isEmpty() const;
};

#endif
