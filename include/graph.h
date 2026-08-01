#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

using namespace std;

struct Edge
{
    int destination;
    double distance;
};

class Graph
{
private:
    int vertices;
    vector<vector<Edge>> adjacencyList;
    vector<string> locationNames;

public:
    Graph(int v);

    void setLocationName(int id, string name);
    void addRoad(int source, int destination, double distance);

    void displayGraph();
};

#endif