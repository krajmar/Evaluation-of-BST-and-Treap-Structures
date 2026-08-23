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

void BST::insert(int key)
{
    root = insert(root, key);
}
BST::Node* BST::insert(Node* root, int key)
{
    if (root == NULL)
    {
        root = new Node(key);
    }
    else
    {
        insert(root->left, key);
    }
}

int main()
{
    return 0;
}