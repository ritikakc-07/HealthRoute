#include <iostream>
#include "include/graph.h"

using namespace std;

int main()
{
    Graph lalitpur(7);

    lalitpur.setLocationName(0, "Pulchowk");
    lalitpur.setLocationName(1, "Jawalakhel");
    lalitpur.setLocationName(2, "Lagankhel");
    lalitpur.setLocationName(3, "Gwarko");
    lalitpur.setLocationName(4, "Satdobato");
    lalitpur.setLocationName(5, "Ekantakuna");
    lalitpur.setLocationName(6, "Bhaisepati");

    lalitpur.addRoad(0, 1, 2.0);
    lalitpur.addRoad(1, 2, 1.5);
    lalitpur.addRoad(2, 3, 2.0);
    lalitpur.addRoad(3, 4, 2.5);
    lalitpur.addRoad(0, 5, 1.5);
    lalitpur.addRoad(5, 6, 2.0);
    lalitpur.addRoad(5, 2, 2.0);

    lalitpur.displayGraph();

    return 0;
}