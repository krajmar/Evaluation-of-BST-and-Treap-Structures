#include <iostream>
#include <cstdlib>
#include "bst/bst.h"

using namespace std;

int main()
{
    BST tree;

    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    std::cout << "Tree: ";
    tree.inorder();
    std::cout << "\n";

    std::cout << "Search 40: "<< tree.search(40) << "\n";

    std::cout << "Search 100: "<< tree.search(100) << "\n";
    return 0;
}