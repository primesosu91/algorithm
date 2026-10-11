#include "graph/CycleDetection.cpp"

int main() {
    mt19937 rng(20261012);
    for (bool directed : {false, true}) {
        for (int trial = 0; trial < 500; ++trial) {
            int n = rng() % 12;
            CycleDetection graph(n, directed);
            vector<pair<int, int>> edges;
            for (int step = 0; step < 15; ++step) {
                if (n) {
                    int u = rng() % n, v = rng() % n;
                    assert(graph.add_edge(u, v) == (int)edges.size());
                    edges.push_back({u, v});
                }
                bool expected = false;
                if (directed) {
                    vector<vector<int>> adjacency(n);
                    vector<int> indegree(n);
                    for (auto [u, v] : edges) { adjacency[u].push_back(v); ++indegree[v]; }
                    queue<int> que;
                    for (int v = 0; v < n; ++v) if (!indegree[v]) que.push(v);
                    int removed = 0;
                    while (!que.empty()) {
                        int v = que.front(); que.pop(); ++removed;
                        for (int to : adjacency[v]) if (--indegree[to] == 0) que.push(to);
                    }
                    expected = removed != n;
                } else {
                    vector<int> component(n);
                    iota(component.begin(), component.end(), 0);
                    for (auto [u, v] : edges) {
                        if (component[u] == component[v]) expected = true;
                        int old = component[v], replacement = component[u];
                        for (int& id : component) if (id == old) id = replacement;
                    }
                }
                assert(graph.has_cycle() == expected && graph.has_cycle() == expected);
                auto vertices = graph.get_cycle_vertices();
                auto restored = graph.get_cycle_edges();
                assert(vertices.size() == restored.size());
                assert(!vertices.empty() == expected);
                set<int> used_vertices(vertices.begin(), vertices.end());
                set<int> used_edges(restored.begin(), restored.end());
                assert(used_vertices.size() == vertices.size() && used_edges.size() == restored.size());
                for (int i = 0; i < (int)vertices.size(); ++i) {
                    assert(0 <= restored[i] && restored[i] < (int)edges.size());
                    auto [u, v] = edges[restored[i]];
                    int from = vertices[i], to = vertices[(i + 1) % vertices.size()];
                    assert((u == from && v == to) || (!directed && v == from && u == to));
                }
            }
        }
    }
    cout << "CycleDetection: Kahn/component reference, loops, parallel edges, and restoration passed\n";
}
