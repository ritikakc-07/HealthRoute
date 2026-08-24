#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include "Hospital.h"
#include <vector>

// Sorts hospitals by availableICUBeds. descending = true -> most beds first.
// Time: O(n log n)   Space: O(n)
void mergeSortByICUBeds(std::vector<Hospital>& hospitals, bool descending = true);

#endif
