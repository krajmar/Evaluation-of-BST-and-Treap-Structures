#include <iostream>
#include "benchmark/benchmark.h"
#include "benchmark/entropy_benchmark.h"

using namespace std;

int main()
{
    cout << "Starting BST vs Treap entropy benchmark...\n";
    runBenchmark();

    return 0;
}