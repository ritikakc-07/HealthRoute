#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace FileHandler {

bool saveHospitals(const std::string& filepath, const std::vector<Hospital>& hospitals) {
    std::ofstream out(filepath);
    if (!out.is_open()) {
        std::cout << "  Could not open " << filepath << " for writing.\n";
        return false;
    }
    // id,name,location,totalICUBeds,availableICUBeds
    for (const auto& h : hospitals) {
        out << h.id << "," << h.name << "," << h.location << ","
            << h.totalICUBeds << "," << h.availableICUBeds << "\n";
    }
    out.close();
    return true;
}

bool loadHospitals(const std::string& filepath, std::vector<Hospital>& hospitals) {
    std::ifstream in(filepath);
    if (!in.is_open()) return false;   // no saved data yet -- not an error

    hospitals.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string field;
        Hospital h{};

        std::getline(ss, field, ','); h.id = std::stoi(field);
        std::getline(ss, field, ','); h.name = field;
        std::getline(ss, field, ','); h.location = field;
        std::getline(ss, field, ','); h.totalICUBeds = std::stoi(field);
        std::getline(ss, field, ','); h.availableICUBeds = std::stoi(field);

        hospitals.push_back(h);
    }
    return true;
}

}  // namespace FileHandler
