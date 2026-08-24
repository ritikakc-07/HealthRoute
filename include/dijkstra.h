#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include "hospital.h"
#include <vector>
#include <string>

class Dijkstra
{
public:
    static std::vector<double> findShortestDistances(
        const Graph& graph,
        const std::string& source
    );

    static void displayShortestDistances(
        const Graph& graph,
        const std::string& source
    );

    static void findNearestAvailableHospital(
        const Graph& graph,
        const std::string& source,
        const Hospital hospitals[],
        int hospitalCount
    );

    static void displayShortestRoutes(
        const Graph& graph,
        const std::string& source
    );
};

#endif