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
    Node* insert(Node* root, int key, int priority);
    bool search(Node* root, int key);
    Node* deleteNode(Node* root, int key);

public:

    Treap();
    ~Treap();
};
