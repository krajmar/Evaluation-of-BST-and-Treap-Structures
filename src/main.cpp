#include <iostream>
#include <cstdlib>
#include "bst/bst.h"
#include "treap/treap.h"

using namespace std;

int main()
{
    Treap treap;

    std::cout << "========================================\n";
    std::cout << "           TREAP TEST PROGRAM\n";
    std::cout << "========================================\n";


    // ========================================
    // 1. INSERTION TEST
    // ========================================

    std::cout << "\n--- INSERTION TEST ---\n";

    treap.insert(50, 80);
    treap.insert(30, 60);
    treap.insert(70, 70);
    treap.insert(20, 90);
    treap.insert(40, 50);
    treap.insert(60, 65);
    treap.insert(80, 55);
    treap.insert(10, 85);
    treap.insert(25, 75);
    treap.insert(35, 45);
    treap.insert(45, 40);
    treap.insert(55, 30);
    treap.insert(65, 68);
    treap.insert(75, 35);
    treap.insert(90, 95);
    treap.insert(5, 20);
    treap.insert(15, 58);
    treap.insert(22, 72);
    treap.insert(33, 48);
    treap.insert(85, 88);

    std::cout << "20 nodes inserted.\n";


    // ========================================
    // 2. SEARCH TEST
    // ========================================

    std::cout << "\n--- SEARCH TEST ---\n";

    int searchKeys[] = {
        5, 10, 15, 20, 25,
        30, 35, 40, 45, 50,
        55, 60, 65, 70, 75,
        80, 85, 90,
        100, 999
    };

    for (int key : searchKeys)
    {
        std::cout << "Search " << key << ": ";

        if (treap.search(key))
        {
            std::cout << "FOUND";
        }
        else
        {
            std::cout << "NOT FOUND";
        }

        std::cout << std::endl;
    }


    // ========================================
    // 3. TREE INFORMATION
    // ========================================

    std::cout << "\n--- TREE INFORMATION ---\n";

    std::cout << "Height: "
              << treap.height()
              << std::endl;

    std::cout << "Rotations: "
              << treap.getRotationCount()
              << std::endl;


    // ========================================
    // 4. DELETE LEAF
    // ========================================

    std::cout << "\n--- DELETE TEST 1: LEAF ---\n";

    std::cout << "Deleting 5...\n";

    treap.deleteNode(5);

    std::cout << "Search 5: "
              << (treap.search(5) ? "FOUND" : "NOT FOUND")
              << std::endl;

    std::cout << "Height: "
              << treap.height()
              << std::endl;

    std::cout << "Rotations: "
              << treap.getRotationCount()
              << std::endl;


    // ========================================
    // 5. DELETE NODE WITH ONE CHILD
    // ========================================

    std::cout << "\n--- DELETE TEST 2: ONE CHILD ---\n";

    std::cout << "Deleting 85...\n";

    treap.deleteNode(85);

    std::cout << "Search 85: "
              << (treap.search(85) ? "FOUND" : "NOT FOUND")
              << std::endl;

    std::cout << "Height: "
              << treap.height()
              << std::endl;

    std::cout << "Rotations: "
              << treap.getRotationCount()
              << std::endl;


    // ========================================
    // 6. DELETE NODE WITH TWO CHILDREN
    // ========================================

    std::cout << "\n--- DELETE TEST 3: TWO CHILDREN ---\n";

    std::cout << "Deleting 30...\n";

    treap.deleteNode(30);

    std::cout << "Search 30: "
              << (treap.search(30) ? "FOUND" : "NOT FOUND")
              << std::endl;

    std::cout << "Height: "
              << treap.height()
              << std::endl;

    std::cout << "Rotations: "
              << treap.getRotationCount()
              << std::endl;


    // ========================================
    // 7. DELETE ROOT
    // ========================================

    std::cout << "\n--- DELETE TEST 4: ROOT ---\n";

    std::cout << "Deleting 90...\n";

    treap.deleteNode(90);

    std::cout << "Search 90: "
              << (treap.search(90) ? "FOUND" : "NOT FOUND")
              << std::endl;

    std::cout << "Height: "
              << treap.height()
              << std::endl;

    std::cout << "Rotations: "
              << treap.getRotationCount()
              << std::endl;


    // ========================================
    // 8. DELETE NON-EXISTING KEY
    // ========================================

    std::cout << "\n--- DELETE TEST 5: NON-EXISTING KEY ---\n";

    std::cout << "Deleting 1000...\n";

    treap.deleteNode(1000);

    std::cout << "Search 1000: "
              << (treap.search(1000) ? "FOUND" : "NOT FOUND")
              << std::endl;

    std::cout << "Height: "
              << treap.height()
              << std::endl;


    // ========================================
    // 9. FINAL SEARCH TEST
    // ========================================

    std::cout << "\n--- FINAL SEARCH TEST ---\n";

    int remainingKeys[] = {
        10, 15, 20, 22, 25,
        35, 40, 45, 50,
        55, 60, 65, 70, 75,
        80
    };

    for (int key : remainingKeys)
    {
        std::cout << "Search " << key << ": ";

        if (treap.search(key))
        {
            std::cout << "FOUND";
        }
        else
        {
            std::cout << "ERROR - NOT FOUND";
        }

        std::cout << std::endl;
    }


    // ========================================
    // 10. FINAL INFORMATION
    // ========================================

    std::cout << "\n--- FINAL TREE INFORMATION ---\n";

    std::cout << "Final height: "
              << treap.height()
              << std::endl;

    std::cout << "Total rotations: "
              << treap.getRotationCount()
              << std::endl;

    std::cout << "\n========================================\n";
    std::cout << "             TEST FINISHED\n";
    std::cout << "========================================\n";
    return 0;
}