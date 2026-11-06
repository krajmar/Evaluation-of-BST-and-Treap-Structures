
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

#include "entropy_benchmark_randomized.h"
#include "../bst/bst.h"
#include "../treap/treap.h"

namespace fs = std::filesystem;
using Clock = std::chrono::steady_clock;
using Nanoseconds = std::chrono::nanoseconds;

constexpr int REPETITIONS = 3;

struct OperationEntropyRandomized
{
    std::string type;
    int key;
    int priority;
};

struct DatasetEntropyRandomized
{
    std::string entropy;
    std::string workload;
    int size;
    fs::path path;
};

struct MeasurementsEntropyRandomized
{
    long long insertionTotal = 0;
    long long insertionCount = 0;
    int finalHeight = -1;
    long long rotations = -1;
};

// ------------------------------------------------------------
// Calculate elapsed time
// ------------------------------------------------------------
long long elapsedNanosecondsEntropyRandomized(
    Clock::time_point start,
    Clock::time_point end)
{
    return std::chrono::duration_cast<Nanoseconds>(
        end - start).count();
}

// ------------------------------------------------------------
// Read dataset file (ignoring search/delete operations)
// ------------------------------------------------------------
std::vector<OperationEntropyRandomized> readDatasetEntropyRandomized(
    const fs::path& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Could not open dataset: " + path.string());
    }

    std::vector<OperationEntropyRandomized> operations;
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
        OperationEntropyRandomized operation;
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
MeasurementsEntropyRandomized runBSTEntropy(
    const std::vector<OperationEntropyRandomized>& operations)
{
    BST bst;
    MeasurementsEntropyRandomized result;

    for (const OperationEntropyRandomized& operation : operations)
    {
        auto start = Clock::now();
        bst.insert(operation.key);
        auto end = Clock::now();

        result.insertionTotal +=
            elapsedNanosecondsEntropyRandomized(start, end);

        ++result.insertionCount;
    }

    result.finalHeight = bst.height();

    return result;
}

// ------------------------------------------------------------
// Run dataset on Treap
// ------------------------------------------------------------
MeasurementsEntropyRandomized runTreapEntropy(
    const std::vector<OperationEntropyRandomized>& operations)
{
    Treap treap;
    MeasurementsEntropyRandomized result;

    for (const OperationEntropyRandomized& operation : operations)
    {
        auto start = Clock::now();
        treap.insert(operation.key, operation.priority);
        auto end = Clock::now();

        result.insertionTotal +=
            elapsedNanosecondsEntropyRandomized(start, end);

        ++result.insertionCount;
    }

    result.finalHeight = treap.height();
    result.rotations = treap.getRotationCount();

    return result;
}

// ------------------------------------------------------------
// Calculate average insertion time
// ------------------------------------------------------------
double averageTimeEntropyRandomized(
    long long total,
    long long count)
{
    if (count == 0)
        return 0.0;

    return static_cast<double>(total) / count;
}

// ------------------------------------------------------------
// Find dataset path
// Expected: datasets/datasets_random_blocks/k_1/static_10000.txt
// ------------------------------------------------------------
fs::path findDatasetEntropyRandomized(
    const std::string& entropy,
    const std::string& workload,
    int size)
{
    return fs::path("datasets/datasets_random_blocks") / entropy /
           (workload + "_" + std::to_string(size) + ".txt");
}

// ------------------------------------------------------------
// Write CSV row
// ------------------------------------------------------------
void writeCSVRowEntropy(
    std::ofstream& csv,
    const DatasetEntropyRandomized& dataset,
    int repetition,
    const std::string& structure,
    const MeasurementsEntropyRandomized& m)
{
    csv << dataset.entropy << ','
        << dataset.workload << ','
        << dataset.size << ','
        << repetition << ','
        << structure << ','
        << m.insertionCount << ','
        << m.insertionTotal << ','
        << std::fixed << std::setprecision(2)
        << averageTimeEntropyRandomized(
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
void runEntropyBenchmarkRandomized()
{
    const std::vector<int> runCounts10000 = {
        1, 2, 4, 5, 8, 10, 16, 20, 25, 40,
        50, 80, 100, 125, 200, 250, 400, 500,
        625, 1000, 1250, 2000, 2500, 5000, 10000
    };

    const std::vector<int> runCounts100000 = {
        1, 2, 4, 5, 8, 10, 16, 20, 25, 32, 40,
        50, 80, 100, 125, 160, 200, 250, 400,
        500, 625, 800, 1000, 1250, 2000, 2500,
        3125, 4000, 5000, 6250, 10000, 12500,
        20000, 25000, 50000, 100000
    };

    const std::array<std::string, 1> workloads = {
        "static"
    };

    const std::array<int, 2> sizes = {
        10000, 100000
    };

    std::ofstream csv("entropy_results_randomized_data.csv");

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
        for (int size : sizes)
        {
            const std::vector<int>& runCounts =
                (size == 10000) ? runCounts10000 : runCounts100000;

            for (int k : runCounts)
            {
                const std::string entropy =
                    "k_" + std::to_string(k);

                const std::string workload = "static";

                fs::path path =
                    fs::path("datasets/datasets_random_blocks") /
                    entropy /
                    ("static_" + std::to_string(size) + ".txt");

                if (!fs::exists(path))
                {
                    std::cerr
                        << "WARNING: Dataset not found: "
                        << path.string() << '\n';
                    continue;
                }

                std::cout << "\nDataset: "
                          << path.string() << '\n';

                std::vector<OperationEntropyRandomized> operations =
                    readDatasetEntropyRandomized(path);

                std::cout << "Insert Operations Loaded: "
                          << operations.size() << '\n';

                DatasetEntropyRandomized dataset{
                    entropy, workload, size, path
                };

                for (int repetition = 1;
                     repetition <= REPETITIONS;
                     ++repetition)
                {
                    MeasurementsEntropyRandomized bstResults =
                        runBSTEntropy(operations);

                    MeasurementsEntropyRandomized treapResults =
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
