
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

#include "entropy_benchmark.h"
#include "../bst/bst.h"
#include "../treap/treap.h"

namespace fs = std::filesystem;
using Clock = std::chrono::steady_clock;
using Nanoseconds = std::chrono::nanoseconds;

constexpr int REPETITIONS = 3;

struct OperationEntropy
{
    std::string type;
    int key;
    int priority;
};

struct DatasetEntropy
{
    std::string entropy;
    std::string workload;
    int size;
    fs::path path;
};

struct MeasurementsEntropy
{
    long long insertionTotal = 0;
    long long insertionCount = 0;
    int finalHeight = -1;
    long long rotations = -1;
};

// ------------------------------------------------------------
// Calculate elapsed time
// ------------------------------------------------------------
long long elapsedNanosecondsEntropy(
    Clock::time_point start,
    Clock::time_point end)
{
    return std::chrono::duration_cast<Nanoseconds>(
        end - start).count();
}

// ------------------------------------------------------------
// Read dataset file (ignoring search/delete operations)
// ------------------------------------------------------------
std::vector<OperationEntropy> readDatasetEntropy(
    const fs::path& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Could not open dataset: " + path.string());
    }

    std::vector<OperationEntropy> operations;
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        ++lineNumber;

        if (line.find_first_not_of(" \t\r\n") ==
            std::string::npos)
        {
            continue;
        }

        std::istringstream stream(line);
        OperationEntropy operation;
        std::string extra;

        if (!(stream >> operation.type
                     >> operation.key
                     >> operation.priority))
        {
            throw std::runtime_error(
                "Invalid line " + std::to_string(lineNumber) +
                " in " + path.string());
        }

        if (stream >> extra)
        {
            throw std::runtime_error(
                "Unexpected extra field on line " +
                std::to_string(lineNumber));
        }

        if (operation.type == "INSERT")
        {
            operations.push_back(operation);
        }
    }

    return operations;
}

// ------------------------------------------------------------
// Run dataset on BST
// ------------------------------------------------------------
MeasurementsEntropy runBSTEntropy(
    const std::vector<OperationEntropy>& operations)
{
    BST bst;
    MeasurementsEntropy result;

    for (const OperationEntropy& operation : operations)
    {
        auto start = Clock::now();
        bst.insert(operation.key);
        auto end = Clock::now();

        result.insertionTotal +=
            elapsedNanosecondsEntropy(start, end);

        ++result.insertionCount;
    }

    result.finalHeight = bst.height();

    return result;
}

// ------------------------------------------------------------
// Run dataset on Treap
// ------------------------------------------------------------
MeasurementsEntropy runTreapEntropy(
    const std::vector<OperationEntropy>& operations)
{
    Treap treap;
    MeasurementsEntropy result;

    for (const OperationEntropy& operation : operations)
    {
        auto start = Clock::now();
        treap.insert(operation.key, operation.priority);
        auto end = Clock::now();

        result.insertionTotal +=
            elapsedNanosecondsEntropy(start, end);

        ++result.insertionCount;
    }

    result.finalHeight = treap.height();
    result.rotations = treap.getRotationCount();

    return result;
}

// ------------------------------------------------------------
// Calculate average insertion time
// ------------------------------------------------------------
double averageTimeEntropy(
    long long total,
    long long count)
{
    if (count == 0)
        return 0.0;

    return static_cast<double>(total) / count;
}

// ------------------------------------------------------------
// Find dataset path
// Expected: datasets/k_1/static_10000.txt
// ------------------------------------------------------------
fs::path findDatasetEntropy(
    const std::string& entropy,
    const std::string& workload,
    int size)
{
    return fs::path("datasets") / entropy /
           (workload + "_" + std::to_string(size) + ".txt");
}

// ------------------------------------------------------------
// Write CSV row
// ------------------------------------------------------------
void writeCSVRowEntropy(
    std::ofstream& csv,
    const DatasetEntropy& dataset,
    int repetition,
    const std::string& structure,
    const MeasurementsEntropy& m)
{
    csv << dataset.entropy << ','
        << dataset.workload << ','
        << dataset.size << ','
        << repetition << ','
        << structure << ','
        << m.insertionCount << ','
        << m.insertionTotal << ','
        << std::fixed << std::setprecision(2)
        << averageTimeEntropy(
               m.insertionTotal,
               m.insertionCount) << ','
        << m.finalHeight << ',';

    if (structure == "Treap")
        csv << m.rotations;

    csv << '\n';
}

// ------------------------------------------------------------
// Main entropy benchmark runner
// ------------------------------------------------------------
void runEntropyBenchmark()
{
    const std::array<std::string, 10> entropies = {
        "k_1", "k_2", "k_4", "k_5", "k_8",
        "k_10", "k_16", "k_20", "k_25", "k_40"
    };

    const std::array<std::string, 1> workloads = {
        "static"
    };

    const std::array<int, 2> sizes = {
        10000, 100000
    };

    std::ofstream csv("entropy_results.csv");

    if (!csv.is_open())
    {
        std::cerr << "Could not create entropy_results.csv\n";
        return;
    }

    csv << "Entropy,Workload,N,Repetition,Structure,"
           "InsertCount,InsertTotal_ns,InsertAverage_ns,"
           "FinalHeight,Rotations\n";

    int datasetsProcessed = 0;

    try
    {
        for (const std::string& entropy : entropies)
        {
            for (const std::string& workload : workloads)
            {
                for (int size : sizes)
                {
                    fs::path path =
                        findDatasetEntropy(entropy, workload, size);

                    if (!fs::exists(path))
                    {
                        std::cerr
                            << "WARNING: Dataset not found: "
                            << path.string() << '\n';
                        continue;
                    }

                    std::cout << "\nDataset: "
                              << path.string() << '\n';

                    std::vector<OperationEntropy> operations =
                        readDatasetEntropy(path);

                    std::cout << "Insert Operations Loaded: "
                              << operations.size() << '\n';

                    DatasetEntropy dataset{
                        entropy, workload, size, path
                    };

                    for (int repetition = 1;
                         repetition <= REPETITIONS;
                         ++repetition)
                    {
                        MeasurementsEntropy bstResults =
                            runBSTEntropy(operations);

                        MeasurementsEntropy treapResults =
                            runTreapEntropy(operations);

                        writeCSVRowEntropy(
                            csv, dataset, repetition,
                            "BST", bstResults);

                        writeCSVRowEntropy(
                            csv, dataset, repetition,
                            "Treap", treapResults);

                        csv.flush();

                        std::cout
                            << "  Repetition " << repetition
                            << "/" << REPETITIONS
                            << " completed\n";
                    }

                    ++datasetsProcessed;
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nBenchmark error: "
                  << e.what() << '\n';
    }

    csv.close();

    std::cout << "\n========================================\n"
              << "Benchmark finished.\n"
              << "Datasets processed: " << datasetsProcessed
              << "\nResults saved to: entropy_results.csv\n"
              << "========================================\n";
}
