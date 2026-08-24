#include "../include/dijkstra.h"
#include <iostream>
#include <limits>

using namespace std;

vector<double> Dijkstra::findShortestDistances(
    const Graph& graph,
    const string& source)
{
    unordered_map<string, double> dist;
    unordered_map<string, string> parent;

    graph.dijkstra(source, dist, parent);

    vector<string> locations = graph.nodeNames();
    vector<double> distances;

    for (const string& location : locations)
    {
        distances.push_back(dist[location]);
    }

    return distances;
}


void Dijkstra::displayShortestDistances(
    const Graph& graph,
    const string& source)
{
    unordered_map<string, double> dist;
    unordered_map<string, string> parent;

    graph.dijkstra(source, dist, parent);

    cout << "\n===== Shortest Distances from "
         << source
         << " =====\n";

    for (const auto& location : graph.nodeNames())
    {
        cout << location << " : ";

        if (dist[location] >= Graph::INF)
        {
            cout << "Unreachable";
        }
        else
        {
            cout << dist[location] << " km";
        }

        cout << endl;
    }
}


void Dijkstra::displayShortestRoutes(
    const Graph& graph,
    const string& source)
{
    unordered_map<string, double> dist;
    unordered_map<string, string> parent;

    graph.dijkstra(source, dist, parent);

    cout << "\n===== Shortest Routes from "
         << source
         << " =====\n";

    for (const string& destination : graph.nodeNames())
    {
        if (destination == source)
            continue;

        cout << "\n"
             << destination
             << " : ";

        if (dist[destination] >= Graph::INF)
        {
            cout << "Unreachable\n";
            continue;
        }

        vector<string> route =
            graph.reconstructPath(
                source,
                destination,
                parent
            );

        for (size_t i = 0; i < route.size(); i++)
        {
            cout << route[i];

            if (i + 1 < route.size())
                cout << " -> ";
        }

        cout << "\tDistance: "
             << dist[destination]
             << " km\n";
    }
}


void Dijkstra::findNearestAvailableHospital(
    const Graph& graph,
    const string& source,
    const Hospital hospitals[],
    int hospitalCount)
{
    vector<string> candidates;

    for (int i = 0; i < hospitalCount; i++)
    {
        if (hospitals[i].availableICUBeds > 0)
        {
            candidates.push_back(
                hospitals[i].location
            );
        }
    }

    vector<pair<string, double>> ranked =
        graph.rankedByDistance(
            source,
            candidates
        );

    cout << "\n===== Nearest Available ICU Hospital =====\n";

    if (ranked.empty())
    {
        cout << "No hospital with available ICU beds found.\n";
        return;
    }

    string nearestLocation = ranked[0].first;
    double shortestDistance = ranked[0].second;

    int nearestHospital = -1;

    for (int i = 0; i < hospitalCount; i++)
    {
        if (hospitals[i].location == nearestLocation &&
            hospitals[i].availableICUBeds > 0)
        {
            nearestHospital = i;
            break;
        }
    }

    if (nearestHospital == -1)
    {
        cout << "No hospital with available ICU beds found.\n";
        return;
    }

    cout << "Hospital: "
         << hospitals[nearestHospital].name
         << endl;

    cout << "Location: "
         << hospitals[nearestHospital].location
         << endl;

    cout << "Distance from "
         << source
         << ": "
         << shortestDistance
         << " km"
         << endl;

    cout << "Available ICU Beds: "
         << hospitals[nearestHospital].availableICUBeds
         << endl;
}