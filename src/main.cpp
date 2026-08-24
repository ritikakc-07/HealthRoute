/*
 * HealthRoute: A Smart Emergency Healthcare Assistance System
 * =============================================================
 * Integrates all three modules behind a single console menu:
 *   - Admin Module   (BST + Merge Sort + File Handling)
 *   - Routing Module (Graph + Dijkstra + Route Reconstruction)
 *   - Ritika Module  (User location flow + Ambulance Queue + Contacts)
 *
 * Build:  make            (see Makefile)
 * Run:    ./healthroute
 */

#include "Hospital.h"
#include "BSTAdmin.h"
#include "MergeSort.h"
#include "Graph.h"
#include "Ambulance.h"
#include "FileHandler.h"

#include <iostream>
#include <limits>
#include <vector>
#include <string>

const std::string DATA_FILE = "data/hospitals.txt";

// ---------------------------------------------------------------------
// Setup helpers
// ---------------------------------------------------------------------

Graph buildLalitpurGraph() {
    Graph g;
    // Junctions (Lalitpur road network -- illustrative distances in km)
    g.addRoad("Pulchowk", "PatanDhoka", 1.2);
    g.addRoad("Pulchowk", "Jawalakhel", 1.0);
    g.addRoad("Jawalakhel", "Kumaripati", 1.5);
    g.addRoad("Kumaripati", "Satdobato", 1.8);
    g.addRoad("Jawalakhel", "Sanepa", 0.8);
    g.addRoad("Sanepa", "Kupondole", 1.0);
    g.addRoad("Kupondole", "Lagankhel", 1.3);
    g.addRoad("Lagankhel", "Satdobato", 1.0);
    g.addRoad("Satdobato", "Ekantakuna", 1.6);
    g.addRoad("PatanDhoka", "Lagankhel", 1.4);

    // Hospitals as nodes, linked to their nearest junction.
    g.addRoad("PatanDhoka", "PatanHospital", 0.5);
    g.addRoad("Kumaripati", "KumudiniHospital", 0.7);
    g.addRoad("Ekantakuna", "NepalMediciti", 0.9);
    return g;
}

std::vector<std::string> selectableLocations() {
    return {"Pulchowk", "PatanDhoka", "Jawalakhel", "Kumaripati",
            "Sanepa", "Kupondole", "Lagankhel", "Satdobato", "Ekantakuna"};
}

std::vector<std::string> hospitalLocations() {
    return {"PatanHospital", "KumudiniHospital", "NepalMediciti"};
}

void seedDefaultHospitals(BSTAdmin& admin) {
    admin.insert({1, "Patan Hospital", "PatanHospital", 20, 4});
    admin.insert({2, "Kumudini Memorial Hospital", "KumudiniHospital", 15, 0});
    admin.insert({3, "Nepal Mediciti", "NepalMediciti", 25, 6});
}

std::vector<Ambulance> defaultFleet() {
    return {
        {"AMB-01", "Rahul", "Available"},
        {"AMB-02", "Sneha", "Available"},
        {"AMB-03", "Imran", "Available"},
    };
}

Hospital* findByLocation(BSTAdmin& admin, const std::string& location) {
    for (auto& h : admin.getAll()) {
        if (h.location == location) return admin.search(h.id);
    }
    return nullptr;
}

// ---------------------------------------------------------------------
// Small input helpers
// ---------------------------------------------------------------------

