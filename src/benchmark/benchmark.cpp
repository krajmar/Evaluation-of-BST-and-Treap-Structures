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
long long elapsedNanoseconds(Clock::time_point start,
                             Clock::time_point end)
{
    return std::chrono::duration_cast<Nanoseconds>(
        end - start
    ).count();
}

// ------------------------------------------------------------
// Read the dataset file.
// Every line must contain: OPERATION KEY PRIORITY
// ------------------------------------------------------------

std::vector<Operation> readDataset(const fs::path& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Could not open dataset: " + path.string()
        );
    }

    std::vector<Operation> operations;
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        ++lineNumber;

        if (line.find_first_not_of(" \t\r\n") == std::string::npos)
            continue;

        std::istringstream stream(line);

        Operation operation;
        std::string extra;

        if (!(stream >> operation.type
                     >> operation.key
                     >> operation.priority))
        {
            throw std::runtime_error(
                "Invalid line " + std::to_string(lineNumber) +
                " in " + path.string()
            );
        }

        if (stream >> extra)
        {
            throw std::runtime_error(
                "Unexpected extra field on line " +
                std::to_string(lineNumber) +
                " in " + path.string()
            );
        }

        if (operation.type != "INSERT" &&
            operation.type != "SEARCH" &&
            operation.type != "DELETE")
        {
            throw std::runtime_error(
                "Unknown operation '" + operation.type +
                "' on line " + std::to_string(lineNumber)
            );
        }

        operations.push_back(operation);
    }

    return operations;
}
// ------------------------------------------------------------
// Run the dataset on a fresh BST.
// ------------------------------------------------------------

Measurements runBST(const std::vector<Operation>& operations)
{
    BST bst;
    Measurements result;

    for (const Operation& operation : operations)
    {
        auto start = Clock::now();

        if (operation.type == "INSERT")
        {
            bst.insert(operation.key);
        }
        else if (operation.type == "SEARCH")
        {
            bool found = bst.search(operation.key);
            (void)found;
        }
        else if (operation.type == "DELETE")
        {
            bst.deleteNode(operation.key);
        }

        auto end = Clock::now();
        long long elapsed = elapsedNanoseconds(start, end);

        if (operation.type == "INSERT")
        {
            result.insertionTotal += elapsed;
            ++result.insertionCount;
        }
        else if (operation.type == "SEARCH")
        {
            result.searchTotal += elapsed;
            ++result.searchCount;
        }
        else if (operation.type == "DELETE")
        {
            result.deletionTotal += elapsed;
            ++result.deletionCount;
        }
    }

    result.finalHeight = bst.height();

    return result;
}

// ------------------------------------------------------------
// Run the same dataset on a fresh Treap.
// ------------------------------------------------------------

Measurements runTreap(const std::vector<Operation>& operations)
{
    Treap treap;
    Measurements result;

    for (const Operation& operation : operations)
    {
        auto start = Clock::now();

        if (operation.type == "INSERT")
        {
            treap.insert(operation.key, operation.priority);
        }
        else if (operation.type == "SEARCH")
        {
            bool found = treap.search(operation.key);
            (void)found;
        }
        else if (operation.type == "DELETE")
        {
            treap.deleteNode(operation.key);
        }

        auto end = Clock::now();
        long long elapsed = elapsedNanoseconds(start, end);

        if (operation.type == "INSERT")
        {
            result.insertionTotal += elapsed;
            ++result.insertionCount;
        }
        else if (operation.type == "SEARCH")
        {
            result.searchTotal += elapsed;
            ++result.searchCount;
        }
        else if (operation.type == "DELETE")
        {
            result.deletionTotal += elapsed;
            ++result.deletionCount;
        }
    }

    result.finalHeight = treap.height();
    result.rotations = treap.getRotationCount();

    return result;
}

double averageTime(long long total, long long count)
{
    if (count == 0)
        return 0.0;

    return static_cast<double>(total) / count;
}
