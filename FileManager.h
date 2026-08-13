#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <vector>
#include "BST.h"
#include "Hospital.h"

using namespace std;

class FileManager
{
public:
    // Load hospitals from file
    void loadHospitals(BST &tree);

    // Save hospitals to file
    void saveHospitals(vector<Hospital> hospitals);
};

#endif