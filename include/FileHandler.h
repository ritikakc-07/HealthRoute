#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "Hospital.h"
#include <vector>
#include <string>

// Save/load hospital records to/from a simple CSV text file so admin
// data survives between runs.
namespace FileHandler {
    bool saveHospitals(const std::string& filepath, const std::vector<Hospital>& hospitals);
    bool loadHospitals(const std::string& filepath, std::vector<Hospital>& hospitals);
}

#endif
