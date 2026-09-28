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
