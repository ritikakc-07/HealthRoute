#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <iostream>
#include <string>

using namespace std;

struct Hospital
{
    int id;
    string name;
    string location;
    int icuBeds;

    // Default Constructor
    Hospital()
    {
        id = 0;
        name = "";
        location = "";
        icuBeds = 0;
    }

    // Parameterized Constructor
    Hospital(int hospitalId, string hospitalName, string hospitalLocation, int beds)
    {
        id = hospitalId;
        name = hospitalName;
        location = hospitalLocation;
        icuBeds = beds;
    }
};

#endif