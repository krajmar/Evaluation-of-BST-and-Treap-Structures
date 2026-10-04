#include <iostream>
#include "benchmark/benchmark.h"
#include "benchmark/sorting.h"

using namespace std;

int main()
{
    cout << "Starting BST vs Treap benchmark...\n";
    runSortingBenchmark();

    return 0;
}