#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// =====================================
// Load Hospitals From File
// =====================================

void FileManager::loadHospitals(BST &tree)
{
    ifstream file("hospitals.txt");

    if (!file)
    {
        cout << "hospitals.txt not found. Starting with empty database.\n";
        return;
    }

    string line;

    while (getline(file, line))
    {
        if(line.empty())
            continue;

        stringstream ss(line);

        Hospital hospital;
        string temp;

        getline(ss, temp, ',');
        hospital.id = stoi(temp);

        getline(ss, hospital.name, ',');

        getline(ss, hospital.location, ',');

        getline(ss, temp, ',');
        hospital.icuBeds = stoi(temp);

        tree.insert(hospital);
    }

    file.close();
}

// =====================================
// Save Hospitals To File
// =====================================

void FileManager::saveHospitals(vector<Hospital> hospitals)
{
    ofstream file("hospitals.txt");

    if (!file)
    {
        cout << "Unable to save file!" << endl;
        return;
    }

    for (Hospital h : hospitals)
    {
        file << h.id << ","
             << h.name << ","
             << h.location << ","
             << h.icuBeds << endl;
    }

    file.close();
}

