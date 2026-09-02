#include "treap.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace std;

Treap::Treap()
{
    root = NULL;
}

//Destructor, for deleting all nodes
void Treap::~Treap(){}

Treap::Node::Node(int key, int priority){
    this->key = key;
    this->priority = priority;
    left = NULL;
    right = NULL;
}

Treap::Node* Treap::rotateLeft(Node* root)
{
    Node* child = root->right;
    root->right = child->left;
    child->left = root;

    return child;
}

Treap::Node* Treap::rotateRight(Node* root)
{
    Node* child = root->left;
    root->left = child->right;
    child->right = root;

    return child;
}
