#include "../include/dijkstra.h"
#include <iostream>
#include <limits>
#include <vector>

using namespace std;


// ==========================================================
// Find shortest distances using Dijkstra's Algorithm
// ==========================================================

vector<double> Dijkstra::findShortestDistances(
    const Graph& graph,
    int source)
{
    int vertices = graph.getVertices();

    const double INF =
        numeric_limits<double>::infinity();

    vector<double> distance(vertices, INF);
    vector<bool> visited(vertices, false);

    distance[source] = 0;

    // Dijkstra's Algorithm
    for(int count = 0; count < vertices; count++)
    {
        int current = -1;
        double minimumDistance = INF;

        // Find unvisited vertex with minimum distance
        for(int i = 0; i < vertices; i++)
        {
            if(!visited[i] &&
               distance[i] < minimumDistance)
            {
                minimumDistance = distance[i];
                current = i;
            }
        }

        // No reachable vertex remains
        if(current == -1)
            break;

        visited[current] = true;

        // Update neighboring vertices
        for(const Edge& edge :
            graph.getNeighbors(current))
        {
            int neighbor = edge.destination;

            double newDistance =
                distance[current] + edge.distance;

            if(newDistance < distance[neighbor])
            {
                distance[neighbor] = newDistance;
            }
        }
    }

    return distance;
}


// ==========================================================
// Display shortest distances
// ==========================================================

void Dijkstra::displayShortestDistances(
    const Graph& graph,
    int source)
{
    vector<double> distances =
        findShortestDistances(graph, source);

    cout << "\n===== Shortest Distances from "
         << graph.getLocationName(source)
         << " =====\n";

    for(int i = 0;
        i < graph.getVertices();
        i++)
    {
        cout << graph.getLocationName(i)
             << " : ";

        if(distances[i] ==
           numeric_limits<double>::infinity())
        {
            cout << "Unreachable";
        }
        else
        {
            cout << distances[i]
                 << " km";
        }

        cout << endl;
    }
}


// ==========================================================
// Display shortest routes from source
// ==========================================================

void Dijkstra::displayShortestRoutes(
    const Graph& graph,
    int source)
{
    int vertices = graph.getVertices();

    const double INF =
        numeric_limits<double>::infinity();

    vector<double> distance(vertices, INF);

    vector<int> parent(vertices, -1);

    vector<bool> visited(vertices, false);

    distance[source] = 0;

    // Dijkstra's Algorithm with parent tracking
    for(int count = 0;
        count < vertices;
        count++)
    {
        int current = -1;

        double minimumDistance = INF;

        // Find nearest unvisited vertex
        for(int i = 0;
            i < vertices;
            i++)
        {
            if(!visited[i] &&
               distance[i] < minimumDistance)
            {
                minimumDistance = distance[i];
                current = i;
            }
        }

        if(current == -1)
            break;

        visited[current] = true;

        // Update neighbors
        for(const Edge& edge :
            graph.getNeighbors(current))
        {
            int neighbor = edge.destination;

            double newDistance =
                distance[current] +
                edge.distance;

            /*
             * Update if the new path is shorter.
             *
             * If distances are equal, keep the existing
             * parent so that the first discovered route
             * remains selected.
             */
            if(newDistance < distance[neighbor])
            {
                distance[neighbor] = newDistance;

                parent[neighbor] = current;
            }
        }
    }


    cout << "\n===== Shortest Routes from "
         << graph.getLocationName(source)
         << " =====\n";


    // Display route to every location
    for(int destination = 0;
        destination < vertices;
        destination++)
    {
        // Do not display route from source to itself
        if(destination == source)
            continue;


        cout << "\n"
             << graph.getLocationName(destination)
             << " : ";


        if(distance[destination] == INF)
        {
            cout << "Unreachable"
                 << endl;

            continue;
        }


        // Store route backwards
        vector<int> route;

        int current = destination;


        while(current != -1)
        {
            route.push_back(current);

            current = parent[current];
        }


        // Display route in correct order
        for(int i = route.size() - 1;
            i >= 0;
            i--)
        {
            cout << graph.getLocationName(
                route[i]
            );

            if(i != 0)
            {
                cout << " -> ";
            }
        }


        cout << "\tDistance: "
             << distance[destination]
             << " km"
             << endl;
    }
}


// ==========================================================
// Find nearest hospital with available ICU
// ==========================================================

void Dijkstra::findNearestAvailableHospital(
    const Graph& graph,
    int source,
    const Hospital hospitals[],
    int hospitalCount)
{
    vector<double> distances =
        findShortestDistances(graph, source);


    int nearestHospital = -1;

    double shortestDistance =
        numeric_limits<double>::infinity();


    // Check every hospital
    for(int i = 0;
        i < hospitalCount;
        i++)
    {
        /*
         * Ignore hospitals that have no
         * ICU beds available.
         */
        if(hospitals[i].availableICUBeds > 0)
        {
            int hospitalLocation =
                hospitals[i].location;


            // Check if this hospital is nearer
            if(distances[hospitalLocation] <
               shortestDistance)
            {
                shortestDistance =
                    distances[hospitalLocation];

                nearestHospital = i;
            }
        }
    }


    cout << "\n===== Nearest Available ICU Hospital =====\n";


    if(nearestHospital == -1)
    {
        cout << "No hospital with available ICU beds found."
             << endl;
    }
    else
    {
        cout << "Hospital: "
             << hospitals[nearestHospital].name
             << endl;


        cout << "Location: "
             << graph.getLocationName(
                    hospitals[nearestHospital].location)
             << endl;


        cout << "Distance from "
             << graph.getLocationName(source)
             << ": "
             << shortestDistance
             << " km"
             << endl;


        cout << "Available ICU Beds: "
             << hospitals[nearestHospital].availableICUBeds
             << endl;
    }
}