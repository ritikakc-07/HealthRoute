#include "Graph.h"
#include <queue>
#include <algorithm>

void Graph::addNode(const std::string& name) {
    adj.emplace(name, std::vector<std::pair<std::string, double>>{});
}

void Graph::addRoad(const std::string& a, const std::string& b, double distance, bool bidirectional) {
    addNode(a);
    addNode(b);
    adj[a].push_back({b, distance});
    if (bidirectional) adj[b].push_back({a, distance});
}

bool Graph::hasNode(const std::string& name) const {
    return adj.find(name) != adj.end();
}

std::vector<std::string> Graph::nodeNames() const {
    std::vector<std::string> names;
    names.reserve(adj.size());
    for (const auto& [name, _] : adj) names.push_back(name);
    std::sort(names.begin(), names.end());
    return names;
}

void Graph::dijkstra(const std::string& source,
                      std::unordered_map<std::string, double>& dist,
                      std::unordered_map<std::string, std::string>& parent) const {
    dist.clear();
    parent.clear();
    for (const auto& [node, edges] : adj) dist[node] = INF;
    if (adj.find(source) == adj.end()) return;
    dist[source] = 0.0;

    std::unordered_map<std::string, bool> visited;
    using PQItem = std::pair<double, std::string>;
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> minHeap;
    minHeap.push({0.0, source});

    while (!minHeap.empty()) {
        auto [d, u] = minHeap.top();
        minHeap.pop();
        if (visited[u]) continue;
        visited[u] = true;

        for (const auto& [v, weight] : adj.at(u)) {
            double newDist = d + weight;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                parent[v] = u;
                minHeap.push({newDist, v});
            }
        }
    }
}

std::vector<std::string> Graph::reconstructPath(
    const std::string& source, const std::string& destination,
    const std::unordered_map<std::string, std::string>& parent) const {

    std::vector<std::string> path;
    if (source == destination) { path.push_back(source); return path; }

    std::string current = destination;
    while (current != source) {
        path.push_back(current);
        auto it = parent.find(current);
        if (it == parent.end()) return {};   // unreachable
        current = it->second;
    }
    path.push_back(source);
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<std::pair<std::string, double>> Graph::rankedByDistance(
    const std::string& source, const std::vector<std::string>& candidates) const {

    std::unordered_map<std::string, double> dist;
    std::unordered_map<std::string, std::string> parent;
    dijkstra(source, dist, parent);

    std::vector<std::pair<std::string, double>> ranked;
    for (const auto& c : candidates) {
        auto it = dist.find(c);
        if (it != dist.end() && it->second < INF) ranked.push_back({c, it->second});
    }
    std::sort(ranked.begin(), ranked.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });
    return ranked;
}
