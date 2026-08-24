#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

// Weighted road-network graph (adjacency list) + Dijkstra's Algorithm.
class Graph {
public:
    static constexpr double INF = 1e18;

    void addNode(const std::string& name);
    void addRoad(const std::string& a, const std::string& b, double distance, bool bidirectional = true);
    bool hasNode(const std::string& name) const;
    std::vector<std::string> nodeNames() const;

    // Single-source shortest path (min-heap based).
    // Fills dist[] with shortest distance to every node, and parent[] so the
    // exact route can be reconstructed. Time: O((V + E) log V)
    void dijkstra(const std::string& source,
                  std::unordered_map<std::string, double>& dist,
                  std::unordered_map<std::string, std::string>& parent) const;

    // Walks the parent map backwards from destination to source.
    std::vector<std::string> reconstructPath(
        const std::string& source, const std::string& destination,
        const std::unordered_map<std::string, std::string>& parent) const;

    // Ranked (nearest-first) list of reachable candidate nodes from source.
    std::vector<std::pair<std::string, double>> rankedByDistance(
        const std::string& source, const std::vector<std::string>& candidates) const;

private:
    std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> adj;
};

#endif
