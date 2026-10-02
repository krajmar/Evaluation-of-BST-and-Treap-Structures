#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

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
                reverse(a.begin() + start, a.begin() + end);
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
