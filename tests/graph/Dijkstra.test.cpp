#include "library/graph/Dijkstra.cpp"
#include "tests/graph/path_reference.hpp"

int main() {
    mt19937 rng(20261012);
    for (bool directed : {false, true}) {
        for (int trial = 0; trial < 500; ++trial) {
            int n = 1 + rng() % 5;
            Dijkstra<long long> graph(n, directed);
            vector<ReferenceEdge> edges;
            for (int i = 0, m = rng() % 10; i < m; ++i) {
                edges.push_back({(int)(rng() % n), (int)(rng() % n), (long long)(rng() % 8)});
                auto e = edges.back();
                assert(graph.add_edge(e.from, e.to, e.cost) == i);
            }
            ReferencePaths reference(n, edges, directed);
            for (int source = 0; source < n; ++source) {
                graph.build(source);
                for (int sink = 0; sink < n; ++sink) {
                    assert(graph.reachable(sink) == reference.reachable(source, sink));
                    if (!reference.reachable(source, sink)) {
                        assert(graph.dist(sink) == graph.INF);
                        assert(graph.get_path(sink).empty() && graph.get_path_edges(sink).empty());
                    } else {
                        assert(graph.dist(sink) == reference.distance[source][sink]);
                        verify_path(graph.get_path(sink), graph.get_path_edges(sink), edges,
                                    directed, source, sink, graph.dist(sink));
                    }
                }
            }
        }
    }
    Dijkstra<long long> large(2);
    large.add_edge(0, 1, large.INF - 1);
    large.build(0);
    assert(large.dist(1) == large.INF - 1);
    cout << "Dijkstra: exhaustive simple paths, restoration, and large distances passed\n";
}
