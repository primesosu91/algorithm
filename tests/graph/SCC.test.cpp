#include "library/graph/SCC.cpp"

int main() {
    mt19937 rng(20261012);
    for (int trial = 0; trial < 500; ++trial) {
        int n = rng() % 15;
        SCC graph(n);
        vector<vector<int>> reference(n);
        for (int step = 0; step < 20; ++step) {
            if (n) {
                int from = rng() % n, to = rng() % n;
                graph.add_edge(from, to);
                reference[from].push_back(to);
            }
            graph.build();
            vector<vector<bool>> reach(n, vector<bool>(n));
            for (int source = 0; source < n; ++source) {
                queue<int> que;
                reach[source][source] = true;
                que.push(source);
                while (!que.empty()) {
                    int v = que.front(); que.pop();
                    for (int to : reference[v]) if (!reach[source][to]) {
                        reach[source][to] = true;
                        que.push(to);
                    }
                }
            }
            int listed = 0;
            assert(graph.count() == (int)graph.groups().size());
            for (int id = 0; id < graph.count(); ++id) {
                assert(!graph.groups()[id].empty());
                for (int v : graph.groups()[id]) {
                    assert(graph.component_id(v) == id);
                    ++listed;
                }
            }
            assert(listed == n);
            for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v) {
                assert((graph.component_id(u) == graph.component_id(v)) == (reach[u][v] && reach[v][u]));
                if (reach[u][v] && !reach[v][u]) assert(graph.component_id(u) < graph.component_id(v));
            }
        }
    }
    cout << "SCC: BFS mutual reachability, topological IDs, and rebuild passed\n";
}
