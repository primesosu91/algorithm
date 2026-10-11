#pragma once

struct ReferenceEdge { int from, to; long long cost; };

// 小さいグラフの単純パスと単純サイクルを全列挙する独立した参照解。
struct ReferencePaths {
    static constexpr long long infinity = 1000000000000000LL;
    int n;
    std::vector<std::vector<long long>> distance;
    std::vector<bool> negative_cycle;

    ReferencePaths(int n, const std::vector<ReferenceEdge>& edges, bool directed)
        : n(n), distance(n, std::vector<long long>(n, infinity)), negative_cycle(n, false) {
        std::vector<std::vector<std::pair<int, long long>>> graph(n);
        for (auto e : edges) {
            graph[e.from].push_back({e.to, e.cost});
            if (!directed) graph[e.to].push_back({e.from, e.cost});
        }
        for (int source = 0; source < n; ++source) {
            auto visit = [&](auto&& self, int v, int used, long long cost) -> void {
                distance[source][v] = std::min(distance[source][v], cost);
                for (auto [to, weight] : graph[v]) {
                    if (to == source && cost + weight < 0) negative_cycle[source] = true;
                    if (!(used >> to & 1)) self(self, to, used | (1 << to), cost + weight);
                }
            };
            visit(visit, source, 1 << source, 0);
        }
    }

    bool reachable(int from, int to) const { return distance[from][to] != infinity; }
    bool negative(int from, int to) const {
        for (int v = 0; v < n; ++v) {
            if (negative_cycle[v] && reachable(from, v) && reachable(v, to)) return true;
        }
        return false;
    }
};

void verify_path(const std::vector<int>& vertices, const std::vector<int>& edge_ids,
                 const std::vector<ReferenceEdge>& edges, bool directed,
                 int source, int sink, long long expected) {
    assert(!vertices.empty() && vertices.front() == source && vertices.back() == sink);
    assert(vertices.size() == edge_ids.size() + 1);
    long long cost = 0;
    for (int i = 0; i < (int)edge_ids.size(); ++i) {
        assert(0 <= edge_ids[i] && edge_ids[i] < (int)edges.size());
        auto e = edges[edge_ids[i]];
        bool forward = e.from == vertices[i] && e.to == vertices[i + 1];
        bool reverse = !directed && e.to == vertices[i] && e.from == vertices[i + 1];
        assert(forward || reverse);
        cost += e.cost;
    }
    assert(cost == expected);
}
