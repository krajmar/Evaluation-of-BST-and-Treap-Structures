#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <chrono>
#include <filesystem>
#include <stdexcept>
#include <iomanip>

#include "bst/bst.h"
#include "treap/treap.h"

namespace fs = std::filesystem;
using Clock = std::chrono::steady_clock;
using Nanoseconds = std::chrono::nanoseconds;

constexpr int REPETITIONS = 10;

struct Operation
{
    std::string type;
    int key;
    int priority;
};

struct Dataset
{
    std::string ordering;
    std::string workload;
    int size;
    fs::path path;
};

struct Measurements
{
    long long insertionTotal = 0;
    long long searchTotal = 0;
    long long deletionTotal = 0;

    long long insertionCount = 0;
    long long searchCount = 0;
    long long deletionCount = 0;

    int finalHeight = -1;
    long long rotations = -1;
};
