#include "library/tree/TreeDiameter.cpp"

int main() {
    mt19937 rng(20261012);
    for (int trial = 0; trial < 500; ++trial) {
        int n = 1 + rng() % 30;
        TreeDiameter tree(n);
        vector<tuple<int, int, long long>> edges;
        vector<vector<pair<int, long long>>> graph(n);
        for (int v = 1; v < n; ++v) {
            int p = rng() % v;
            long long weight = 1 + rng() % 100;
            tree.add_edge(p, v, weight);
            graph[p].push_back({v, weight}); graph[v].push_back({p, weight});
            edges.push_back({p, v, weight});
        }
        long long expected = 0;
        for (int source = 0; source < n; ++source) {
            vector<long long> distance(n, -1);
            queue<int> que;
            distance[source] = 0; que.push(source);
            while (!que.empty()) {
                int v = que.front(); que.pop();
                for (auto [to, weight] : graph[v]) if (distance[to] == -1) {
                    distance[to] = distance[v] + weight;
                    que.push(to);
                }
            }
            expected = max(expected, *max_element(distance.begin(), distance.end()));
        }
        for (int repeat = 0; repeat < 2; ++repeat) {
            tree.build();
            assert(tree.diameter_length() == expected);
            const auto& vertices = tree.diameter_vertices();
            const auto& restored = tree.diameter_edges();
            auto ends = tree.diameter_endpoints();
            assert(vertices.front() == ends.first && vertices.back() == ends.second);
            assert(vertices.size() == restored.size() + 1);
            long long length = 0;
            for (int i = 0; i < (int)restored.size(); ++i) {
                auto [u, v, weight] = edges[restored[i]];
                assert((u == vertices[i] && v == vertices[i + 1]) || (v == vertices[i] && u == vertices[i + 1]));
                length += weight;
            }
            assert(length == expected);
        }
    }
    cout << "TreeDiameter: all starting vertices, weighted restoration, and rebuild passed\n";
}
