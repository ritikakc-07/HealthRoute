#include "MergeSort.h"

static void merge(std::vector<Hospital>& arr, int left, int mid, int right, bool descending) {
    std::vector<Hospital> leftPart(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<Hospital> rightPart(arr.begin() + mid + 1, arr.begin() + right + 1);

    size_t i = 0, j = 0;
    int k = left;

    while (i < leftPart.size() && j < rightPart.size()) {
        bool takeLeft = descending
            ? (leftPart[i].availableICUBeds >= rightPart[j].availableICUBeds)
            : (leftPart[i].availableICUBeds <= rightPart[j].availableICUBeds);
        arr[k++] = takeLeft ? leftPart[i++] : rightPart[j++];
    }
    while (i < leftPart.size()) arr[k++] = leftPart[i++];
    while (j < rightPart.size()) arr[k++] = rightPart[j++];
}

static void mergeSort(std::vector<Hospital>& arr, int left, int right, bool descending) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid, descending);
    mergeSort(arr, mid + 1, right, descending);
    merge(arr, left, mid, right, descending);
}

void mergeSortByICUBeds(std::vector<Hospital>& hospitals, bool descending) {
    if (hospitals.size() < 2) return;
    mergeSort(hospitals, 0, static_cast<int>(hospitals.size()) - 1, descending);
}
