#include "library/graph/MinCostFlow.cpp"

struct InputEdge { int from, to, capacity, cost; };

// 全辺の流量を列挙し、保存則を満たす各流量の最小費用を直接求める。
vector<long long> reference(int n, const vector<InputEdge>& edges) {
    vector<long long> best(20, numeric_limits<long long>::max());
    vector<int> balance(n, 0);
    auto enumerate = [&](auto&& self, int index, long long cost) -> void {
        if (index == (int)edges.size()) {
            for (int v = 1; v + 1 < n; ++v) if (balance[v]) return;
            int flow = balance[0];
            if (flow < 0 || flow >= (int)best.size() || balance[n - 1] != -flow) return;
            best[flow] = min(best[flow], cost);
            return;
        }
        auto e = edges[index];
        for (int flow = 0; flow <= e.capacity; ++flow) {
            balance[e.from] += flow;
            balance[e.to] -= flow;
            self(self, index + 1, cost + (long long)flow * e.cost);
            balance[e.from] -= flow;
            balance[e.to] += flow;
        }
    };
    enumerate(enumerate, 0, 0);
    return best;
}

MinCostFlow make_graph(int n, const vector<InputEdge>& edges) {
    MinCostFlow graph(n);
    for (auto e : edges) graph.add_edge(e.from, e.to, e.capacity, e.cost);
    return graph;
}

int main() {
    // 旧実装では番兵以上の距離が未到達として捨てられた。
    const long long maximum = numeric_limits<long long>::max();
    for (long long cost : {maximum / 4 - 1, maximum / 4, maximum / 4 + 1,
                           maximum / 2, maximum}) {
        MinCostFlow graph(3);
        graph.add_edge(0, 1, 1, cost);
        graph.add_edge(1, 2, 1, 0);
        auto result = graph.flow(0, 2, 1);
        assert(result.flow == 1 && result.cost == cost);
        assert(graph.get_edge(0).flow == 1);
        assert(graph.flow(0, 2, 1).flow == 0);
    }
    // 再呼出し時のBellman-Fordでも大きな距離を区別する。
    MinCostFlow repeated(2);
    repeated.add_edge(0, 1, 2, maximum / 4);
    for (int i = 0; i < 2; ++i) {
        auto result = repeated.flow(0, 1, 1);
        assert(result.flow == 1 && result.cost == maximum / 4);
    }

    mt19937 rng(20261012);
    for (int trial = 0; trial < 500; ++trial) {
        int n = 2 + rng() % 4;
        vector<InputEdge> edges;
        for (int i = 0, m = rng() % 9; i < m; ++i) {
            edges.push_back({(int)(rng() % n), (int)(rng() % n),
                             (int)(rng() % 3), (int)(rng() % 8)});
        }
        auto best = reference(n, edges);
        int limit = rng() % 8;
        int expected_flow = limit;
        while (expected_flow && best[expected_flow] == maximum) --expected_flow;
        auto graph = make_graph(n, edges);
        auto slope = graph.slope(0, n - 1, limit);
        assert(slope.front().flow == 0 && slope.front().cost == 0);
        assert(slope.back().flow == expected_flow);
        assert(slope.back().cost == best[expected_flow]);
        long long previous_marginal = -1;
        for (int i = 1; i < (int)slope.size(); ++i) {
            auto left = slope[i - 1], right = slope[i];
            assert(left.flow < right.flow);
            long long marginal = (right.cost - left.cost) / (right.flow - left.flow);
            assert(marginal > previous_marginal);
            previous_marginal = marginal;
            for (int flow = left.flow; flow <= right.flow; ++flow) {
                assert(best[flow] == left.cost + (flow - left.flow) * marginal);
            }
        }
        auto actual_edges = graph.edges();
        vector<long long> balance(n);
        long long cost = 0;
        for (int i = 0; i < (int)edges.size(); ++i) {
            auto actual = actual_edges[i];
            assert(actual.from == edges[i].from && actual.to == edges[i].to);
            assert(actual.capacity == edges[i].capacity && actual.cost == edges[i].cost);
            assert(0 <= actual.flow && actual.flow <= actual.capacity);
            balance[actual.from] += actual.flow;
            balance[actual.to] -= actual.flow;
            cost += actual.flow * actual.cost;
        }
        assert(balance[0] == expected_flow && balance[n - 1] == -expected_flow);
        for (int v = 1; v + 1 < n; ++v) assert(balance[v] == 0);
        assert(cost == best[expected_flow]);
        auto split = make_graph(n, edges);
        auto first = split.flow(0, n - 1, limit / 2);
        auto second = split.flow(0, n - 1, limit - first.flow);
        assert(first.flow + second.flow == expected_flow);
        assert(first.cost + second.cost == best[expected_flow]);
    }
    cout << "MinCostFlow: exhaustive flow/cost, slope, repeat, and large-cost checks passed\n";
}
