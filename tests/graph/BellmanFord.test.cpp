#include "graph/BellmanFord.cpp"
#include "tests/graph/path_reference.hpp"

int main() {
    mt19937 rng(20261012);
    for (bool directed : {false, true}) {
        for (int trial = 0; trial < 500; ++trial) {
            int n = 1 + rng() % 5;
            BellmanFord<long long> graph(n, directed);
            vector<ReferenceEdge> edges;
            for (int i = 0, m = rng() % 10; i < m; ++i) {
                edges.push_back({(int)(rng() % n), (int)(rng() % n), (long long)(rng() % 11) - 4});
                auto e = edges.back();
                assert(graph.add_edge(e.from, e.to, e.cost) == i);
            }
            ReferencePaths reference(n, edges, directed);
            for (int source = 0; source < n; ++source) {
                graph.build(source);
                bool negative = false;
                for (int sink = 0; sink < n; ++sink) {
                    assert(graph.reachable(sink) == reference.reachable(source, sink));
                    assert(graph.is_negative(sink) == reference.negative(source, sink));
                    negative = negative || reference.negative(source, sink);
                    if (!reference.reachable(source, sink) || reference.negative(source, sink)) {
                        assert(graph.get_path(sink).empty() && graph.get_path_edges(sink).empty());
                    } else {
                        assert(graph.dist(sink) == reference.distance[source][sink]);
                        verify_path(graph.get_path(sink), graph.get_path_edges(sink), edges,
                                    directed, source, sink, graph.dist(sink));
                    }
                }
                assert(graph.has_negative_cycle() == negative);
            }
        }
    }
    BellmanFord<long long> large(3);
    large.add_edge(0, 1, numeric_limits<long long>::max());
    large.add_edge(1, 2, numeric_limits<long long>::max());
    large.add_edge(0, 2, 0);
    large.build(0);
    assert(large.dist(1) == numeric_limits<long long>::max() && large.dist(2) == 0);
    BellmanFord<long long> small(2);
    small.add_edge(0, 1, numeric_limits<long long>::min());
    small.build(0);
    assert(small.dist(1) == numeric_limits<long long>::min());
    cout << "BellmanFord: exhaustive paths/cycles, propagation, and wide intermediates passed\n";
}
