#ifndef AMBULANCE_H
#define AMBULANCE_H

#include <string>
#include <vector>
#include <queue>

struct Ambulance {
    std::string id;
    std::string driverName;
    std::string status;   // "Available" | "Busy"
};

// Ritika's module: user location handling support + FIFO ambulance dispatch
// + emergency contacts.
class DispatchCenter {
public:
    explicit DispatchCenter(std::vector<Ambulance> fleet);

    Ambulance* requestAmbulance(const std::string& destinationHospital);
    void completeTrip(Ambulance* ambulance);
    void showFleetStatus() const;
    static void showEmergencyContacts();

private:
    std::vector<Ambulance> roster;
    std::queue<Ambulance*> availableQueue;
};

#endif
