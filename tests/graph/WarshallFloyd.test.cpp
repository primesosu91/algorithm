#include "library/graph/WarshallFloyd.cpp"
#include "tests/graph/path_reference.hpp"

int main() {
    WarshallFloyd<long long> empty(0);
    empty.build();
    assert(!empty.has_negative_cycle());
    mt19937 rng(20261012);
    for (bool directed : {false, true}) {
        for (int trial = 0; trial < 500; ++trial) {
            int n = 1 + rng() % 5;
            WarshallFloyd<long long> graph(n, directed);
            vector<ReferenceEdge> edges;
            for (int i = 0, m = rng() % 10; i < m; ++i) {
                edges.push_back({(int)(rng() % n), (int)(rng() % n), (long long)(rng() % 11) - 4});
                auto e = edges.back();
                graph.add_edge(e.from, e.to, e.cost);
            }
            ReferencePaths reference(n, edges, directed);
            for (int repeat = 0; repeat < 2; ++repeat) {
                graph.build();
                bool negative = false;
                for (int source = 0; source < n; ++source) {
                    negative = negative || reference.negative_cycle[source];
                    for (int sink = 0; sink < n; ++sink) {
                        assert(graph.reachable(source, sink) == reference.reachable(source, sink));
                        assert(graph.is_negative_infinite(source, sink) == reference.negative(source, sink));
                        if (!reference.reachable(source, sink) || reference.negative(source, sink)) {
                            assert(graph.get_path(source, sink).empty());
                            assert(graph.get_path_edges(source, sink).empty());
                        } else {
                            assert(graph.dist(source, sink) == reference.distance[source][sink]);
                            verify_path(graph.get_path(source, sink), graph.get_path_edges(source, sink),
                                        edges, directed, source, sink, graph.dist(source, sink));
                        }
                    }
                }
                assert(graph.has_negative_cycle() == negative);
            }
        }
    }
    cout << "WarshallFloyd: exhaustive paths/cycles, restoration, and rebuild passed\n";
}
