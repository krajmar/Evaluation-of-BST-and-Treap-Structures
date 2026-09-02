//
// Created by User3245 on 8/30/2026.
//

class Treap
{
private:
    struct Node {
        int key;
        int priority;
        Node* left;
        Node* right;

        Node(int key, int priority);
    };

    Node* root;

    //Basic functions
    Node* insert(Node* root, int key, int priority);
    bool search(Node* root, int key);
    Node* deleteNode(Node* root, int key);

    // Rotations
    Node* rotateLeft(Node* root);
    Node* rotateRight(Node* root);

    // Utility functions
    int height(Node* root);
    void destroyTree(Node* root);

    // Rotation counters
    long long leftRotationCount;
    long long rightRotationCount;

public:

    Treap();
    ~Treap();

    void insert(int key, int priority);
    bool search(int key);
    void deleteNode(int key);

    int height();
    long long getRotationCount();
};
