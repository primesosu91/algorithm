#include "graph/MaxFlow.cpp"

int main() {
    mt19937 rng(20261012);
    for (int trial = 0; trial < 1000; ++trial) {
        int n = 2 + rng() % 6;
        MaxFlow graph(n);
        for (int i = 0, m = rng() % 20; i < m; ++i) {
            graph.add_edge(rng() % n, rng() % n, rng() % 8);
        }
        auto original = graph.edges();
        long long expected = numeric_limits<long long>::max();
        for (int mask = 0; mask < (1 << n); ++mask) {
            if (!(mask & 1) || (mask >> (n - 1) & 1)) continue;
            long long cut = 0;
            for (auto e : original) {
                if ((mask >> e.from & 1) && !(mask >> e.to & 1)) cut += e.capacity;
            }
            expected = min(expected, cut);
        }
        assert(graph.flow(0, n - 1) == expected);
        assert(graph.flow(0, n - 1) == 0);
        auto reachable = graph.min_cut(0);
        auto source_side = graph.source_side(0);
        vector<int> listed;
        for (int v = 0; v < n; ++v) if (reachable[v]) listed.push_back(v);
        assert(source_side == listed);
        assert(reachable[0] && !reachable[n - 1]);
        long long cut = 0;
        vector<long long> balance(n);
        for (auto e : graph.edges()) {
            assert(0 <= e.flow && e.flow <= e.capacity);
            balance[e.from] += e.flow;
            balance[e.to] -= e.flow;
            if (reachable[e.from] && !reachable[e.to]) cut += e.capacity;
        }
        assert(cut == expected && balance[0] == expected && balance[n - 1] == -expected);
        for (int v = 1; v + 1 < n; ++v) assert(balance[v] == 0);
    }
    cout << "MaxFlow: exhaustive cuts and flow conservation passed\n";
}
