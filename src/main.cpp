#include <iostream>
#include <cstdlib>
#include "bst/bst.h"

using namespace std;

int main()
{
    BST tree;

    tree.insert(10);
    tree.insert(40);
    tree.insert(70);
    tree.insert(50);
    tree.insert(45);
    tree.insert(60);
    tree.insert(90);
    tree.insert(15);
    tree.insert(20);
    tree.insert(5);
    tree.insert(100);
    tree.insert(75);
    tree.insert(80);
    tree.insert(65);

    cout<<"Height: "<<tree.height()<<endl;

    cout << "Tree: ";
    tree.inorder();
    cout << "\n";

    cout << "Search 40: "<< tree.search(40) << "\n";
    cout << "Search 70: "<< tree.search(70) << "\n";
    cout << "Search 100: "<< tree.search(100) << "\n";

    tree.deleteNode(50);
    tree.deleteNode(90);
    tree.inorder();
    return 0;
}