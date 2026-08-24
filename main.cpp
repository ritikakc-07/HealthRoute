#include <iostream>
#include "include/graph.h"
#include "include/dijkstra.h"
#include "include/hospital.h"

using namespace std;

int main()
{
    // ======================================================
    // Create Lalitpur road network
    // ======================================================

    Graph lalitpur(7);


    // ======================================================
    // Locations
    // ======================================================

    lalitpur.setLocationName(
        0, "Pulchowk"
    );

    lalitpur.setLocationName(
        1, "Jawalakhel"
    );

    lalitpur.setLocationName(
        2, "Lagankhel"
    );

    lalitpur.setLocationName(
        3, "Gwarko"
    );

    lalitpur.setLocationName(
        4, "Satdobato"
    );

    lalitpur.setLocationName(
        5, "Ekantakuna"
    );

    lalitpur.setLocationName(
        6, "Bhaisepati"
    );


    // ======================================================
    // Roads
    // ======================================================

    lalitpur.addRoad(
        0, 1, 1.0
    );

    lalitpur.addRoad(
        1, 2, 1.5
    );

    lalitpur.addRoad(
        2, 3, 2.0
    );

    lalitpur.addRoad(
        3, 4, 2.5
    );

    lalitpur.addRoad(
        0, 5, 1.5
    );

    lalitpur.addRoad(
        5, 6, 2.0
    );

    lalitpur.addRoad(
        5, 2, 2.0
    );


    // ======================================================
    // Hospital Information
    // ======================================================

    Hospital hospitals[] =
    {
        {
            "Alka Hospital",
            1,
            2
        },

        {
            "Patan Hospital",
            2,
            3
        },

        {
            "B&B Hospital",
            3,
            0
        },

        {
            "Nepal Mediciti Hospital",
            6,
            4
        }
    };


    // ======================================================
    // Display Road Network
    // ======================================================

    lalitpur.displayGraph();


    // ======================================================
    // Starting Location
    // Pulchowk = vertex 0
    // ======================================================

    int source = 0;


    // ======================================================
    // Calculate Shortest Distances
    // ======================================================

    Dijkstra::displayShortestDistances(
        lalitpur,
        source
    );


    // ======================================================
    // Display Shortest Routes
    // ======================================================

    Dijkstra::displayShortestRoutes(
        lalitpur,
        source
    );


    // ======================================================
    // Find Nearest Hospital with Available ICU
    // ======================================================

    Dijkstra::findNearestAvailableHospital(
        lalitpur,
        source,
        hospitals,
        4
    );


    // ======================================================
    // Display Hospital Information
    // ======================================================

    cout << "\n===== Hospital Information =====\n";


    for(const Hospital& hospital : hospitals)
    {
        cout << hospital.name
             << " | Location: "
             << lalitpur.getLocationName(
                    hospital.location
                )
             << " | ICU Beds Available: "
             << hospital.availableICUBeds
             << endl;
    }


    return 0;
}