#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include "hospital.h"
#include <vector>

using namespace std;

class Dijkstra
{
public:

    // Calculate shortest distances from source
    static vector<double> findShortestDistances(
        const Graph& graph,
        int source
    );

    // Display shortest distances
    static void displayShortestDistances(
        const Graph& graph,
        int source
    );

    // Find nearest hospital having available ICU beds
    static void findNearestAvailableHospital(
        const Graph& graph,
        int source,
        const Hospital hospitals[],
        int hospitalCount
    );

    // Display shortest route from source to every location
    static void displayShortestRoutes(
        const Graph& graph,
        int source
    );
};

#endif