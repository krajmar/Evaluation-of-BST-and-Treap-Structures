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

void BST::inorder(Node* root)
{
    if (root == NULL){
        return;
    }

    inorder(root->left);
    std::cout << root->key << " ";
    inorder(root->right);
}

void BST::inorder(){
    inorder(root);
}

void BST::insert(int key){
    root = insert(root, key);
}

BST::Node* BST::insert(Node* root, int key){
    if (root == NULL){
        return new Node(key);
    }

    if (key < root->key){
        root->left = insert(root->left, key);
    }
    else if(key > root->key){
        root->right = insert(root->right, key);
    }

    return root;
}

bool BST::search(Node* root, int key){
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

BST::Node* BST::findMin(Node* root)
{
    if (root == NULL){
        return NULL;
    }
    if (root->left != NULL){
        return findMin(root->left);
    }
    return root;
}

BST::Node* BST::deleteNode(Node* root, int key)
{
    if (root == NULL){
        return root;
    }
    if (key < root->key){
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->key){
        root->right = deleteNode(root->right, key);
    }
    else{
        // Node has no left child
        if (root->left == NULL){
            Node* temp = root->right;
            delete root;
            return temp;
        }

        // Node has no right child
        if (root->right == NULL){
            Node* temp = root->left;
            delete root;
            return temp;
        }

        // Node has two children
        Node* temp = findMin(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }

    return root;
}

int main()
{
    return 0;
}