int readInt(const std::string& prompt) {
    int value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Invalid number, try again: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

// ---------------------------------------------------------------------
// Admin Module (BST + Merge Sort + File Handling)
// ---------------------------------------------------------------------

void adminMenu(BSTAdmin& admin) {
    while (true) {
        std::cout << "\n---- Admin Panel ----\n"
                     "1. Add hospital\n"
                     "2. Update ICU beds\n"
                     "3. Delete hospital\n"
                     "4. Search hospital by ID\n"
                     "5. Display all hospitals (BST in-order)\n"
                     "6. Display hospitals sorted by ICU beds (Merge Sort)\n"
                     "7. Save to file\n"
                     "8. Back to main menu\n";
        int choice = readInt("Choose an option: ");

        switch (choice) {
            case 1: {
                Hospital h;
                h.id = readInt("  Hospital ID: ");
                h.name = readLine("  Name: ");
                h.location = readLine("  Location (must match a graph node): ");
                h.totalICUBeds = readInt("  Total ICU beds: ");
                h.availableICUBeds = readInt("  Available ICU beds: ");
                admin.insert(h);
                std::cout << "  Hospital added.\n";
                break;
            }
            case 2: {
                int id = readInt("  Hospital ID to update: ");
                int available = readInt("  New available ICU bed count: ");
                if (admin.updateICUBeds(id, available))
                    std::cout << "  Updated.\n";
                else
                    std::cout << "  Hospital not found.\n";
                break;
            }
            case 3: {
                int id = readInt("  Hospital ID to delete: ");
                if (admin.remove(id))
                    std::cout << "  Deleted.\n";
                else
                    std::cout << "  Hospital not found.\n";
                break;
            }
            case 4: {
                int id = readInt("  Hospital ID to search: ");
                Hospital* h = admin.search(id);
                if (h)
                    std::cout << "  Found: " << h->name << " at " << h->location
                               << " | ICU " << h->availableICUBeds << "/" << h->totalICUBeds << "\n";
                else
                    std::cout << "  Hospital not found.\n";
                break;
            }
            case 5:
                admin.displayAll();
                break;
            case 6: {
                std::vector<Hospital> all = admin.getAll();
                mergeSortByICUBeds(all, true);   // most available ICU beds first
                std::cout << "  Hospitals ranked by available ICU beds:\n";
                for (const auto& h : all)
                    std::cout << "    " << h.name << " (" << h.location << "): "
                               << h.availableICUBeds << " beds\n";
                break;
            }
            case 7: {
                if (FileHandler::saveHospitals(DATA_FILE, admin.getAll()))
                    std::cout << "  Saved to " << DATA_FILE << "\n";
                break;
            }
            case 8:
                return;
            default:
                std::cout << "  Invalid option.\n";
        }
    }
}

// ---------------------------------------------------------------------
// Emergency Assistance flow (Routing Module + Ritika Module combined)
// ---------------------------------------------------------------------

void emergencyMenu(BSTAdmin& admin, const Graph& graph, DispatchCenter& dispatch) {
    std::cout << "\n---- Emergency Assistance ----\n";

    auto locations = selectableLocations();
    std::cout << "Select your current location:\n";
    for (size_t i = 0; i < locations.size(); i++)
        std::cout << "  " << (i + 1) << ". " << locations[i] << "\n";

    int locChoice = readInt("Enter choice: ");
    if (locChoice < 1 || locChoice > static_cast<int>(locations.size())) {
        std::cout << "  Invalid location selected.\n";
        return;
    }
    std::string userLocation = locations[locChoice - 1];
    std::string emergencyType = readLine("Enter emergency type (e.g. Cardiac, Accident): ");

    std::cout << "\n=== Emergency request: " << emergencyType << " at " << userLocation << " ===\n";

    // Routing Module (Ugin): Dijkstra -> ranked hospital list
    auto ranked = graph.rankedByDistance(userLocation, hospitalLocations());
    std::cout << "  Ranked hospitals by distance:\n";
    for (const auto& [loc, dist] : ranked)
        std::cout << "    " << loc << " -> " << dist << " km\n";

    // Admin Module (BST): filter by ICU bed availability, nearest-first
    Hospital* best = nullptr;
    double bestDist = 0.0;
    for (const auto& [loc, dist] : ranked) {
        Hospital* h = findByLocation(admin, loc);
        if (h && h->availableICUBeds > 0) {
            best = h;
            bestDist = dist;
            break;
        }
    }

    if (!best) {
        std::cout << "  No hospital with an available ICU bed found nearby.\n";
        return;
    }
    std::cout << "  Recommended hospital: " << best->name
               << " (" << best->availableICUBeds << " ICU beds available, "
               << bestDist << " km away)\n";

    // Route reconstruction (parent array)
    std::unordered_map<std::string, double> dist;
    std::unordered_map<std::string, std::string> parent;
    graph.dijkstra(userLocation, dist, parent);
    auto path = graph.reconstructPath(userLocation, best->location, parent);
    std::cout << "  Route: ";
    for (size_t i = 0; i < path.size(); i++)
        std::cout << path[i] << (i + 1 < path.size() ? " -> " : "\n");

    // Ritika module: ambulance dispatch + emergency contacts
    std::string wantsAmbulance = readLine("Request an ambulance? (y/n): ");
    if (!wantsAmbulance.empty() && (wantsAmbulance[0] == 'y' || wantsAmbulance[0] == 'Y')) {
        Ambulance* amb = dispatch.requestAmbulance(best->name);
        if (!amb) std::cout << "  No ambulances currently available.\n";
    }
    DispatchCenter::showEmergencyContacts();
}

// ---------------------------------------------------------------------
// main
// ---------------------------------------------------------------------

int main() {
    BSTAdmin admin;
    std::vector<Hospital> loaded;
    if (FileHandler::loadHospitals(DATA_FILE, loaded) && !loaded.empty()) {
        for (const auto& h : loaded) admin.insert(h);
        std::cout << "Loaded " << loaded.size() << " hospital record(s) from " << DATA_FILE << "\n";
    } else {
        seedDefaultHospitals(admin);
        std::cout << "No saved data found -- seeded default hospital records.\n";
    }

    Graph graph = buildLalitpurGraph();
    DispatchCenter dispatch(defaultFleet());

    while (true) {
        std::cout << "\n===== HealthRoute Main Menu =====\n"
                     "1. Admin Panel\n"
                     "2. Emergency Assistance\n"
                     "3. Show ambulance fleet status\n"
                     "4. Save & Exit\n";
        int choice = readInt("Choose an option: ");

        if (choice == 1) {
            adminMenu(admin);
        } else if (choice == 2) {
            emergencyMenu(admin, graph, dispatch);
        } else if (choice == 3) {
            dispatch.showFleetStatus();
        } else if (choice == 4) {
            FileHandler::saveHospitals(DATA_FILE, admin.getAll());
            std::cout << "Saved. Goodbye.\n";
            break;
        } else {
            std::cout << "  Invalid option.\n";
        }
    }
    return 0;
}
