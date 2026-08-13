#include "MergeSort.h"
#include <iostream>

using namespace std;


// ==============================
// Merge Function
// ==============================

void MergeSort::merge(vector<Hospital>& hospitals, int left, int mid, int right)
{
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    vector<Hospital> leftArray(leftSize);
    vector<Hospital> rightArray(rightSize);


    // Copy data into temporary arrays

    for (int i = 0; i < leftSize; i++)
    {
        leftArray[i] = hospitals[left + i];
    }

    for (int i = 0; i < rightSize; i++)
    {
        rightArray[i] = hospitals[mid + 1 + i];
    }


    int i = 0;
    int j = 0;
    int k = left;


    // Sorting by ICU Beds (Descending)

    while (i < leftSize && j < rightSize)
    {
        if (leftArray[i].icuBeds >= rightArray[j].icuBeds)
        {
            hospitals[k] = leftArray[i];
            i++;
        }
        else
        {
            hospitals[k] = rightArray[j];
            j++;
        }

        k++;
    }


    // Remaining elements

    while (i < leftSize)
    {
        hospitals[k] = leftArray[i];
        i++;
        k++;
    }


    while (j < rightSize)
    {
        hospitals[k] = rightArray[j];
        j++;
        k++;
    }
}


// ==============================
// Merge Sort Function
// ==============================

void MergeSort::mergeSort(vector<Hospital>& hospitals, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;


        // Divide left part
        mergeSort(hospitals, left, mid);


        // Divide right part
        mergeSort(hospitals, mid + 1, right);


        // Merge sorted parts
        merge(hospitals, left, mid, right);
    }
}


// ==============================
// Display Sorted Hospitals
// ==============================

void MergeSort::displaySortedHospitals(vector<Hospital>& hospitals)
{
    cout << "\n===== Hospitals Sorted By ICU Beds =====\n\n";


    for (Hospital h : hospitals)
    {
        cout << "----------------------------------" << endl;
        cout << "Hospital ID : " << h.id << endl;
        cout << "Name        : " << h.name << endl;
        cout << "Location    : " << h.location << endl;
        cout << "ICU Beds    : " << h.icuBeds << endl;
    }
}