#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <chrono>
#include <filesystem>
#include <stdexcept>
#include <iomanip>
#include <algorithm>

#include "sorting.h"
#include "../bst/bst.h"
#include "../treap/treap.h"

using namespace std;
namespace fs = std::filesystem;
using Clock = std::chrono::steady_clock;
using Nanoseconds = std::chrono::nanoseconds;

// ============================================================
// CLASSIC MERGE SORT
// ============================================================

void classicMerge(vector<int>& a,
                  vector<int>& temp,
                  int left,
                  int mid,
                  int right,
                  long long& comparisons)
{
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right)
    {
        comparisons++;

        if (a[i] <= a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while (i <= mid)
        temp[k++] = a[i++];

    while (j <= right)
        temp[k++] = a[j++];

    for (int p = left; p <= right; p++)
        a[p] = temp[p];
}

void classicMergeSortRecursive(vector<int>& a,
                               vector<int>& temp,
                               int left,
                               int right,
                               long long& comparisons)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    classicMergeSortRecursive(
        a, temp, left, mid, comparisons);

    classicMergeSortRecursive(
        a, temp, mid + 1, right, comparisons);

    classicMerge(
        a, temp, left, mid, right, comparisons);
}

long long classicMergeSort(vector<int>& a)
{
    if (a.size() < 2)
        return 0;

    vector<int> temp(a.size());
    long long comparisons = 0;

    classicMergeSortRecursive(
        a, temp, 0, static_cast<int>(a.size()) - 1,
        comparisons);

    return comparisons;
}

// ============================================================
// NATURAL MERGE SORT
// ============================================================

