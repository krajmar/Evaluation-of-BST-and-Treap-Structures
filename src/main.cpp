#include <iostream>
#include "benchmark/benchmark.h"
#include "benchmark/entropy_benchmark.h"
#include "benchmark/sorting.h"

using namespace std;

int main()
{
    cout << "Starting BST vs Treap entropy benchmark...\n";
    runEntropyBenchmark();

    return 0;
}