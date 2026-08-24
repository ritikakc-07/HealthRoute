#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>

// Core hospital record used by the Admin (BST) module, the routing
// (Graph/Dijkstra) module, and file handling.
struct Hospital {
    int id;
    std::string name;
    std::string location;      // must match a node name in the Graph
    int totalICUBeds;
    int availableICUBeds;
};

#endif
