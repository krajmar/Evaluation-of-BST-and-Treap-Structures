#include "bst.h"
#include <iostream>
#include <cstdlib>

using namespace std;

BST::BST()
{
    root = NULL;
}

//Destructor, for deleting all nodes
BST::~BST(){}

BST::Node* Node(int key){
    this->key = key;
    left = NULL;
    right = NULL;
}

void BST::insert(int key)
{
    root = insert(root, key);
}

BST::Node* BST::insert(Node* root, int key)
{
    if (root == NULL)
    {
        return new Node(key);
    }

    if (key < root->key)
    {
        root->left = insert(root->left, key);
    }
    else if(key > root->key)
    {
        root->right = insert(root->right, key);
    }

    return root;
}

bool BST::search(Node* root, int key)
{
    if (root == NULL)
    {
        return false;
    }

    if (key < root->key)
    {
        return search(root->left, key);
    }

    if (key > root->key)
    {
        return search(root->right, key);
    }

    return true;
}

int main()
{
    return 0;
}