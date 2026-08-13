#ifndef MERGESORT_H
#define MERGESORT_H

#include <iostream>
#include <vector>
#include "Hospital.h"

using namespace std;

class MergeSort
{
private:
    // Merge two sorted subarrays
    void merge(vector<Hospital>& hospitals, int left, int mid, int right);

public:
    // Sort hospitals by ICU Beds (Descending)
    void mergeSort(vector<Hospital>& hospitals, int left, int right);

    // Display sorted hospitals
    void displaySortedHospitals(vector<Hospital>& hospitals);
};

#endif