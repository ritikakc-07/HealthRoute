#include "Ambulance.h"
#include <iostream>

DispatchCenter::DispatchCenter(std::vector<Ambulance> fleet) : roster(std::move(fleet)) {
    for (auto& amb : roster)
        if (amb.status == "Available") availableQueue.push(&amb);
}

Ambulance* DispatchCenter::requestAmbulance(const std::string& destinationHospital) {
    if (availableQueue.empty()) return nullptr;
    Ambulance* ambulance = availableQueue.front();
    availableQueue.pop();
    ambulance->status = "Busy";
    std::cout << "  -> Ambulance " << ambulance->id << " (driver: " << ambulance->driverName
               << ") dispatched to " << destinationHospital << ". Status: Busy\n";
    return ambulance;
}

void DispatchCenter::completeTrip(Ambulance* ambulance) {
    ambulance->status = "Available";
    availableQueue.push(ambulance);
    std::cout << "  Ambulance " << ambulance->id << " is back in service.\n";
}

void DispatchCenter::showFleetStatus() const {
    std::cout << "  Ambulance fleet status:\n";
    for (const auto& a : roster)
        std::cout << "    " << a.id << " (" << a.driverName << "): " << a.status << "\n";
}

void DispatchCenter::showEmergencyContacts() {
    std::cout << "  Emergency contacts:\n";
    std::cout << "    Ambulance: 102\n";
    std::cout << "    Police: 100\n";
    std::cout << "    Fire Department: 101\n";
}
