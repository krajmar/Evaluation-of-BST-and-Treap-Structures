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

Treap::Node* Treap::insert(Node* root, int key, int priority)
{
    if (root == NULL)
    {
        return new Node(key, priority);
    }

    if (key < root->key){

        root->left = insert(root->left, key, priority);

        if (root->left->priority > root->priority)
        {
            root = rotateRight(root);
        }
    }
    else if (key > root->key){

        root->right = insert(root->right, key, priority);

        if (root->right->priority > root->priority)
        {
            root = rotateLeft(root);
        }
    }

    return root;
}
