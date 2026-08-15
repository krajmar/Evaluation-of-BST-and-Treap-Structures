//
// Created by User3245 on 8/15/2026.
//
#include <iostream>

class BST
{
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        Node(int key);
    };

    Node* root;
    Node* insert(Node* root, int key);
    bool search(Node* root, int key);
    Node* deleteNode(Node* root, int key);
    Node* findMin(Node* root);

public:
    BST();
    ~BST();

    void insert(int key);
    bool search(int key);
    void deleteNode(int key);
    int height(Node* root);
};
