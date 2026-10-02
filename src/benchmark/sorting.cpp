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