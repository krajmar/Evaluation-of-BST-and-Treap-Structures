#include "treap.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace std;

Treap::Treap()
{
    root = NULL;
    leftRotationCount = 0;
    rightRotationCount = 0;
}

//Destructor, for deleting all nodes
Treap::~Treap(){}

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

    leftRotationCount++;

    return child;
}

Treap::Node* Treap::rotateRight(Node* root)
{
    Node* child = root->left;
    root->left = child->right;
    child->right = root;

    rightRotationCount++;

    return child;
}

long long Treap::getRotationCount()
{
    long long total = leftRotationCount + rightRotationCount;
    return total;
}

Treap::Node* Treap::insert(Node* root, int key, int priority)
{
    if (root == NULL)
    {
        return new Node(key, priority);
    }

    if (key < root->key){

        root->left = insert(root->left, key, priority);
        if (root->left->priority > root->priority){
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

void Treap::insert(int key, int priority)
{
    root = insert(root, key, priority);
}

bool Treap::search(Node* root, int key){

    if (root == NULL){
        return false;
    }

    if (key < root->key){
        return search(root->left, key);
    }

    if (key > root->key){
        return search(root->right, key);
    }

    return true;
}

bool Treap::search(int key)
{
    return search(root, key);
}

Treap::Node* Treap::deleteNode(Node* root, int key)
{
    if (root == NULL)
    {
        return NULL;
    }
    if (key < root->key)
    {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = deleteNode(root->right, key);
    }
    else
    {
        //No children
        if (root->left == NULL && root->right == NULL)
        {
            delete root;
            return NULL;
        }

        //Only right child
        if (root->left == NULL)
        {
            root = rotateLeft(root);
            root->left = deleteNode(root->left, key);
        }

        //Only left child
        else if (root->right == NULL)
        {
            root = rotateRight(root);
            root->right = deleteNode(root->right, key);
        }

        //Two children
        else if (root->left->priority > root->right->priority)
        {
            root = rotateRight(root);
            root->right = deleteNode(root->right, key);
        }
        else
        {
            root = rotateLeft(root);
            root->left = deleteNode(root->left, key);
        }
    }
    return root;
}

void Treap::deleteNode(int key)
{
    root = deleteNode(root, key);
}

int Treap::height(Node* root)
{
    if (root == NULL)
    {
        return -1;
    }

    return 1 + max(height(root->left), height(root->right));
}
int Treap::height()
{
    return height(root);
}