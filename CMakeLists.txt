cmake_minimum_required(VERSION 3.20)

project(Evaluation_of_BST_and_Treap_Structures)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Evaluation_of_BST_and_Treap_Structures
        src/main.cpp
        src/bst/bst.cpp
        src/treap/treap.cpp
)