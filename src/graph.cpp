#include "../include/graph.h"
#include <iostream>

using namespace std;

Graph::Graph(int v)
{
    vertices = v;
    adjacencyList.resize(vertices);
    locationNames.resize(vertices);
}

void Graph::setLocationName(int id, string name)
{
    if(id >= 0 && id < vertices)
    {
        locationNames[id] = name;
    }
}

void Graph::addRoad(int source, int destination, double distance)
{
    if(source >= 0 && source < vertices &&
       destination >= 0 && destination < vertices)
    {
        adjacencyList[source].push_back({destination, distance});
        adjacencyList[destination].push_back({source, distance});
    }
}

void Graph::displayGraph()
{
    cout << "\n===== Lalitpur Road Network =====\n";

    for(int i = 0; i < vertices; i++)
    {
        cout << locationNames[i] << " : ";

        for(const Edge &edge : adjacencyList[i])
        {
            cout << " -> "
                 << locationNames[edge.destination]
                 << " (" << edge.distance << " km)";
        }

        cout << endl;
    }
}

int Graph::getVertices() const
{
    return vertices;
}

const vector<Edge>& Graph::getNeighbors(int vertex) const
{
    return adjacencyList[vertex];
}

string Graph::getLocationName(int id) const
{
    return locationNames[id];
}