// Merges two adjacent sorted ranges:
// [left, mid) and [mid, right)
void naturalMerge(vector<int>& a,
                  vector<int>& temp,
                  int left,
                  int mid,
                  int right,
                  long long& comparisons)
{
    int i = left;
    int j = mid;
    int k = left;

    while (i < mid && j < right)
    {
        comparisons++;

        if (a[i] <= a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while (i < mid)
        temp[k++] = a[i++];

    while (j < right)
        temp[k++] = a[j++];

    for (int p = left; p < right; p++)
        a[p] = temp[p];
}

// Detect natural runs, then repeatedly merge adjacent pairs.
long long naturalMergeSort(vector<int>& a)
{
    const int n = static_cast<int>(a.size());

    if (n < 2)
        return 0;

    long long comparisons = 0;

    // Each run is represented by its starting and ending
    // positions: [start, end).
    vector<pair<int, int>> runs;

    int start = 0;

    // --------------------------------------------------------
    // Step 1: Detect the initial natural runs.
    // --------------------------------------------------------

    while (start < n)
    {
        int end = start + 1;

        if (end < n)
        {
            comparisons++;

            // Strictly descending run.
            if (a[end] < a[start])
            {
                end++;

                while (end < n)
                {
                    comparisons++;

                    if (a[end] < a[end - 1])
                        end++;
                    else
                        break;
                }

                // Reverse only strictly descending runs.
                // This preserves stability when equal keys exist.
                std::reverse(a.begin() + start, a.begin() + end);
            }
            else
            {
                // Non-decreasing run.
                end++;

                while (end < n)
                {
                    comparisons++;

                    if (a[end] >= a[end - 1])
                        end++;
                    else
                        break;
                }
            }
        }

        runs.push_back({start, end});
        start = end;
    }

    vector<int> temp(n);

    // --------------------------------------------------------
    // Step 2: Merge adjacent runs in successive passes.
    // --------------------------------------------------------

    while (runs.size() > 1)
    {
        vector<pair<int, int>> nextRuns;
        nextRuns.reserve((runs.size() + 1) / 2);

        size_t i = 0;

        while (i + 1 < runs.size())
        {
            int left = runs[i].first;
            int mid = runs[i].second;
            int right = runs[i + 1].second;

            naturalMerge(
                a, temp, left, mid, right, comparisons);

            nextRuns.push_back({left, right});
            i += 2;
        }

        // Carry an unpaired final run to the next pass.
        if (i < runs.size())
            nextRuns.push_back(runs[i]);

        runs = move(nextRuns);
    }

    return comparisons;
}

long long countInversionsMerge(
    vector<int>& a,
    vector<int>& temp,
    int left,
    int right)
{
    if (left >= right)
        return 0;

    int mid = left + (right - left) / 2;

    long long inversions = 0;

    inversions += countInversionsMerge(
        a, temp, left, mid);

    inversions += countInversionsMerge(
        a, temp, mid + 1, right);

    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right)
    {
        if (a[i] <= a[j])
        {
            temp[k++] = a[i++];
        }
        else
        {
            // Every remaining element in the left half
            // forms an inversion with a[j].
            inversions += (mid - i + 1);

            temp[k++] = a[j++];
        }
    }

    while (i <= mid)
        temp[k++] = a[i++];

    while (j <= right)
        temp[k++] = a[j++];

    for (int p = left; p <= right; p++)
        a[p] = temp[p];

    return inversions;
}


long long countInversions(const vector<int>& original)
{
    if (original.size() < 2)
        return 0;

    vector<int> a = original;
    vector<int> temp(a.size());

    return countInversionsMerge(
        a,
        temp,
        0,
        static_cast<int>(a.size()) - 1
    );
}

// ============================================================
// DATASET READING
// ============================================================

vector<int> readInsertKeys(const string& filename)
{
    ifstream file(filename);

    if (!file.is_open())
        throw runtime_error("Could not open file: " + filename);

    vector<int> keys;
    string line;

    while (getline(file, line))
    {
        istringstream stream(line);

        string operation;
        int key;
        int priority;

        if (!(stream >> operation >> key >> priority))
            continue;

        if (operation == "INSERT")
            keys.push_back(key);
    }

    return keys;
}


// ============================================================
// BENCHMARK
// ============================================================

struct Result
{
    string ordering;
    string algorithm;
    int n;
    int repetition;
    long long timeNs;
    long long comparisons;
    bool sortedCorrectly;
};

Result benchmarkOne(const vector<int>& original,
                    const string& ordering,
                    const string& algorithm,
                    int repetition)
{
    // Every run starts with the same unsorted input.
    vector<int> data = original;

    long long comparisons = 0;

    auto start = Clock::now();

    if (algorithm == "Classic Merge Sort")
        comparisons = classicMergeSort(data);
    else
        comparisons = naturalMergeSort(data);

    auto finish = Clock::now();

    long long elapsedNs =
    std::chrono::duration_cast<Nanoseconds>(
        finish - start
    ).count();

    // Validation is deliberately outside the timed interval.
    bool correct = is_sorted(data.begin(), data.end());

    return {
        ordering,
        algorithm,
        static_cast<int>(original.size()),
        repetition,
        elapsedNs,
        comparisons,
        correct
    };
}

void runSortingBenchmark()
{
    const int DATASET_SIZE = 10000;
    const int REPETITIONS = 3;

    const vector<string> orderings = {
        "ascending",
        "descending",
        "random"
    };

    const vector<string> algorithms = {
        "Classic Merge Sort",
        "Natural Merge Sort"
    };

    ofstream csv("sorting_results.csv");

    if (!csv.is_open())
    {
        cerr << "Could not create sorting_results.csv\n";
    }

    csv << "Ordering,N,Inversions,Algorithm,Repetition,"
           "Time_ns,Time_s,KeyComparisons,SortedCorrectly\n";

    try
    {
        for (const string& ordering : orderings)
        {
            string filename =
                "datasets/" + ordering + "/static_10000.txt";

            vector<int> original = readInsertKeys(filename);

            if (original.size() != DATASET_SIZE)
            {
                cerr << "Unexpected number of INSERT keys in "
                     << filename << ": " << original.size()
                     << " (expected " << DATASET_SIZE << ")\n";
            }

            long long inversions = countInversions(original);

            cout << "\nDataset: " << ordering
                 << " (" << original.size() << " keys)\n";

            cout << "Inversions: "
                 << inversions << '\n';

            for (const string& algorithm : algorithms)
            {
                for (int repetition = 1;
                     repetition <= REPETITIONS;
                     repetition++)
                {
                    Result result = benchmarkOne(
                        original,
                        ordering,
                        algorithm,
                        repetition);

                    double timeSeconds =
                        result.timeNs / 1'000'000'000.0;

                    csv << result.ordering << ','
                        << result.n << ','
                        << inversions << ','
                        << result.algorithm << ','
                        << result.repetition << ','
                        << result.timeNs << ','
                        << timeSeconds << ','
                        << result.comparisons << ','
                        << (result.sortedCorrectly ? "true" : "false")
                        << '\n';

                    // Preserve results if the program stops early.
                    csv.flush();

                    cout << algorithm
                         << " | repetition " << repetition
                         << " | " << result.timeNs << " ns"
                         << " | comparisons: " << result.comparisons
                         << " | sorted: "
                         << (result.sortedCorrectly ? "yes" : "NO")
                         << '\n';

                    if (!result.sortedCorrectly)
                    {
                        cerr << "ERROR: Sorting failed!\n";
                    }
                }
            }
        }
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << '\n';
    }

    csv.close();

    cout << "\nBenchmark completed successfully.\n";
    cout << "Results saved to sorting_results.csv\n";
}